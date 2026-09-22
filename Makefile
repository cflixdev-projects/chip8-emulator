CC = gcc
CFLAGS = -Wall -Iheaderfiles
LDFLAGS = -lsdl3

OBJS = main.o init_chip.o load_rom.o emu_cycle.o
TARGET = chip8-emu

$(TARGET): $(OBJS)
	$(CC) $(OBJS) -o $(TARGET) $(LDFLAGS)

main.o: main.c headerfiles/load_rom.h headerfiles/init_chip.h headerfiles/emu_cycle.h
	$(CC) $(CFLAGS) -c main.c

load_rom.o: load_rom.c headerfiles/load_rom.h
	$(CC) $(CFLAGS) -c load_rom.c

init_chip.o: init_chip.c headerfiles/init_chip.h
	$(CC) $(CFLAGS) -c init_chip.c

emu_cycle.o: emu_cycle.c headerfiles/emu_cycle.h
	$(CC) $(CFLAGS) -c emu_cycle.c

run: $(TARGET)
	./$(TARGET) Pong.ch8

clean:
	rm -f *.o $(TARGET)

.PHONY: clean run