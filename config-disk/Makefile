# Download AppleCommander from https://applecommander.github.io/install/
AC ?= java -jar AppleCommander-ac-1.8.0.jar

BASIC_SRCS = STARTUP.bas

AppleIIVGA-Config.dsk: STARTUP.bas
	cp DOS_3.3_boot.dsk $@~
	$(AC) -d $@~ STARTUP
	$(AC) -bas $@~ STARTUP < STARTUP.bas
	mv $@~ $@

clean:
	rm -f AppleIIVGA-Config.dsk *~
