CC = g++
CFLAGS = -std=c++17 -Wall -o
INPUT = main
OUT = $(INPUT)

all: $(INPUT)

$(OUT): $(INPUT).cpp
	$(CC) $(INPUT).cpp $(CFLAGS) $(OUT)
