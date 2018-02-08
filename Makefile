##
#    LPC2148 interface
#
#    Makefile:
#             Makefile ARM-testboard
#
#    opmerkingen:
#                -Dit bestand beschrijft het compilatieproces van de code
#                 De indeling is als volgt:
#                 1. algemene instellingen
#                 2. definitie van te compileren code
#                 3. compilatieproces
#

NAME            = TEMPLATE

##############
# Instellingen
##

# optimalisatie
 # De optimalisatie-instellingen van de compiler.
 # Kies -O0 als je aan het testen bent. Kies anders voor -O1 of -O2.
OPT             = -Os

# werken we vanuit ROM of RAM?
 # Kies RUN_FROM_ROM (flash) of RUN_FROM_RAM (jtag)
MEM             = RUN_FROM_ROM

# debug instellingen
 # standaard debug wordt ingesteld met -g.
 # Helemaal geen debugcode door dit leeg te laten.
 # Zie voor overige opties de manpage van GCC
#DBG             = -g

# Globale defines
 # Het is mogelijk om vanuit het Makefile defines door te geven
 # naar de code. Zo kan bijvoorbeeld vanuit het Makefile aan worden
 # gegeven of een bepaald stuk code wel of niet mee wordt gecompileerd.
 # Let op: voor elke define moet een '-D' !
#DEFINES         =

# Include instelling
 # Door hier een mapnaam aan toe te voegen, zal deze aan het standaard
 # include path worden toegevoegd. Hierdoor kun je bestanden met <>
 # includen, b.v #include <bestand.h>.
 # Let op: voor elke include moet een '-I' !
#INCLUDE_PROJECT =

##############
# wat te compileren
##


# welke drivers moeten we compileren?
 # Door een hekje ('#') voor een regel te zetten wordt de desbetreffende code
 # niet meegecompileerd.
# Sourcefiles

SRC             = ./src
INC				= ./inc
LIB				= ./lib


# welke sourcesfiles
SRC_FILES		= $(SRC)/main.o $(SRC)/isp.c $(SRC)/serial_fifo.o

# welke drivers moeten we compileren?
drivers         = exceptions.o
drivers        += buzzer.o
drivers        += pll.o
drivers        += uart.o
drivers        += delay.o
drivers        += print.o
drivers        += usbcontrol.o
drivers        += usbhw_lpc.o
drivers        += usbinit.o
drivers        += usbstdreq.o
drivers        += printf_usb.o
drivers        += uart_usb.o
drivers        += kaku.o
drivers        += RC5.o

# de dingen hieronder hoeven voor uC/OSII niet te worden aangepast
################################################################################
################################################################################

# path-settings
#PREFIX          = @
 # Die '@' hiervoor zorgt ervoor dat het eigenlijke compileer-commando niet zichtbaar is op het scherm
 # Je krijft dan dus alleen de warnings e/o errors te zien; maakt het wat overzichtelijker.

AS              = $(PREFIX)arm-elf-as
CC              = $(PREFIX)arm-elf-gcc
CPP             = $(PREFIX)arm-elf-g++
OBJCOPY         = $(PREFIX)arm-elf-objcopy
INSIGHT         = $(PREFIX)arm-elf-insight
READELF         = $(PREFIX)arm-elf-readelf

# output settings
ELF             = $(NAME).elf
HEX             = $(NAME).hex
MAP             = $(NAME).map
MAP             = $(NAME).map
ifeq ($(MEM),RUN_FROM_RAM)
LNK             = Linkerscript_RAM
else
LNK             = Linkerscript_ROM
endif

# Set compiler options
INCLUDES        = -I $(INC) -I $(SRC) -I $(LIB)
DEFINES         = -D__CPU_MODE__=0
WARNINGSETTINGS = -Wall -Wshadow -Wpointer-arith -Wbad-function-cast -Wcast-align -Wsign-compare \
                  -Waggregate-return -Wstrict-prototypes -Wmissing-prototypes -Wmissing-declarations -Wunused
CFLAGS          = $(DBG) -pipe $(WARNINGSETTINGS) -O3 -mcpu=arm7tdmi -mtune=arm7tdmi -mstructure-size-boundary=32\
                  -fpack-struct -fno-builtin $(INCLUDES) $(OPT) -D $(MEM) $(DEFINES)

LDFLAGS         = -Wl,-Map,$(MAP),-T,$(LNK) -nostartfiles

driver_objects=$(patsubst %.o,$(LIB)/%.o,$(drivers))


all: $(ELF) $(HEX)

$(ELF): $(driver_objects) $(SRC_FILES) crt0.o
	$(CC) $(LDFLAGS) $(driver_objects) $(SRC_FILES) -o $(ELF)

$(HEX): $(ELF)
ifeq ($(MEM),RUN_FROM_ROM)
	$(OBJCOPY) -O ihex $(ELF) $(HEX)
endif

crt0.o: crt0.S
	$(CC) $(CFLAGS) -c crt0.S


debug: $(ELF)
ifeq ($(MEM),RUN_FROM_ROM)
	@echo Code is voor het flash gecompileerd; werkt niet via JTAG
else
	$(if $(shell pslist | grep -i OcdLib), ,  cmd /c start OcdLibRemote --cpu ARM7 --device WIGGLER --speed 1)
	$(INSIGHT) $(ELF)
endif

clean:
	rm -f $(wildcard $(SRC)/*.o) $(wildcard $(LIB)/*.o) $(MAP) $(ELF) $(HEX) #*/

realclean:clean
	rm -f $(driver_objects)
