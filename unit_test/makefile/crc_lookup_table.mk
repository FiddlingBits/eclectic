####################################################################################################
# FILE:	 crc_lookup_table.mk
# BRIEF: Unit Test Makefile
####################################################################################################

####################################################################################################
# Targets
####################################################################################################

BUILD_DIRECTORY := ../build
EXECUTABLE := crc_lookup_table.exe
LIBRARY := unit_test.a

####################################################################################################
# Defines
####################################################################################################

APPLICATION_DEFINES :=                    \
  -DCRC_CRC8_LOOKUP_TABLE_METHOD          \
  -DCRC_CRC8_CDMA2000_LOOKUP_TABLE_METHOD \
  -DCRC_CRC8_DARC_LOOKUP_TABLE_METHOD     \
  -DCRC_CRC8_DVB_S2_LOOKUP_TABLE_METHOD   \
  -DCRC_CRC8_EBU_LOOKUP_TABLE_METHOD

LIBRARY_DEFINES := 

####################################################################################################
# Include Paths
####################################################################################################

APPLICATION_INCLUDE_PATHS := \
  -I../../app/               \
  -I../../lib/unity/

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

APPLICATION_SOURCES := \
  ../main.c            \
  ../test/crc_test.c   \
  ../../app/crc.c

LIBRARY_SOURCES :=                \
  ../../lib/unity/unity.c         \
  ../../lib/unity/unity_fixture.c \
  ../../lib/unity/unity_memory.c

####################################################################################################
# Rules
####################################################################################################

all: $(BUILD_DIRECTORY)/$(LIBRARY)
	@echo "Building Executable: $(EXECUTABLE)"
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
