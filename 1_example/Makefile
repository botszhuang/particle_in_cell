CC          := gcc
EXE_DIR     := exe
EXE_FILE    := $(EXE_DIR)/main.exe

SRC_DIR  := c
OBJ_DIR  := o
SRCS     := $(shell find $(SRC_DIR) -name *.c )
OBJS     := $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRCS))
ccInc    := $(addprefix -I,./$(shell find $(SRC_DIR) -type d))

cl_DIR   := cl
cl_h_DIR := cl_h
cl_s     := $(shell find $(cl_DIR) -name *.cl )
#cl_h_s   := $(patsubst $(cl_DIR)/%.cl,$(cl_h_DIR)/%_cl.h,$(cl_s))
cl_h_s   := $(patsubst $(cl_DIR)/%.cl,$(cl_h_DIR)/%_cl.o,$(wildcard $(cl_DIR)/*.cl))
clInc    := -I./$(cl_h_DIR)


CUDA_INC := /usr/local/cuda/include

CCFLAGS := -I$(CUDA_INC) $(ccInc) $(clInc) -DDEBUG
LDFLAGS := -L/usr/local/cuda/lib -L/usr/local/cuda/lib64 -lOpenCL -lm

cl_common_obj := $(cl_h_DIR)/c_and_cl_common.o

################################################################################

# Target rules
all: build

build:$(cl_h_s) $(cl_common_obj) $(EXE_FILE)

$(OBJS): $(cl_h_s) $(cl_common_obj) 

# Link executable
$(EXE_FILE):$(OBJS) $(cl_h_s) $(cl_common_obj) | $(EXE_DIR)
	$(CC) -o $@ $(OBJS) $(cl_h_s) $(cl_common_obj) $(CCFLAGS) $(LDFLAGS)

# Compile C source .c -> obj/*.o
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c |$(OBJ_DIR)
	$(CC) -c $< -o $@ $(CCFLAGS)

# Embed kernels: .cl -> _cl.h
#$(cl_h_DIR)/%_cl.h: $(cl_DIR)/%.cl | $(cl_h_DIR)
#	xxd -i -n $* $< > $@
# Embed kernels: .cl -> _cl.o
#$(cl_h_DIR)/%_cl.o: $(cl_DIR)/%.cl | $(cl_h_DIR)
#	ld -r -b binary -o $@ $<
#	cat $< <(echo -ne '\0') > $@

$(cl_h_DIR)/%_cl.o: $(cl_DIR)/%.cl | $(cl_h_DIR)
	TEMP_NULL=$@.temp.null; \
	cp $< $${TEMP_NULL}; \
	printf '\0' >> $${TEMP_NULL}; \
	ld -r -b binary -o $@ $${TEMP_NULL}; \
	rm $${TEMP_NULL}

	

$(cl_h_DIR)/c_and_cl_common.o: c/c_and_cl_common.h | $(cl_h_DIR)
	TEMP_NULL=$@.temp.null; \
	cp $< $${TEMP_NULL}; \
	printf '\0' >> $${TEMP_NULL}; \
	ld -r -b binary -o $@ $${TEMP_NULL}; \
	rm $${TEMP_NULL}

$(OBJ_DIR):
	mkdir -p $@

$(cl_h_DIR):
	mkdir -p $@

$(EXE_DIR):
	mkdir -p $@

.PHONY:run
run: build
	./$(EXE_FILE)

.PHONY:clean
clean:
	rm -rf $(OBJ_DIR) $(cl_h_DIR) $(EXE_DIR)
