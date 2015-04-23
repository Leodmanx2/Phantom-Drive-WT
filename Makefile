PROJECT_NAME = Phantom-Drive

SRCDIR = $(CURDIR)/src
OBJDIR = $(CURDIR)/obj

OBJ = $(OBJDIR)/main.o $(OBJDIR)/Application.o

FLAGS = -std=c++0x -Wall -c

all: $(OBJ)
	c++ -o $(PROJECT_NAME) $(OBJ) -lSDL2

$(OBJDIR)/main.o: $(SRCDIR)/main.cpp
	c++ $(FLAGS) -o $(OBJDIR)/main.o $(SRCDIR)/main.cpp

$(OBJDIR)/Application.o: $(SRCDIR)/Application.cpp $(SRCDIR)/Application.h
	c++ $(FLAGS) -o $(OBJDIR)/Application.o $(SRCDIR)/Application.cpp

clean:
	rm -f $(OBJ) $(PROJECT_NAME)


