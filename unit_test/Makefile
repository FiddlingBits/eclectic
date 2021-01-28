####################################################################################################
# FILE:		Makefile
# BRIEF:	Unit Test Makefile
####################################################################################################

####################################################################################################
# Targets
####################################################################################################

BUILD_DIRECTORY := ./build
EXECUTABLE := unit_test.exe
LIBRARY := unit_test.a

####################################################################################################
# Defines
####################################################################################################

APPLICATION_DEFINES :=                                                 \
  -DRANDOM_RANDOMIZATION_SEED_FILE_NAME='"./build/randomization_seed"'

LIBRARY_DEFINES := 

####################################################################################################
# Include Paths
####################################################################################################

APPLICATION_INCLUDE_PATHS := \
  -I../app/                  \
  -I../lib/unity/

LIBRARY_INCLUDE_PATHS := \
  -I../lib/unity/

####################################################################################################
# Compiler Flags
####################################################################################################

APPLICATION_COMPILER_FLAGS := \
  -Weverything

LIBRARY_COMPILER_FLAGS := \
  -Wno-everything

####################################################################################################
# Sources
####################################################################################################

APPLICATION_SOURCES :=   \
  ./main.c               \
  ./test/template_test.c \
  ../app/random.c

LIBRARY_SOURCES :=             \
  ../lib/unity/unity.c         \
  ../lib/unity/unity_fixture.c \
  ../lib/unity/unity_memory.c

####################################################################################################
# Rules
####################################################################################################

all: $(BUILD_DIRECTORY)/$(LIBRARY)
	@clang -c $(APPLICATION_DEFINES) $(APPLICATION_COMPILER_FLAGS) $(APPLICATION_INCLUDE_PATHS) $(APPLICATION_SOURCES)
	@clang *.o -o $(EXECUTABLE) $(BUILD_DIRECTORY)/$(LIBRARY)
	@mv $(EXECUTABLE) $(BUILD_DIRECTORY)
	@rm -f *.o
	@./$(BUILD_DIRECTORY)/$(EXECUTABLE)

$(BUILD_DIRECTORY)/$(LIBRARY): $(BUILD_DIRECTORY)
	@clang -c $(LIBRARY_DEFINES) $(LIBRARY_COMPILER_FLAGS) $(LIBRARY_INCLUDE_PATHS) $(LIBRARY_SOURCES)
	@ar rcs $(LIBRARY) *.o
	@mv $(LIBRARY) $(BUILD_DIRECTORY)
	@rm -f *.o

$(BUILD_DIRECTORY):
	@mkdir $(BUILD_DIRECTORY)

clean: FORCE
	@rm -f *.exe.stackdump
	@rm -f *.o
	@rm -fr $(BUILD_DIRECTORY)

test: FORCE
	@printf "Running 10000 times...\n"
	@printf "  Failures: "
	@./$(BUILD_DIRECTORY)/$(EXECUTABLE) -r 10000 | grep -c '[^0] Failures' || true

FORCE:

