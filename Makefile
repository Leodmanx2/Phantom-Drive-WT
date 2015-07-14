PROJECT_NAME = Phantom-Drive

SRCDIR = $(CURDIR)/src
OBJDIR = $(CURDIR)/obj
ASSDIR = $(CURDIR)/ass
BINDIR = $(CURDIR)/bin

OBJ = $(addprefix $(OBJDIR)/, main.o Application.o RenderSystem.o Window.o \
                              Actor.o RenderModel.o RenderModel2D.o \
                              RenderModel3D.o Camera.o DummyActor.o Logger.o)

CXXFLAGS = -std=c++0x -Wall -c -g
LDFLAGS = -g

# Certain library names and flags depend on the OS
ifeq ($(OS), Windows_NT)
	LDLIBS = -lmingw32 -lopengl32 -lglu32 -lglew32
	LDFLAGS += -mwindows
else
	LDLIBS = -lGL -lGLU -lGLEW
endif
LDLIBS += -lSDL2main -lSDL2 -lassimp -llibphysfs

	
all: $(OBJ)
	$(CXX) $(LDFLAGS) -o $(PROJECT_NAME).exe $(OBJ) $(LDLIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	c++ $(CXXFLAGS) -o $@ $<

$(OBJ): | $(OBJDIR)

$(OBJDIR):
	mkdir $(OBJDIR)

.PHONY: clean
clean:
	rm -rf $(OBJDIR) $(PROJECT_NAME)

