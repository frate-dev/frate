CC = g++ 
CFLAGS = -Wall -Wextra -Wpedantic -Wold-style-cast -std=c++20 -I./include
MAKEFLAGS += -j5


SRC := ./src
BUILD := ./build
RESOURCES := ./resources/*
SRCS := $(shell find $(SRC) -name *.cpp)
OBJS := $(subst $(SRC)/,$(BUILD)/,$(addsuffix .o,$(basename $(SRCS))))

.PHONY: all 
all: main 


main: $(OBJS)
	cp -r $(RESOURCES) $(BUILD)
	$(CC) $(CFLAGS)  -o $(BUILD)/$@ $^

$(BUILD)/%.o: $(SRC)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf ./build/*.o ./build/main


