#No point in writing a makefile for windows, I'll never compile it
#on that platform.

TARGET = tute_one
LDFLAGS = -lGL -lGLU -lglut
CFLAGS = -Wall -std=c99
$(TARGET): $(TARGET).c
	gcc $(TARGET).c -o $(TARGET) $(LDFLAGS) $(CFLAGS)

clean: 
	rm $(TARGET)
