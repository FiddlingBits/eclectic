####################################################################################################
# FILE:	 get_random.mk
# BRIEF: Get Random Makefile
####################################################################################################

####################################################################################################
# Targets
####################################################################################################

BUILD_DIRECTORY := ../build
EXECUTABLE_CUSTOM_HELPER := get_random_custom_helper.exe
EXECUTABLE_DEFAULT_HELPER := get_random_default_helper.exe

####################################################################################################
# Include Paths
####################################################################################################

APPLICATION_INCLUDE_PATHS := \
  -I../../app/

####################################################################################################
# Compiler Flags
####################################################################################################

APPLICATION_COMPILER_FLAGS := \
  -Weverything

####################################################################################################
# Sources
####################################################################################################

APPLICATION_SOURCES := \
  ../get_random.c      \
  ../../app/random.c

####################################################################################################
# Rules
####################################################################################################

all: $(BUILD_DIRECTORY)
	@echo "Building Executable: $(EXECUTABLE_CUSTOM_HELPER)"
	@clang -o $(EXECUTABLE_CUSTOM_HELPER) $(APPLICATION_COMPILER_FLAGS) $(APPLICATION_INCLUDE_PATHS) $(APPLICATION_SOURCES)
	@mv $(EXECUTABLE_CUSTOM_HELPER) $(BUILD_DIRECTORY)
	@rm -f *.o
	@echo "Building Executable: $(EXECUTABLE_DEFAULT_HELPER)"
	@clang -o $(EXECUTABLE_DEFAULT_HELPER) -DRANDOM_RANDOMIZATION_SEED_FILE_NAME=\"../build/randomixzation_seed\" $(APPLICATION_COMPILER_FLAGS) $(APPLICATION_INCLUDE_PATHS) $(APPLICATION_SOURCES)
	@mv $(EXECUTABLE_DEFAULT_HELPER) $(BUILD_DIRECTORY)
	@rm -f *.o

$(BUILD_DIRECTORY):
	@mkdir $(BUILD_DIRECTORY)

clean: FORCE
	@rm -f *.exe.stackdump
	@rm -f *.o
	@rm -fr $(BUILD_DIRECTORY)

FORCE:
