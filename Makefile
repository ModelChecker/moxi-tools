CC := clang
GPERF := gperf
PYTHON := python3

TOOLS_DIR := tools
SRC_DIR := src
BUILD_DIR := build
OBJ_DIR := build/obj
BIN := moxisc

DBGFLAGS := -g -O0 -DDEBUG -Waggregate-return -Wbad-function-cast \
	 -Wcast-align -Wconversion -Wdisabled-optimization -Wdouble-promotion -Wfloat-equal -Wformat-nonliteral \
	  -Wformat-security -Wformat=2 -Wimplicit -Wlong-long -Wmisleading-indentation  -Wmissing-include-dirs \
	  -Wpacked -Wpedantic -Wshadow -Wsign-conversion  -Wswitch-default -Wundef -Wunreachable-code -Wunused \
	  -Wvariadic-macros -Wwrite-strings -Wall -Wextra -pedantic  -Wpointer-arith -Wcast-qual  \
	  -Wno-deprecated-non-prototype -Wno-missing-field-initializers -Wno-gnu-zero-variadic-macro-arguments
DBGLDFLAGS := -fsanitize=address -fsanitize=undefined

CFLAGS := -Wall -I$(SRC_DIR) -std=c99 -Wno-deprecated-non-prototype -fPIC

LIB := -lyices

# Select dynamic library suffix and flags per platform
UNAME_S := $(shell uname -s)
ifeq ($(UNAME_S),Darwin)
LIBRARY := libmoxiparser.dylib
LIB_LDFLAGS := -dynamiclib -install_name @rpath/libmoxiparser.dylib
else
LIBRARY := libmoxiparser.so
LIB_LDFLAGS := -shared
endif

# Token hashing
gperf_generated := $(SRC_DIR)/parse/hash_token.h \
				   $(SRC_DIR)/parse/hash_symbol.h \
				   $(SRC_DIR)/moxi/hash_logic.h

# Parser generation
python_generated := $(SRC_DIR)/parse/parse.c

SRC_DIRS := \
	$(SRC_DIR) \
	$(SRC_DIR)/parse \
	$(SRC_DIR)/util \
	$(SRC_DIR)/moxi \
	$(SRC_DIR)/io \
	$(SRC_DIR)/api
INC_DIRS := $(SRC_DIRS)
OBJ_DIRS := $(OBJ_DIR) $(addprefix $(OBJ_DIR)/, $(subst $(SRC_DIR),,$(SRC_DIRS)))

ifneq ("$(wildcard $(python_generated))","")
SRC_FILES := $(foreach x, $(SRC_DIRS), $(wildcard $(addprefix $(x)/*,.c*)))
else
SRC_FILES := $(foreach x, $(SRC_DIRS), $(wildcard $(addprefix $(x)/*,.c*))) $(python_generated)
endif

# Exclude main.c from library build
LIBRARY_SRC_FILES := $(filter-out $(SRC_DIR)/main.c, $(SRC_FILES))

OBJ_FILES := $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(basename $(subst $(SRC_DIR)/,,$(SRC_FILES)))))
LIBRARY_OBJ_FILES := $(addprefix $(OBJ_DIR)/, $(addsuffix .o, $(basename $(subst $(SRC_DIR)/,,$(LIBRARY_SRC_FILES)))))
DEP_FILES := $(addprefix $(OBJ_DIR)/, $(addsuffix .d, $(basename $(subst $(SRC_DIR)/,,$(SRC_FILES)))))

# Define default rule first
default: $(BUILD_DIR)/$(BIN) $(BUILD_DIR)/$(LIBRARY) $(BUILD_DIR)/moxiparser.h

$(SRC_DIR)/parse/hash_token.h: $(SRC_DIR)/parse/token.gperf
	$(GPERF) -C -E -t --output-file=$(SRC_DIR)/parse/hash_token.h \
		--lookup-function-name=find_moxi_tok \
		--hash-function=hash_tok $(SRC_DIR)/parse/token.gperf

$(SRC_DIR)/parse/hash_symbol.h: $(SRC_DIR)/parse/symbol.gperf
	$(GPERF) -C -E -t --output-file=$(SRC_DIR)/parse/hash_symbol.h \
		--lookup-function-name=find_moxi_thy_sym \
		--hash-function=hash_sym $(SRC_DIR)/parse/symbol.gperf

$(SRC_DIR)/moxi/hash_logic.h: $(SRC_DIR)/moxi/logic.gperf
	$(GPERF) -C -E -t --output-file=$(SRC_DIR)/moxi/hash_logic.h \
	 --lookup-function-name=find_moxi_logic \
		--hash-function=hash_logic $(SRC_DIR)/moxi/logic.gperf

$(SRC_DIR)/parse/parse.c: $(TOOLS_DIR)/moxi.tbl $(TOOLS_DIR)/parse_table.py
	$(PYTHON) $(TOOLS_DIR)/parse_table.py $(TOOLS_DIR)/moxi.tbl > $(SRC_DIR)/parse/parse.c

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/%.d: $(SRC_DIR)/%.c | $(OBJ_DIR)
	@set -e; rm -f $@; echo Building dependency file $@ ; \
		$(CC) -MM -MG -MT $*.o $(CFLAGS) $< > $@.$$$$ ; \
		sed 's,\($*\).o[ :]*,$(OBJ_DIR)/\1.o $@ : ,g' < $@.$$$$ > $@ ; \
		sed 's, \($*\).h, $(SRC_DIR)/\1.h,g' < $@.$$$$ > $@ ; \
		rm -f $@.$$$$

ifneq ($(filter clean,$(MAKECMDGOALS)),clean)
-include $(DEP_FILES)
endif

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

$(BUILD_DIR)/$(BIN): $(gperf_generated) $(OBJ_FILES) $(DEP_FILES) | $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $@ $(OBJ_FILES) $(LIB) $(LDFLAGS)

$(BUILD_DIR)/$(LIBRARY): $(gperf_generated) $(LIBRARY_OBJ_FILES) $(DEP_FILES) | $(BUILD_DIR)
	$(CC) $(LIB_LDFLAGS) -o $@ $(LIBRARY_OBJ_FILES) $(LIB) $(LDFLAGS)

$(BUILD_DIR)/moxiparser.h: $(SRC_DIR)/moxiparser.h | $(BUILD_DIR)
	cp $< $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIRS)

bin: $(BUILD_DIR)/$(BIN)

lib: $(BUILD_DIR)/$(LIBRARY)

debug: CFLAGS += $(DBGFLAGS)
debug: LDFLAGS += $(DBGLDFLAGS)
debug: $(BUILD_DIR)/$(BIN) $(BUILD_DIR)/$(LIBRARY) $(BUILD_DIR)/moxiparser.h

clean:
	rm -rf $(OBJ_DIR) $(BUILD_DIR) $(gperf_generated) $(python_generated)

install: $(BUILD_DIR)/$(BIN) $(BUILD_DIR)/$(LIBRARY) $(BUILD_DIR)/moxiparser.h
	@echo "Installing to $(BUILD_DIR)/"
	@echo "Binary: $(BUILD_DIR)/$(BIN)"
	@echo "Library: $(BUILD_DIR)/$(LIBRARY)"
	@echo "Header: $(BUILD_DIR)/moxiparser.h"
