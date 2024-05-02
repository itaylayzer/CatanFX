CC=clang
CFLAGS=-g -I.
LDFLAGS=
SRC_DIR=.
OBJ_DIR=obj
EXE=main

SRCS=$(wildcard $(SRC_DIR)/*.c) \
     $(wildcard $(SRC_DIR)/controller/*.c) \
     $(wildcard $(SRC_DIR)/model/*.c) \
     $(wildcard $(SRC_DIR)/server/*.c) \
     $(wildcard $(SRC_DIR)/structures/avl_tree/*.c) \
     $(wildcard $(SRC_DIR)/structures/graph/*.c) \
	 $(wildcard $(SRC_DIR)/utils/math/*.c) \
	 $(wildcard $(SRC_DIR)/structures/linked_list/*.c) \
	 $(wildcard $(SRC_DIR)/structures/linked_list/circular/*.c) \
	 $(wildcard $(SRC_DIR)/structures/queue/*.c) \
	 $(wildcard $(SRC_DIR)/utils/printt/*.c)

WRITE_SRCS=$(wildcard $(SRC_DIR)/utils/math/*.c) \
		$(wildcard $(SRC_DIR)/write/*.c)

WRITE_OBJS=$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(WRITE_SRCS))

OBJS=$(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))

HDRS=$(wildcard $(SRC_DIR)/*.h) \
     $(wildcard $(SRC_DIR)/controller/*.h) \
     $(wildcard $(SRC_DIR)/model/*.h) \
     $(wildcard $(SRC_DIR)/server/*.h) \
     $(wildcard $(SRC_DIR)/structures/avl_tree/*.h) \
     $(wildcard $(SRC_DIR)/structures/graph/*.h) \
	 $(wildcard $(SRC_DIR)/utils/math/*.h)

all: 
	mkdir -p $(OBJ_DIR)
	@for src in $(SRCS); do \
		obj=$$(echo $$src | sed 's|$(SRC_DIR)/\(.*\)\.c|$(OBJ_DIR)/\1|' | xargs dirname); \
		mkdir -p $$obj; \
		$(CC) $(CFLAGS) -c $$src -o $${obj}/$$(basename $$src .c).o;  \
	done

	clang $(OBJS) -o $(EXE) $(LDFLAGS)

run: all
	./$(EXE)
r: run
clean:
	rm -rf ${OBJ_DIR}

wr:
	rm -rf ${OBJ_DIR}
	mkdir -p $(OBJ_DIR)
	@for src in $(WRITE_SRCS); do \
		obj=$$(echo $$src | sed 's|$(SRC_DIR)/\(.*\)\.c|$(OBJ_DIR)/\1|' | xargs dirname); \
		mkdir -p $$obj; \
		$(CC) -c $$src -o $${obj}/$$(basename $$src .c).o;  \
	done

	clang $(WRITE_OBJS) -o $(EXE) $(LDFLAGS)
	./$(EXE)