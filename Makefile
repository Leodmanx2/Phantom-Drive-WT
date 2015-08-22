PROJECT_NAME = Phantom-Drive

SRCDIR = $(CURDIR)/src
OBJDIR = $(CURDIR)/obj
ASSDIR = $(CURDIR)/ass
BINDIR = $(CURDIR)/bin

OBJ = $(addprefix $(OBJDIR)/, main.o Application.o RenderSystem.o Window.o \
                              Actor.o RenderModel.o RenderModel2D.o \
                              RenderModel3D.o Camera.o DummyActor.o Logger.o \
															Scene.o PhysicsModel.o PhysicsSystem.o)

CXXFLAGS = -std=c++0x -Wall -c -g
LDFLAGS = -g

# Certain library names and flags depend on the OS
ifeq ($(OS), Windows_NT)
	LDLIBS = -lmingw32 -lopengl32 -lglu32 -lglew32 -llibphysfs
	LDFLAGS += -mwindows
	EXE_NAME = $(PROJECT_NAME).exe
else
	LDLIBS = -lGL -lGLU -lGLEW -lphysfs
	EXE_NAME = $(PROJECT_NAME)
endif
LDLIBS += -lSDL2main -lSDL2 -lassimp -lBulletDynamics -lBulletCollision -lLinearMath

	
all: $(OBJ)
	$(CXX) $(LDFLAGS) -o $(EXE_NAME) $(OBJ) $(LDLIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OBJ): | $(OBJDIR)

$(OBJDIR):
	mkdir $(OBJDIR)

.PHONY: clean
clean:
	rm -rf $(OBJDIR) $(EXE_NAME)

