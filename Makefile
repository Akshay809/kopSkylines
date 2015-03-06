IDIR = include
LDIR = lib
ODIR = build
SDIR = src
TARGET = bin/final_prog

CC = g++
CFLAGS = -I $(IDIR)

LIB = 
VPATH = src:lib

HDRS = $(shell find include -type f -name *.h)
SRCS = $(shell find lib src -type f -name *.cpp)
OBJS = $(patsubst %, $(ODIR)/%.o, $(basename $(notdir $(SRCS))))

$(ODIR)/%.o: %.cpp $(HDRS)
	@mkdir -p $(ODIR)
	@echo " $(CC) -c $< $(CFLAGS) -o $@"; $(CC) -c $< $(CFLAGS) -o $@

$(TARGET): $(OBJS)
	@mkdir -p bin
	@echo " Linking ... ";
	@echo " $(CC) $^ -o $@ $(LIB)"; $(CC) $^ -o $@ $(LIB)

clean:
	@echo " Cleaaning ... ";
	@echo " $(RM) -r $(ODIR) $(TARGET)"; $(RM) -r $(ODIR) $(TARGET)

.PHONY: clean