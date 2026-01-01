CC = g++
CFLAGS = -Wall -std=c++11
OBJ = main.o rezerwacja.o
TARGET = system_rezerwacji

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJ)

main.o: main.cpp rezerwacja.h
	$(CC) $(CFLAGS) -c main.cpp

rezerwacja.o: rezerwacja.cpp rezerwacja.h
	$(CC) $(CFLAGS) -c rezerwacja.cpp

clean:
	rm -f *.o $(TARGET)
