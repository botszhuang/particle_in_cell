PROJECT_NAME := /home/botsz/code/ghost_at_particle_in_cell
CUDA_INC     := /usr/local/cuda/include
CUDA_LIB     := /usr/local/cuda/lib
CUDA_LIB64   := /usr/local/cuda/lib64

CC := gcc

# Path Definitions
SRC_PATH     := $(PROJECT_NAME)/0_src
EXAMPLE_PATH := $(PROJECT_NAME)/1_example

C_DIR   := $(SRC_PATH)
CL_DIR  := $(SRC_PATH)
OBJ_DIR := $(EXAMPLE_PATH)/o
EXE_DIR := $(EXAMPLE_PATH)/exe

# File Discovery
EXE_FILE := $(EXE_DIR)/main.exe
C_FILES  := $(shell find $(C_DIR) -name "*.c")
CL_FILES := $(shell find $(CL_DIR) -name "*.cl")

# Generate Object list: maps 0_src/c/file.c to 1_example/o/file.o
O_FILES  := $(patsubst $(C_DIR)/%.c,$(OBJ_DIR)/%.o,$(C_FILES))

# Generate Header list for CL files
CL_HEADERS := $(patsubst $(CL_DIR)/%.cl,$(OBJ_DIR)/%_cl.h,$(CL_FILES))

# Include all subdirectories under C_DIR
INC_DIRS := $(shell find $(C_DIR) -type d)
CC_INC   := $(addprefix -I,$(INC_DIRS)) -I$(OBJ_DIR)

CCFLAGS  := -I$(CUDA_INC) $(CC_INC) -g -DDEBUG
LDFLAGS  := -L$(CUDA_LIB) -L$(CUDA_LIB64) -lOpenCL -lm


######################################

all: build

build: $(CL_HEADERS) $(EXE_FILE)

# Link executable
$(EXE_FILE): $(O_FILES) | $(EXE_DIR)
	$(CC) -o $@ $^ $(CCFLAGS) $(LDFLAGS)

# Compile C source .c -> obj/*.o
$(OBJ_DIR)/%.o: $(C_DIR)/%.c $(CL_HEADERS) | $(OBJ_DIR)
	@mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) -c $< -o $@

# Convert kernel.cl -> kernel_cl.h in the object folder
$(OBJ_DIR)/%_cl.h: $(CL_DIR)/%.cl | $(OBJ_DIR)
	@echo "Baking kernel: $(notdir $<)"
	@cd $(CL_DIR) && xxd -i $(notdir $<) > $(abspath $@)

$(OBJ_DIR) $(EXE_DIR):
	mkdir -p $@

.PHONY:run clean

run: build
#	valgrind --leak-check=full --show-leak-kinds=all --verbose $(EXE_FILE)
#	valgrind $(EXE_FILE)
	$(EXE_FILE)

clean:
	rm -rf $(OBJ_DIR) $(EXE_DIR)
