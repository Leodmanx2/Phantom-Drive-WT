PROJECT_NAME = Phantom-Drive

SRCDIR = $(CURDIR)/src
OBJDIR = $(CURDIR)/obj
ASSDIR = $(CURDIR)/ass
BINDIR = $(CURDIR)/bin

OBJ = $(addprefix $(OBJDIR)/, main.o Application.o RenderSystem.o Window.o \
                              Actor.o RenderModel.o RenderModel2D.o \
                              RenderModel3D.o Camera.o Storage.o DummyActor.o \
                              Logger.o)

FLAGS = -std=c++0x -Wall -c

# Some of the linked libraries are obviously only for compiling
# on windows. Remove 'em and things should be fine on other systems.
all: $(OBJ)
	c++ -o $(PROJECT_NAME).exe $(OBJ) -lmingw32 -lSDL2main -lSDL2 -mwindows \
	-lglu32 -lopengl32 -lglew32 -lassimp

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	c++ $(FLAGS) -o $@ $<

$(OBJ): | $(OBJDIR)

$(OBJDIR):
	mkdir $(OBJDIR)

.PHONY: clean
clean:
	rm -rf $(OBJDIR) $(PROJECT_NAME)

