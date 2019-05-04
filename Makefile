.SUFFIXES:	.c

CC		= gcc
CFLAGS	= -Wall -g -I./include
SRC		= $(wildcard ./src/*.c)
OBJ		= $(SRC:./src/%.c=./build/%.o)
DEP		= $(OBJ:%.o=%.d)


.PHONY: all
all: ./build ./build/rtty

./build:
	@[ -d $@ ] || mkdir $@

-include $(DEP)

.SECONDARY:
./build/%.o: ./src/%.c
	@echo -e "\033[0;33m*\033[0m $@"
	@$(CC) $(CFLAGS) -MMD -c $< -o $@

./build/rtty: $(OBJ)
	@echo -e "\033[0;33m*\033[0m $@"
	@$(CC) $(OBJ) -o $@

.PHONY: clean
clean:
	@rm -r ./build
