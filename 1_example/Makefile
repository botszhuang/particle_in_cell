PROJECT_NAME := ..
# Ensure these paths exist on your system
CUDA_INC     := /usr/local/cuda/include
CUDA_LIB     := /usr/local/cuda/lib64

CC := gcc

# Path Definitions
SRC_PATH     := $(PROJECT_NAME)/0_src
EXAMPLE_PATH := $(PROJECT_NAME)/1_example

OBJ_DIR := $(EXAMPLE_PATH)/o
EXE_DIR := $(EXAMPLE_PATH)/exe

# File Discovery
EXE_FILE := $(EXE_DIR)/main.exe
C_FILES  := $(shell find $(SRC_PATH) -name "*.c")
CL_FILES := $(shell find $(SRC_PATH)/cl_kernel -name "*.cl")

O_FILES  := $(patsubst $(SRC_PATH)/%.c,$(OBJ_DIR)/%.o,$(C_FILES))

CL_HEADERS := $(foreach f,$(CL_FILES),$(OBJ_DIR)/$(notdir $(patsubst %/,%,$(dir $(f))))_cl.h)

ALL_SRC_DIRS := $(shell find $(SRC_PATH) -type d)
CC_INC := $(addprefix -I,$(ALL_SRC_DIRS)) -I$(OBJ_DIR)

# Ensure CCFLAGS uses this CC_INC
CCFLAGS  := -I$(CUDA_INC) $(CC_INC) -g -Wall -DDEBUG
LDFLAGS  := -L$(CUDA_LIB) -lOpenCL -lm

######################################

all: $(EXE_FILE)

# Link executable
$(EXE_FILE): $(O_FILES)
	@mkdir -p $(EXE_DIR)
	$(CC) -o $@ $^ $(LDFLAGS)

# Compile C source: % matches the relative path from SRC_PATH
$(OBJ_DIR)/%.o: $(SRC_PATH)/%.c $(CL_HEADERS)
	@mkdir -p $(dir $@)
	$(CC) $(CCFLAGS) -c $< -o $@

define BAKE_KERNEL
$(OBJ_DIR)/$(notdir $(patsubst %/,%,$(dir $(1))))_cl.h: $(1)
	@mkdir -p $$(dir $$@)
	@echo "Baking $$(notdir $(1)) from $$(notdir $$(patsubst %/,%,$$(dir $(1)))) into $$@"
	cd $$(dir $(1)) && xxd -i $$(notdir $(1)) > $$(abspath $$@)
	sed -i 's/kernel_cl/$(notdir $(patsubst %/,%,$(dir $(1))))_cl/g' $$@
endef

$(foreach f,$(CL_FILES),$(eval $(call BAKE_KERNEL,$(f))))

.PHONY: run clean

run: all
#	valgrind --leak-check=full --show-leak-kinds=all --verbose $(EXE_FILE)
#	valgrind $(EXE_FILE)
	$(EXE_FILE)   $(PROJECT_NAME)/1_example/input/input.tex
clean:
	rm -rf $(OBJ_DIR) $(EXE_DIR)
