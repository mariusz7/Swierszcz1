#####################################################################
# Description: avr-gcc projects Makefile
# Created at: Mon Jul 18 00:14:18 CEST 2005
# System: Linux 2.6.10-helios-5 on i686
#####################################################################

CC=avr-gcc
OBJCOPY=avr-objcopy
OBJDUMP=avr-objdump

HEXFILE=program.hex
AVR_TYPE=atmega16
UISP_AVR_TYPE=ATmega16

#FINAL_RELEASE jest dla konfiguracji ostatecznej, bez diody i lcd

#dla FINAL_RELEASE - wersja finalna programu do balonu
OBJECTS=program.o common.o dallas.o morse.o radio.o ds18b20.o
OBJECTS_FLAGS=-g -DFINAL_RELEASE -Wall -Os -mmcu=$(AVR_TYPE) -c

#dla DEBUG_RELEASE - wlaczony kod dla lcd i led
#OBJECTS=program.o common.o dallas.o morse.o radio.o ds18b20.o led.o lcd.o
#OBJECTS_FLAGS=-g -DDEBUG_RELEASE -Wall -Os -mmcu=$(AVR_TYPE) -c


#Jezeli chce sie zobaczyc ostrzezenia odnosnie konfiguracji, nalezy dodac
#do OBJECTS_FLAGS flage -DSHOW_WARNINGS

$(HEXFILE) : clean program.out
	$(OBJCOPY) -j .text -j .data -O ihex program.out $(HEXFILE)

program.out : $(OBJECTS)
	$(CC) -g -Wall -o program.out -mmcu=$(AVR_TYPE) -Wl,-Map,program.map $(OBJECTS)

program.o : program.c
	$(CC) $(OBJECTS_FLAGS) program.c

common.o : common.c common.h
	$(CC) $(OBJECTS_FLAGS) common.c

radio.o : radio.c radio.h
	$(CC) $(OBJECTS_FLAGS) radio.c

morse.o : morse.c morse.h
	$(CC) $(OBJECTS_FLAGS) morse.c

ds18b20.o : ds18b20.c ds18b20.h
	$(CC) $(OBJECTS_FLAGS) ds18b20.c

dallas.o : dallas.c dallas.h
	$(CC) $(OBJECTS_FLAGS) dallas.c

led.o : led.c led.h
	$(CC) $(OBJECTS_FLAGS) led.c

lcd.o : lcd.c lcd.h
	$(CC) $(OBJECTS_FLAGS) lcd.c

erase :
	uisp -dprog=stk200 -dpart=$(UISP_AVR_TYPE) --erase -v=3

upload :
	uisp -dprog=stk200 -dpart=$(UISP_AVR_TYPE) --upload if=program.hex -v=3

verify :
	uisp -dprog=stk200 -dpart=$(UISP_AVR_TYPE) --verify if=program.hex -v=3

download :
	uisp -dprog=stk200 -dpart=$(UISP_AVR_TYPE) --download of=program.avr -v=3

rd_fuses :
	uisp -dprog=stk200 -dpart=$(UISP_AVR_TYPE) --rd_fuses -v=3

#droga na skroty do 2 najczesciej wykorzystywanych opcji
eu : erase upload

clean :
	rm -f $(HEXFILE)
	rm -f program.out
	rm -f program.map
	rm -f $(OBJECTS)

size :
	$(OBJDUMP) -h program.out

asm :
	$(OBJDUMP) -S program.out
