CC=avr-gcc
OBJCOPY=avr-objcopy
CFLAGS=-mmcu=attiny85 -Os -I/usr/local/include
PROGFLAGS=-B100 -p t85 -F -c $(ISP) -e
#ISP=avrisp2
#ISP=jtag3isp
ISP=usbtiny

programs = controller 

all: ${programs}

clean:
	rm -f *.hex *~ *.o ${programs} *.S

%.S: %.c
	$(CC) $(CFLAGS) -o $@ -S $<
%.hex: %
	$(OBJCOPY) -j .text -j .data -O ihex $< $@

install-%: %.hex
	avrdude $(PROGFLAGS) -U flash:w:$<
