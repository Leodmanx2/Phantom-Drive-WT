TARGET = Phantom-Drive

SRCDIR = $(CURDIR)/src
OBJDIR = $(CURDIR)/obj
BINDIR = $(CURDIR)/bin

SOURCES := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(SOURCES:$(SRCDIR)/%.cpp=$(OBJDIR)/%.o)

CXXFLAGS = -std=c++17

CXX_WARNINGS = -pedantic -Wall -Wextra -Wcast-align -Wcast-qual \
               -Wctor-dtor-privacy -Wdisabled-optimization -Wformat=2 \
               -Winit-self -Wmissing-declarations -Wmissing-include-dirs \
               -Wold-style-cast -Woverloaded-virtual -Wredundant-decls \
               -Wshadow -Wstrict-overflow=5 -Wswitch-default -Wundef \
               -Weffc++ -Winline -Wswitch-enum

ifeq ($(CXX), clang++)
	CXXFLAGS_WARNINGS += -Wdeprecated -Wdocumentation -Werror=documentation
endif

ifeq ($(OS), Windows_NT)
	LDLIBS = -lopengl32 -lglu32
else
	LDLIBS = -lGL -lGLU
endif
LDLIBS += -lglfw3 -lglbinding -lglobjects

all: CXXFLAGS += -O2 -march=native -finline-functions -flto
all: LDFLAGS += -s -flto
all: $(BINDIR)/$(TARGET)

debug: CXXFLAGS += -D DEBUG -g $(CXX_WARNINGS)
debug: $(BINDIR)/$(TARGET)

$(BINDIR)/$(TARGET): $(OBJECTS) | $(BINDIR)
	--@printf "[ ] Linking $(BINDIR)/$(TARGET)"
	@$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ $(OBJECTS) $(LDLIBS)
	--@printf "\r[✓] Linking $(TARGET)\n"

$(BINDIR):
	-@printf "[ ] Making binary directory"
	@mkdir -p $(BINDIR)
	-@printf "\r[✓] Making binary directory\n"

$(OBJECTS): $(OBJDIR)/%.o : $(SRCDIR)/%.cpp | $(OBJDIR)
	-@printf "[ ] Compiling $<"
	@$(CXX) $(CXXFLAGS) -c $< -o $@
	-@printf "\r[✓] Compiling $<\n"

$(OBJDIR):
	-@printf "[ ] Making object code directory"
	@mkdir -p $(OBJDIR)
	-@printf "\r[✓] Making object code directory\n"

.PHONY: clean
clean:
	@rm -f $(OBJECTS) $(BINDIR)/$(TARGET)
