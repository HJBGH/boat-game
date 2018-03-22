#No point in writing a makefile for windows, I'll never compile it
#on that platform.

TARGET = assignment_one
LDFLAGS = -lGL -lGLU -lglut
CFLAGS = -Wall -std=c99
HEADERS = includes.h keyboard.h
$(TARGET): $(TARGET).c keyboard.o
	gcc $(TARGET).c keyboard.c -o $(TARGET) $(LDFLAGS) $(CFLAGS)

%.o: %.c $(HEADERS)
	gcc $(CFLAGS) -c $<

clean: 
	rm $(TARGET) *.o
