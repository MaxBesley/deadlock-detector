# A simple Makefile for Project 1 of COMP30023. By Max Besley.

CC=gcc
CFLAGS=-Wall

SRC_DIR=src
SRC_FILES=$(wildcard $(SRC_DIR)/*.c)

OBJ_DIR=obj
OBJ_FILES=$(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

BIN_DIR=bin
EXE_NAME=detect
EXE=$(BIN_DIR)/$(EXE_NAME)


all: $(EXE) clean

$(EXE): $(OBJ_FILES)
	@mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) $^ -o $@
	@echo "-------------- DONE: EXECUTABLE CREATED --------------"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@


clean:
	@echo "Deleting object files..."
	rm -f $(OBJ_DIR)/*.o
	rmdir $(OBJ_DIR)

format:
	clang-format -i *.c *.h
