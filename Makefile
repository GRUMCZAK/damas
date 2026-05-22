CC       = cc
WARNINGS = -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow -Wwrite-strings -Wcast-qual -Wswitch-enum -Wconversion -Wunreachable-code -Wformat=2
CFLAGS   = -Wall -Wextra $(WARNINGS) -g3 -O0
BUILDDIR = build
TARGET   = damas

SRCS = main.c $(wildcard src/*.c)
OBJS = $(SRCS:%.c=$(BUILDDIR)/%.o)

all: $(TARGET)

run: all
	./$(TARGET)

$(TARGET): $(OBJS)
	$(CC) -o $@ $^

$(BUILDDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILDDIR) $(TARGET)

.PHONY: all clean run
