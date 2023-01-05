## Set default configuration
TARGET = ./program
CC = g++
FLAGS = --std=c++17

## Make array of all object files for library
OBJS =
OBJS += src/main.cpp
OBJS += src/menu.cpp
OBJS += src/menu_render.cpp
OBJS += src/file_handler.cpp
OBJS += src/database_controller.cpp
OBJS += src/encoder.cpp

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(TARGET)

%.o: %.cpp
	$(CC) $(FLAGS) -c $< -o $@

rebuild: clean all

clean:
	rm -rf $(TARGET) src/*.o