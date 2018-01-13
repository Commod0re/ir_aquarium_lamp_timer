#!/usr/bin/make -f

IRALT_MODE = IRALT_MODE_IR

.PHONY: all mode-ir mode-relay upload

all: upload

mode-ir: upload

mode-relay: IRALT_MODE = IRALT_MODE_RELAY
mode-relay: upload

upload:
	env LATFORMIO_BUILD_FLAGS=-D$(IRALT_MODE) \
		pio run -t upload