IR Aquarium Lamp Timer
======================

Adafruit Feather HUZZAH based aquarium lamp timer. Hands-free time accuracy is achieved by syncing with an NTP server.

Requirements
------------

Software
^^^^^^^^

* `PlatformIO <http://platformio.org/>`_
* Libraries:
  * Time
  * NTPClient
  * IRremoteESP8266
  * Adafruit_FeatherOLED
  * Adafruit GFX
  * Adafruit SSD1306

Hardware
^^^^^^^^

This code was written

* Adafruit Feather HUZZAH
* Adafruit FeatherWing Proto
* Adafruit FeatherWing OLED 128x32 Display
* Feather Stacking Headers
* 1x 2N2222 NPN transistor
* 1x 100Ω resistor
* 1x 5mm IR LED

Diagrams coming soon

Building the Code
-----------------

First, clone the git repo. You'll need to edit src/conf.h and supply your WiFi SSID and password, and the host for the NTP server you'd like to use. Then, build and deploy to your Feather HUZZAH by invoking ``pio run``. That's it!

Caveats and Areas for Future Improvement
----------------------------------------

If you'd like to use this code as a base for your own project, please do! Below are the

Timezone
^^^^^^^^

The code as-written assumes the device resides in the United States in the PST timezone. This area of the codebase could be more featureful, but this was enough for my purposes.

To change the timezone:

1. in src/tzoffsets.h, add new timezone offsets for your timezone, if they are not present
2. in src/main.cpp:
   * in the function set_st, change ``ntpc.setTimeOffset(PST);`` to use your standard timezone offset.
   * in the function set_dt, change ``ntpc.setTimeOffset(PDT);`` to use your daylight savings timezone.

To change the timezone rules:

1. In src/util.cpp, change ``get_dst_start`` and ``get_dst_end`` to match your locale's daylight savings start/end days.
2. In src/main.cpp, change ``atd.add_task(yr, 3, get_dst_start(yr), 0, 0, set_dt);`` and ``atd.add_task(yr, 11, get_dst_end(yr), 0, 0, set_st);`` to specify the correct months for your locale's daylight savings rules.


Lamp On/Off Hours
^^^^^^^^^^^^^^^^^

The code as-written also hardcodes the lamp on/off times. These can be changed by updating the values for ``ON_HR`` and ``OFF_HR`` in ``main.cpp`` on lines 18 and 19. This could be changed to store values in non-volatile memory and allow them to be selected with the buttons on the FeatherWing OLED, but there is no support for that in the code currently.

Lamp On/Off Remote Codes
^^^^^^^^^^^^^^^^^^^^^^^^

The IR codes used are specific to our aquarium lamp. I found these by breadboarding a basic IR receiver circuit with another Feather HUZZAH I had. I used the `IRrecvDumpV2 example <https://github.com/markszabo/IRremoteESP8266/tree/master/examples/IRrecvDumpV2>`_ to capture the codes I needed.

To change the codes sent, capture the codes needed for the lamp or other IR device you're controlling, using its remote, and update ``LAMP_OFF`` and ``LAMP_ON`` on lines 15 and 16 of main.cpp. If your device receives a different style of IR code than NEC, or the code is not 32-bits, you'll need to change ``irsend.sendNEC(LAMP_ON, 32);`` on line 87 of main.cpp, and ``irsend.sendNEC(LAMP_OFF, 32);`` on line 104 of main.cpp.


Adding More Times
^^^^^^^^^^^^^^^^^

Adding to this code to blast IR at more times (like if you want to add some dimming, or perhaps some lamp color changes), you'll need to add additional task functions. Use ``send_off`` or ``send_on`` as a template. Schedule them by calling ``atd.add_task`` in ``setup``. Also: in lib/Atduino/Atduino.h, increase ``ATD_QUEUE_SIZE``.
