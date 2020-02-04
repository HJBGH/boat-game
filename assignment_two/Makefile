#No point in writing a makefile for windows, I'll never compile it
#on that platform.

TARGET = assignment_two
LDFLAGS = -lGL -lGLU -lglut -lm
CFLAGS = -Wall -std=gnu99 -g
#I know these lines are getting long but I don't know how to write good makefiles
HEADERS = *.h
$(TARGET): $(TARGET).c *.o
	gcc $(TARGET).c keyboard.c terrain.c mouse.c boats.c island.c projectiles.c utils.c turrets.c libSOIL.a -o $(TARGET) $(LDFLAGS) $(CFLAGS)

%.o: %.c $(HEADERS)
	gcc $(CFLAGS) -c $<

clean: 
	rm $(TARGET) *.o
