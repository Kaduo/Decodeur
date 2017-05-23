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
LDFLAGS += -lm

# Liste des fichiers objet

# les notres...
OBJPROF_FILES =  $(OBJPROF_DIR)/huffman.o  $(OBJPROF_DIR)/jpeg_reader.o $(OBJPROF_DIR)/bitstream.o

# et les votres!
OBJ_FILES = $(OBJ_DIR)/extraction.o $(OBJ_DIR)/ppm.o $(OBJ_DIR)/reconstruction.o $(OBJ_DIR)/rgb.o $(OBJ_DIR)/upsampling.o $(OBJ_DIR)/jpeg2ppm.o

# cible par défaut

TARGET = $(BIN_DIR)/jpeg2ppm

all: $(TARGET)

$(TARGET): $(OBJPROF_FILES) $(OBJ_FILES)
	$(LD) $(LDFLAGS) $(OBJPROF_FILES) $(OBJ_FILES) -o $(TARGET)

#$(OBJ_DIR)/example.o: $(SRC_DIR)/example.c $(INC_DIR)/jpeg_reader.h $(INC_DIR)/bitstream.h
#	$(CC) $(CFLAGS) -c $(SRC_DIR)/example.c -o $(OBJ_DIR)/example.o

$(OBJ_DIR)/extraction.o: $(SRC_DIR)/extraction.c $(INC_DIR)/extraction.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/extraction.c -o $(OBJ_DIR)/extraction.o

$(OBJ_DIR)/ppm.o: $(SRC_DIR)/ppm.c $(INC_DIR)/ppm.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/ppm.c -o $(OBJ_DIR)/ppm.o

$(OBJ_DIR)/reconstruction.o: $(SRC_DIR)/reconstruction.c $(INC_DIR)/reconstruction.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/reconstruction.c -o $(OBJ_DIR)/reconstruction.o

$(OBJ_DIR)/rgb.o: $(SRC_DIR)/rgb.c $(INC_DIR)/rgb.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/rgb.c -o $(OBJ_DIR)/rgb.o

$(OBJ_DIR)/upsampling.o: $(SRC_DIR)/upsampling.c $(INC_DIR)/upsampling.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/upsampling.c -o $(OBJ_DIR)/upsampling.o

$(OBJ_DIR)/jpeg2ppm.o: $(SRC_DIR)/jpeg2ppm.c #Rajouter les includes ici ! $(INC_DIR)/nom_du_fichier.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/jpeg2ppm.c -o $(OBJ_DIR)/jpeg2ppm.o


.PHONY: clean

clean:
	rm -f $(TARGET) $(OBJ_FILES)
