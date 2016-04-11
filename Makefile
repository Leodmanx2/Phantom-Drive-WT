################################################################################
# Progress indicator by Giovanni Funchal with modifications 
# by phyatt and myself (https://stackoverflow.com/q/451413)
################################################################################

ifneq ($(words $(MAKECMDGOALS)),1)
.DEFAULT_GOAL = all
%:
	@$(MAKE) $@ --no-print-directory -rRf $(firstword $(MAKEFILE_LIST))
else
ifndef ECHO
T := $(shell $(MAKE) $(MAKECMDGOALS) --no-print-directory \
	-nrRf $(firstword $(MAKEFILE_LIST)) \
	ECHO="COUNTTHIS" | grep -c "COUNTTHIS")

N := x
C = $(words $N)$(eval N := x $N)
ECHO = echo "[`expr $C '*' 100 / $T`%]"
endif


################################################################################
# Configuration
################################################################################

PROJECT_NAME = Phantom-Drive

SRCDIR = $(CURDIR)/src
OBJDIR = $(CURDIR)/obj
ASSDIR = $(CURDIR)/ass
BINDIR = $(CURDIR)/bin

OBJ = $(addprefix $(OBJDIR)/, main.o Application.o RenderSystem.o \
                              Actor.o RenderModel.o \
                              Camera.o DummyActor.o Logger.o \
                              Scene.o PhysicsSimulator.o ActorMotionState.o \
                              SimulatedPhysicsActor.o \
                              tiny_obj_loader.o Shader.o Light.o)

ifeq ($(CXX), clang++)
	CXXFLAGS_CLANG = -Wdeprecated
	CXXFLAGS_CLANG += -Wdocumentation -Werror=documentation
endif

CXXFLAGS_WARNINGS = -pedantic -Wall -Wextra -Wcast-align -Wcast-qual \
                    -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 \
                    -Winit-self -Wmissing-declarations -Wmissing-include-dirs \
                    -Wold-style-cast -Woverloaded-virtual -Wredundant-decls \
                    -Wshadow -Wstrict-overflow=5 -Wswitch-default -Wundef \
                    -Weffc++ -Winline -Wswitch-enum

CXXFLAGS += -std=c++14 $(CXXFLAGS_CLANG) $(CXXFLAGS_WARNINGS) -c -g
LDFLAGS += -g

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


################################################################################
# Targets
################################################################################

all: $(OBJ)
	@$(ECHO) Linking $(EXE_NAME)
	@$(CXX) $(LDFLAGS) -o $(EXE_NAME) $(OBJ) $(LDLIBS)

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@$(ECHO) Compiling $<
	@$(CXX) $(CXXFLAGS) -o $@ $<

$(OBJDIR)/%.o: $(SRCDIR)/%.cc
	@$(ECHO) Compiling $<
	@$(CXX) $(CXXFLAGS) -o $@ $<

$(OBJ): | $(OBJDIR)

$(OBJDIR):
	@$(ECHO) Making object code directory
	@mkdir $(OBJDIR)

.PHONY: clean
clean:
	@$(ECHO) Removing object and binary files
	@rm -rf $(OBJDIR) $(EXE_NAME)


# Progress indicator end
endif
