TARGET = ./program
CC = g++
FLAGS = 

SRC = $(wildcard src/*.cpp)
OBJ = $(patsubst src/%.cpp, src/%.o, $(SRC))

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(FLAGS) $(OBJ) -o $(TARGET)

%.o: %.cpp
	$(CC) $(FLAGS) -c $< -o $@

rebuild: clean all

clean:
	rm -rf $(TARGET) src/*.o