## Set default configuration
TARGET = ./program
CC = g++
FLAGS = --std=c++17 -w

## Make array of all object files for library
OBJS =
OBJS += src/main.o
OBJS += src/menu.o
OBJS += src/file_handler.o
OBJS += src/database_controller.o
OBJS += src/encoder.o


all: $(TARGET) clean_o

$(TARGET): $(OBJS)
	$(CC) $(FLAGS) $(OBJS) -o $(TARGET)

%.o: %.cpp
	$(CC) $(FLAGS) -c $< -o $@

rebuild: clean all

clean:
	rm -rf $(TARGET) $(OBJS)

clean_o:
	rm -rf $(OBJS)