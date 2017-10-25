CC=gcc
CFLAGS= -Wall -std=gnu99
INCLUDE= -I./main/src/sd_spi \
		 -I./main/src/sd_rw
LIBRARY= -lwiringPi
ifdef DEBUG
CFLAGS+= -g
endif

#Dirs
SPI_DIR=./main/src/sd_spi
RW_DIR=./main/src/sd_rw

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $(INCLUDE) $< 

all: $(RW_DIR)/main \
	$(SPI_DIR)/libspi.so

$(RW_DIR)/main: $(RW_DIR)/main.o $(RW_DIR)/rw_block.o $(SPI_DIR)/spi.o
	$(CC) -o $@ $^ $(LIBRARY)

$(RW_DIR)/rw_block.o: $(RW_DIR)/rw_block.c
	$(CC) -c $(CFLAGS) $(INCLUDE) -o $@ $^ 

$(SPI_DIR)/libspi.so: $(SPI_DIR)/spi.o
	$(CC) -shared -o $@ $<

$(SPI_DIR)/spi.o: $(SPI_DIR)/spi.c $(SPI_DIR)/spi.h
	$(CC) -c -o $@ $(CFLAGS) $(INCLUDE) -fPIC $<
