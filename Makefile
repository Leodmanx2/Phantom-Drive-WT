PROJECT_NAME = Phantom-Drive

SRCDIR = $(CURDIR)/src
OBJDIR = $(CURDIR)/obj

OBJ = $(OBJDIR)/main.o $(OBJDIR)/Application.o $(OBJDIR)/RenderSystem.o \
      $(OBJDIR)/Window.o $(OBJDIR)/Actor.o $(OBJDIR)/RenderModel.o \
			$(OBJDIR)/RenderModel2D.o $(OBJDIR)/RenderModel3D.o

FLAGS = -std=c++0x -Wall -c

# Some of the linked libraries are obviously only for compiling
# on windows. Remove 'em and things should be fine on other systems.
all: $(OBJ)
	c++ -o $(PROJECT_NAME) $(OBJ) -lmingw32 -lSDL2main -lSDL2 -mwindows \
	-lglu32 -lopengl32 -lglew32 -lassimp

$(OBJDIR)/main.o: $(SRCDIR)/main.cpp
	c++ $(FLAGS) -o $(OBJDIR)/main.o $(SRCDIR)/main.cpp

$(OBJDIR)/Application.o: $(SRCDIR)/Application.cpp $(SRCDIR)/Application.h
	c++ $(FLAGS) -o $(OBJDIR)/Application.o $(SRCDIR)/Application.cpp

$(OBJDIR)/RenderSystem.o: $(SRCDIR)/RenderSystem.cpp $(SRCDIR)/RenderSystem.h
	c++ $(FLAGS) -o $(OBJDIR)/RenderSystem.o $(SRCDIR)/RenderSystem.cpp
	
$(OBJDIR)/Window.o: $(SRCDIR)/Window.cpp $(SRCDIR)/Window.h
	c++ $(FLAGS) -o $(OBJDIR)/Window.o $(SRCDIR)/Window.cpp
	
$(OBJDIR)/Actor.o: $(SRCDIR)/Actor.cpp $(SRCDIR)/Actor.h
	c++ $(FLAGS) -o $(OBJDIR)/Actor.o $(SRCDIR)/Actor.cpp
	
$(OBJDIR)/RenderModel.o: $(SRCDIR)/RenderModel.cpp $(SRCDIR)/RenderModel.h
	c++ $(FLAGS) -o $(OBJDIR)/RenderModel.o $(SRCDIR)/RenderModel.cpp
	
$(OBJDIR)/RenderModel2D.o: $(SRCDIR)/RenderModel2D.cpp $(SRCDIR)/RenderModel2D.h
	c++ $(FLAGS) -o $(OBJDIR)/RenderModel2D.o $(SRCDIR)/RenderModel2D.cpp
	
$(OBJDIR)/RenderModel3D.o: $(SRCDIR)/RenderModel3D.cpp $(SRCDIR)/RenderModel3D.h
	c++ $(FLAGS) -o $(OBJDIR)/RenderModel3D.o $(SRCDIR)/RenderModel3D.cpp

clean:
	rm -f $(OBJ) $(PROJECT_NAME)


