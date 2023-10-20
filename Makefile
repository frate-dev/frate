CC = g++


BUILD_DIR = build
SRC_DIR = src
PROJECT_NAME = cmakegen


build:
	$(CC) $(SRC_DIR)/main.cpp -o $(BUILD_DIR)/$(PROJECT_NAME)
run:
	./$(BUILD_DIR)/$(PROJECT_NAME)
clean:
	rm -r ./$(BUILD_DIR)/*

.PHONY: build run clean
