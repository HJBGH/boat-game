#No point in writing a makefile for windows, I'll never compile it
#on that platform.

TARGET = assignment_one
LDFLAGS = -lGL -lGLU -lglut -lm
CFLAGS = -Wall -std=c99
HEADERS = includes.h keyboard.h boats.h island.h
$(TARGET): $(TARGET).c keyboard.o boats.o island.o
	gcc $(TARGET).c keyboard.c boats.c island.c -o $(TARGET) $(LDFLAGS) $(CFLAGS)

%.o: %.c $(HEADERS)
	gcc $(CFLAGS) -c $<

clean: 
	rm $(TARGET) *.o
