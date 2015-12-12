PROJECT_NAME = Phantom-Drive

SRCDIR = $(CURDIR)/src
OBJDIR = $(CURDIR)/obj
ASSDIR = $(CURDIR)/ass
BINDIR = $(CURDIR)/bin

OBJ = $(addprefix $(OBJDIR)/, main.o Application.o RenderSystem.o \
                              Actor.o RenderModel.o RenderModel2D.o \
                              Camera.o DummyActor.o DummyActor2.o Logger.o \
															Scene.o PhysicsSimulator.o ActorMotionState.o \
															SimulatedPhysicsActor.o RenderModel3D.o \
															tiny_obj_loader.o Shader.o)

ifeq ($(CXX), clang++)
	CXXFLAGS_CLANG = -Wdeprecated
	CXXFLAGS_CLANG += -Wdocumentation -Werror=documentation
endif

CXXFLAGS = -std=c++11 -Wall -Wextra $(CXXFLAGS_CLANG) -c -g
LDFLAGS = -g

# Certain library names and flags depend on the OS
ifeq ($(OS), Windows_NT)
	LDLIBS = -lmingw32 -lopengl32 -lglu32 -lglew32 -llibphysfs -lSDL2main
	LDFLAGS += -mwindows
	EXE_NAME = $(PROJECT_NAME).exe
else
	LDLIBS = -lGL -lGLU -lGLEW -lphysfs -lSDLmain
	EXE_NAME = $(PROJECT_NAME)
endif
LDLIBS += -lSDL2 -lBulletDynamics -lBulletCollision -lLinearMath

	
all: $(OBJ)
	$(CXX) $(LDFLAGS) -o $(EXE_NAME) $(OBJ) $(LDLIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ $<
	
$(OBJDIR)/%.o: $(SRCDIR)/%.cc
	$(CXX) $(CXXFLAGS) -o $@ $<

$(OBJ): | $(OBJDIR)

$(OBJDIR):
	mkdir $(OBJDIR)

.PHONY: clean
clean:
	rm -rf $(OBJDIR) $(EXE_NAME)

