CC=gcc
CFLAGS= -Wall -std=gnu99
LIBRARY= -lwiringPi
ifdef DEBUG
CFLAGS+= -g
endif

#Dirs
SPI_DIR=./src/main/sd_spi
RW_DIR=./src/main/sd_rw
TEST_DIR=./src/test

INCLUDE= -I$(SPI_DIR) \
		 -I$(RW_DIR)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $(INCLUDE) $< 

all: $(TEST_DIR)/main #\
#	$(SPI_DIR)/libspi.so

$(TEST_DIR)/main: $(TEST_DIR)/main.o $(RW_DIR)/rw_block.o $(SPI_DIR)/spi.o
	$(CC) -o $@ $^ $(LIBRARY)

#$(SPI_DIR)/libspi.so: $(SPI_DIR)/spi.o
#	$(CC) -shared -o $@ $<

#$(SPI_DIR)/spi.o: $(SPI_DIR)/spi.c $(SPI_DIR)/spi.h
#	$(CC) -c -o $@ $(CFLAGS) $(INCLUDE) -fPIC $<
