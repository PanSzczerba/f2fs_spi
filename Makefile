CC=gcc
CFLAGS= -Wall -std=gnu99
LIBRARY= -lwiringPi
ifdef DEBUG
CFLAGS+= -g
endif

#Dirs
SPI_DIR=./src/main/spi
SD_DIR=./src/main/sd
TEST_DIR=./src/test

INCLUDE= -I$(SPI_DIR) \
		 -I$(SD_DIR)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $(INCLUDE) $< 

all: $(TEST_DIR)/main #\

$(TEST_DIR)/main: $(TEST_DIR)/main.o $(SD_DIR)/sd.o $(SPI_DIR)/spi.o
	$(CC) -o $@ $^ $(LIBRARY)

#$(SPI_DIR)/libspi.so: $(SPI_DIR)/spi.o
#	$(CC) -shared -o $@ $<

#$(SPI_DIR)/spi.o: $(SPI_DIR)/spi.c $(SPI_DIR)/spi.h
#	$(CC) -c -o $@ $(CFLAGS) $(INCLUDE) -fPIC $<
