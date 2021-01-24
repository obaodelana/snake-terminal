CC = g++
CFLAGS = -lncurses -std=c++11 -D_DEFAULT_SOURCE -D_XOPEN_SOURCE=600
TARGET = snake

$(TARGET): main.cpp
	$(CC) main.cpp -o $(TARGET) $(CFLAGS)

clean:
	$(RM) $(TARGET)
