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

CFLAGS += $(INC) -Wall -std=c99 -O3 -g  -Wextra

ifdef DEBUG
CFLAGS += -DDEBUG
endif

LDFLAGS = -lm

# Liste des fichiers objet

# Professeurs
OBJPROF_FILES =
# Étudiants
OBJ_FILES = $(OBJ_DIR)/shared.o $(OBJ_DIR)/block.o $(OBJ_DIR)/zigzag.o $(OBJ_DIR)/rgb.o $(OBJ_DIR)/picture.o $(OBJ_DIR)/mcu.o $(OBJ_DIR)/jpeg2ppm.o $(OBJ_DIR)/component.o $(OBJ_DIR)/huffman.o  $(OBJ_DIR)/jpeg_reader.o $(OBJ_DIR)/bitstream.o

# cible par défaut

TARGET = $(BIN_DIR)/jpeg2ppm

all: $(TARGET)

$(TARGET): $(OBJPROF_FILES) $(OBJ_FILES)
	$(LD) $(LDFLAGS) $(OBJPROF_FILES) $(OBJ_FILES) -o $(TARGET)

$(OBJ_DIR)/shared.o: $(SRC_DIR)/shared.c $(INC_DIR)/shared.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/shared.c -o $(OBJ_DIR)/shared.o

$(OBJ_DIR)/rgb.o: $(SRC_DIR)/rgb.c $(INC_DIR)/rgb.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/rgb.c -o $(OBJ_DIR)/rgb.o

$(OBJ_DIR)/block.o: $(SRC_DIR)/block.c $(INC_DIR)/block.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/block.c -o $(OBJ_DIR)/block.o

$(OBJ_DIR)/zigzag.o: $(SRC_DIR)/zigzag.c $(INC_DIR)/zigzag.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/zigzag.c -o $(OBJ_DIR)/zigzag.o

$(OBJ_DIR)/picture.o: $(SRC_DIR)/picture.c $(INC_DIR)/picture.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/picture.c -o $(OBJ_DIR)/picture.o

$(OBJ_DIR)/mcu.o: $(SRC_DIR)/mcu.c $(INC_DIR)/mcu.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/mcu.c -o $(OBJ_DIR)/mcu.o

$(OBJ_DIR)/component.o: $(SRC_DIR)/component.c $(INC_DIR)/component.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/component.c -o $(OBJ_DIR)/component.o

$(OBJ_DIR)/jpeg2ppm.o: $(SRC_DIR)/jpeg2ppm.c $(INC_DIR)/jpeg2ppm.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/jpeg2ppm.c -o $(OBJ_DIR)/jpeg2ppm.o

$(OBJ_DIR)/bitstream.o: $(SRC_DIR)/bitstream.c $(INC_DIR)/bitstream.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/bitstream.c -o $(OBJ_DIR)/bitstream.o

$(OBJ_DIR)/huffman.o: $(SRC_DIR)/huffman.c $(INC_DIR)/huffman.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/huffman.c -o $(OBJ_DIR)/huffman.o

$(OBJ_DIR)/jpeg_reader.o: $(SRC_DIR)/jpeg_reader.c $(INC_DIR)/jpeg_reader.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/jpeg_reader.c -o $(OBJ_DIR)/jpeg_reader.o
	    
debug: clean
	$(MAKE) DEBUG=1

.PHONY: clean

clean:
	rm -f $(TARGET) $(OBJ_FILES)
	rm -f images/*ppm images/*pgm
