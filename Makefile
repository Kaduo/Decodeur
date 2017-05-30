# Repertoires du projet

BIN_DIR = bin
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
OBJPROF_DIR = obj-prof

# Options de compilation/édition des liens

CC = clang
LD = clang
INC = -I$(INC_DIR)

CFLAGS += $(INC) -Wall -std=c99 -O0 -g  -Wextra
LDFLAGS = -lm

# Liste des fichiers objet

# les notres...
OBJPROF_FILES =  $(OBJPROF_DIR)/huffman.o  $(OBJPROF_DIR)/jpeg_reader.o $(OBJPROF_DIR)/bitstream.o

# et les votres!
OBJ_FILES = $(OBJ_DIR)/example.o $(OBJ_DIR)/block.o $(OBJ_DIR)/zigzag.o $(OBJ_DIR)/format.o $(OBJ_DIR)/picture.o

# cible par défaut

TARGET = $(BIN_DIR)/example

all: $(TARGET)

$(TARGET): $(OBJPROF_FILES) $(OBJ_FILES)
	$(LD) $(LDFLAGS) $(OBJPROF_FILES) $(OBJ_FILES) -o $(TARGET)

$(OBJ_DIR)/example.o: $(SRC_DIR)/example.c $(INC_DIR)/jpeg_reader.h $(INC_DIR)/bitstream.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/example.c -o $(OBJ_DIR)/example.o

$(OBJ_DIR)/block.o: $(SRC_DIR)/block.c $(INC_DIR)/block.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/block.c -o $(OBJ_DIR)/block.o

$(OBJ_DIR)/zigzag.o: $(SRC_DIR)/zigzag.c $(INC_DIR)/zigzag.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/zigzag.c -o $(OBJ_DIR)/zigzag.o

$(OBJ_DIR)/format.o: $(SRC_DIR)/format.c $(INC_DIR)/format.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/format.c -o $(OBJ_DIR)/format.o


$(OBJ_DIR)/picture.o: $(SRC_DIR)/picture.c $(INC_DIR)/picture.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/picture.c -o $(OBJ_DIR)/picture.o

$(OBJ_DIR)/mcu.o: $(SRC_DIR)/mcu.c $(INC_DIR)/mcu.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/mcu.c -o $(OBJ_DIR)/mcu.o

$(OBJ_DIR)/jpeg2ppm.o: $(SRC_DIR)/jpeg2ppm.c $(INC_DIR)/jpeg2ppm.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/jpeg2ppm.c -o $(OBJ_DIR)/jpeg2ppm.o

$(OBJ_DIR)/upsampling.o: $(SRC_DIR)/upsampling.c $(INC_DIR)/upsampling.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/upsampling.c -o $(OBJ_DIR)/upsampling.o


.PHONY: clean

clean:
	rm -f $(TARGET) $(OBJ_FILES)
