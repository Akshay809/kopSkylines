INCLUDE = include
LIB = lib
BUILD = build
BIN = bin
DATA = data
TEST = test

CC = g++
CFLAGS = -I $(INCLUDE)

SRCS = $(shell find $(LIB) -type f -name *.cpp)
OBJS = $(patsubst %, $(BUILD)/%.o, $(basename $(notdir $(SRCS))))

TSTSRCS = $(shell find $(TEST) -type f -name *.cpp)
TSTEXCS = $(basename $(TSTSRCS))

DATSRCS = $(shell find $(DATA) -type f -name *.cpp)
DATEXCS = $(basename $(DATSRCS))

all: compile config data

# ******************************************************** #
#  Building a dependency tree of entire project directory  #
# ******************************************************** #

depend: .depend

.depend: $(SRCS) .tmp
	@rm -f ./.depend
	@$(CC) -std=c++11 $(CFLAGS) -MM $^ > ./.depend;
	@sed -i 's/^[A-Za-z0-9][A-Za-z0-9_]*\.o\:/$(BUILD)\/&/' .depend
	@cat .tmp >> .depend

.tmp: $(TSTSRCS)
	@rm -f ./.tmp
	@$(CC) -std=c++11 $(CFLAGS) -MM $^ > ./.tmp;
	@sed -i 's/^[A-Za-z0-9][A-Za-z0-9_]*\.o\:/$(BUILD)\/&/' .tmp
	@sed -i 's/\.o\:/\:/' .tmp

-include .depend

# ******************************************************** #
#                Compiling all source files                #
# ******************************************************** #

compile: $(OBJS)

$(BUILD)/%.o: $(LIB)/%.cpp
	@mkdir -p $(BUILD)
	@echo " Compiling $<...";
	@$(CC) -std=c++11 -c $< $(CFLAGS) -o $@

# ******************************************************** #
#                      Building tests                      #
# ******************************************************** #

tests: $(TSTEXCS)

$(TEST)/%: $(TEST)/%.cpp compile Skyline.config
	@mkdir -p $(TEST)/result
	@echo " Building test $(@F)..."
	@$(CC) -std=c++11 $(CFLAGS) $< $(shell cat Skyline.config) -o $@

# ******************************************************** #
#           Auto generating config file template           #
# ******************************************************** #

config: $(OBJS)
	@echo " Generating Skyline.config..."
	@rm -f Skyline.config
	@echo $(OBJS) > Skyline.config
	@sed -i 's/[ ]/'\\n'/g' Skyline.config

# ******************************************************** #
#                 Building Data Generators                 #
# ******************************************************** #

data: $(DATEXCS)

$(DATA)/%: $(DATA)/%.cpp
	@echo " Generating data generator $(@F)..."
	@$(CC) -std=c++11 $(CFLAGS) $< -o $@

# ******************************************************** #

clean:
	@echo " Cleaning ... ";
	@echo " rm -rf $(BUILD)";rm -rf $(BUILD)
	@echo " rm -f Skyline.config";rm -f Skyline.config
	@echo " rm -f $(TSTEXCS)";rm -f $(TSTEXCS)
	@echo " rm -f $(DATEXCS)";rm -f $(DATEXCS)

.PHONY: clean data compile tests config all
