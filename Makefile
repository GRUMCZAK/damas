CC       = cc
WARNINGS = -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow -Wwrite-strings -Wcast-qual -Wswitch-enum -Wconversion -Wunreachable-code -Wformat=2
CFLAGS   = -Wall -Wextra $(WARNINGS) -g3 -O0
BUILDDIR = build
TARGETS  = server client

SRCS = $(wildcard src/*.c)
OBJS = $(SRCS:%.c=$(BUILDDIR)/%.o)

all: $(TARGETS)

run-%: %
	./$<

$(TARGETS): %: $(BUILDDIR)/%.o $(OBJS)
	$(CC) -o $@ $^

$(BUILDDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILDDIR) $(TARGETS)

.PHONY: all clean
