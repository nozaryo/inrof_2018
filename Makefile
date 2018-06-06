WORKNAME = inrof
OBJFILES = main.o
DEVICE = atmega1284p
CFLAGS = -mmcu=$(DEVICE) -Os
LDFLAFGS =
PORT = /dev/ttyUSB0
LFUSE = 0b11100010
HFUSE = 0b11011001

%.o: %.c
	avr-g++ $(CFLAGS) $^ -o $@

test:
	avrdude -c avrisp -P $(PORT) -b 19200 -p $(DEVICE)

write: $(OBJFILES)
	sudo avrdude -c avrisp -P $(PORT) -b 19200 -p $(DEVICE) -U flash:w:$(OBJFILES):e

fuse:
	avrdude -c avrisp -P $(PORT) -b 19200 -p $(DEVICE) -U lfuse:w:$(LFUSE):m
	avrdude -c avrisp -P $(PORT) -b 19200 -p $(DEVICE) -U hfuse:w:$(HFUSE):m

push:
	git push -u origin master
commit:
	git commit -a -m "little change"
clean:
	rm -f $(OBJFILES)
