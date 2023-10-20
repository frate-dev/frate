CC = g++ 
CFLAGS = -Wall -Wextra -Wpedantic -Wold-style-cast -std=c++20


SRC := ./src
BUILD := ./build
SRCS := $(shell find $(SRC) -name *.cpp)
OBJS := $(subst $(SRC)/,$(BUILD)/,$(addsuffix .o,$(basename $(SRCS))))

.PHONY: all 
all: main 


main: $(OBJS)
	$(CC) $(CFLAGS)  -o $(BUILD)/$@ $^

$(BUILD)/%.o: $(SRC)/%.cpp
	$(CC)  $(CFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -rf ./build/*.o ./build/main


