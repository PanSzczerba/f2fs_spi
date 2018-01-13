CC=gcc
CFLAGS= -Wall -std=gnu99
LIBRARY= -lwiringPi
ifdef DEBUG
CFLAGS+= -g
endif

#Dirs
SPI_DIR=./src/main/spi
SD_DIR=./src/main/sd
F2FS_DIR=./src/main/f2fs
TEST_DIR=./src/test

INCLUDE= -I$(SPI_DIR) \
		 -I$(SD_DIR) \
         -I$(F2FS_DIR)

%.o: %.c
	$(CC) -c $(CFLAGS) -o $@ $(INCLUDE) $< 

all: $(TEST_DIR)/main #\

$(TEST_DIR)/main: $(TEST_DIR)/main.o $(SD_DIR)/rw.o $(SD_DIR)/mbr.o $(SD_DIR)/init.o $(SPI_DIR)/spi.o $(F2FS_DIR)/superblock.o $(F2FS_DIR)/checkpoint.o $(F2FS_DIR)/natentry.o $(F2FS_DIR)/inode.o
	$(CC) -o $@ $^ $(LIBRARY)

#$(SPI_DIR)/libspi.so: $(SPI_DIR)/spi.o
#	$(CC) -shared -o $@ $<

#$(SPI_DIR)/spi.o: $(SPI_DIR)/spi.c $(SPI_DIR)/spi.h
#	$(CC) -c -o $@ $(CFLAGS) $(INCLUDE) -fPIC $<
