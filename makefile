CC = gcc
CFLAGS = -g -Wall
TARGET = LorenzAttractor

all: $(TARGET)

$(TARGET): $(TARGET).c
	$(CC)	$(CFLAGS)	-o	$(TARGET)	$(TARGET).c	-lglut	-lGLU -lGL -lm


clean:
	$(RM) LorenzAttractor
