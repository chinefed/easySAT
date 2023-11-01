# Compiler and flags
CC := gcc
CFLAGS := -Wall -O3 -g -fsanitize=address

# Source and object files
SRCDIR := ./src
BUILDDIR := ./build
SOURCES := $(wildcard $(SRCDIR)/*.c)
COBJS := $(patsubst $(SRCDIR)/%.c,$(BUILDDIR)/%.o,$(SOURCES))

# Output binary
TARGET := $(BUILDDIR)/easySAT

.PHONY: all clean

all: $(TARGET)

clean:
	rm -f $(COBJS) $(TARGET)

$(TARGET): $(COBJS)
	$(CC) $(CFLAGS) -o $@ $^

$(BUILDDIR)/%.o: $(SRCDIR)/%.c | $(BUILDDIR)
	$(CC) $(CFLAGS) -c -o $@ $<

$(BUILDDIR):
	mkdir -p $(BUILDDIR)
