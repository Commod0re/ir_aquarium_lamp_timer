/* Copyright 2017 Michael Greene
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
 
#include "conf.h"

#include <Arduino.h>
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <IRremoteESP8266.h>
#include <FeatherOLED_Aquarium.h>
#include <Atduino.h>
#include <TimeLib.h>

#ifdef IRALT_MODE_IR
#include <IRsend.h>
#endif /* IRALT_MODE_IR */

#include "util.h"
#include "tzoffsets.h"


#ifdef IRALT_MODE_IR
// remote codes to send
#define LAMP_OFF  0x00F740BFUL
#define LAMP_ON   0x00F7C03FUL
#endif /* IRALT_MODE_IR */
// lamp will be on from 8:00AM - 4:00PM
#define ON_HR     8
#define OFF_HR    16
#define CONTROL_PIN    15

enum meridiem {am, pm};


const char *ssid = IRALT_WIFI_SSID;
const char *pass = IRALT_WIFI_PSK;
// bool last_daytime;
WiFiUDP ntpUDP;

Atduino atd = Atduino();
NTPClient ntpc(ntpUDP, IRALT_NTP_SERVER, PST);
FeatherOLED_Aquarium oled = FeatherOLED_Aquarium();

#ifdef IRALT_MODE_IR
IRsend irsend(CONTROL_PIN);
#endif /* IRALT_MODE_IR */



String aqua_formatted_time(unsigned long epochtime) {
    meridiem m = am;
    int hrs = hourFormat12(epochtime);
    int min = minute(epochtime);
    String hoursStr, minutesStr, meridiemStr;

    hoursStr = (hrs < 10 ? "0" : "") + String(hrs);
    minutesStr = (min < 10 ? "0" : "") + String(min);
    meridiemStr = isAM(epochtime)?"AM":"PM";

    return hoursStr + ":" + minutesStr + " " + meridiemStr + " ";
}



void set_st(struct atduino_timespec_t &scheduled) {
    Serial.println("set_pst");
    unsigned long epochtime = ntpc.getEpochTime();
    uint16_t yr = year(epochtime) + 1;

    ntpc.setTimeOffset(PST);
    ntpc.update();

    // schedule next run
    atd.add_task(yr, 11, get_dst_end(yr), 0, 0, set_st);
}


void set_dt(struct atduino_timespec_t &scheduled) {
    Serial.println("set_pdt");
    unsigned long epochtime = ntpc.getEpochTime();
    uint16_t yr = year(epochtime) + 1;

    ntpc.setTimeOffset(PDT);
    ntpc.update();

    // schedule next run
    atd.add_task(yr, 11, get_dst_end(yr), 0, 0, set_dt);
}


void send_on(struct atduino_timespec_t &scheduled) {
    Serial.println("send_on");
    tmElements_t sched;
    sched.Minute = scheduled.minute;
    sched.Hour = scheduled.hour;
    sched.Day = scheduled.day;
    sched.Month = scheduled.month;
    sched.Year = scheduled.year - 1970;
    unsigned long tomorrow = nextMidnight(makeTime(sched));
    oled.setIcon(daytime);

#ifdef IRALT_MODE_IR
    irsend.sendNEC(LAMP_ON, 32);
#endif /* IRALT_MODE_IR */
#ifdef IRALT_MODE_RELAY
    digitalWrite(CONTROL_PIN, HIGH);
#endif /* IRALT_MODE_RELAY */

    // schedule next run
    atd.add_task(year(tomorrow), month(tomorrow), day(tomorrow), ON_HR, 0, send_on);
}


void send_off(struct atduino_timespec_t &scheduled) {
    Serial.println("send_off");
    tmElements_t sched;
    sched.Minute = scheduled.minute;
    sched.Hour = scheduled.hour;
    sched.Day = scheduled.day;
    sched.Month = scheduled.month;
    sched.Year = scheduled.year - 1970;
    unsigned long tomorrow = nextMidnight(makeTime(sched));
    oled.setIcon(nighttime);

#ifdef IRALT_MODE_IR
    irsend.sendNEC(LAMP_OFF, 32);
#endif /* IRALT_MODE_IR */
#ifdef IRALT_MODE_RELAY
    digitalWrite(CONTROL_PIN, LOW);
#endif /* IRALT_MODE_RELAY */

    // schedule next run
    atd.add_task(year(tomorrow), month(tomorrow), day(tomorrow), OFF_HR, 0, send_off);
}


void setup() {
    unsigned long epochtime;
    uint16_t yr;
    uint8_t mo, dy;

    // put your setup code here, to run once:
    Serial.begin(115200);
#ifdef IRALT_MODE_IR
    irsend.begin();
#endif /* IRALT_MODE_IR */
#ifdef IRALT_MODE_RELAY
    pinMode(CONTROL_PIN, OUTPUT);
#endif /* IRALT_MODE_RELAY */

    // screen
    oled.init();
    oled.setBatteryVisible(false);
    oled.setBatteryIcon(false);
    oled.setRSSIVisible(true);
    oled.setConnected(false);

    // connect to wifi
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, pass);
    while (WiFi.status() != WL_CONNECTED) {
        delay(500);
    }
    oled.setConnected(true);
    oled.setIPAddress(WiFi.localIP());
    oled.setRSSI(WiFi.RSSI());

    // connect to NTP
    ntpc.begin();
    ntpc.update();

    epochtime = ntpc.getEpochTime();
    yr = year(epochtime);
    mo = month(epochtime);
    dy = day(epochtime);

    // add PST/DST selection tasks
    // DST begins on the second Sunday of March
    atd.add_task(yr, 3, get_dst_start(yr), 0, 0, set_dt);
    atd.add_task(yr, 11, get_dst_end(yr), 0, 0, set_st);

    // add ON/OFF tasks
    atd.add_task(yr, mo, dy, ON_HR, 0, send_on);
    atd.add_task(yr, mo, dy, OFF_HR, 0, send_off);
}


void loop() {
    struct atduino_timespec_t curtime;
    unsigned long ts;

    // clear the display
    oled.clearDisplay();

    // update NTP client
    ntpc.update();

    // do atd stuff
    ts = ntpc.getEpochTime();
    curtime.minute = minute(ts);
    curtime.hour = hour(ts);
    curtime.day = day(ts);
    curtime.month = month(ts);
    curtime.year = year(ts);
    atd.check(curtime);

    // print time to the message area
    oled.setCursor(10, 12);
    // oled.println(timeClient.getFormattedTime());
    oled.println(aqua_formatted_time(ts));

    // update status values
    oled.setIPAddress(WiFi.localIP());
    oled.setRSSI(WiFi.RSSI());

    // update screen
    oled.refreshIcons();

    delay(1000);
}
