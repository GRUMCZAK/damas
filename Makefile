CC       = cc
WARNINGS = -Wall -Wextra -Wfloat-equal -Wundef -Wshadow -Wpointer-arith -Wcast-align -Wstrict-prototypes -Wstrict-overflow -Wwrite-strings -Wcast-qual -Wswitch-enum -Wconversion -Wunreachable-code -Wformat=2
CFLAGS   = -std=gnu23 $(WARNINGS) -g3 -O0
BUILDDIR = build
TARGETS  = server client

COMMON_SRCS = $(wildcard src/common/*.c)
COMMON_OBJS = $(COMMON_SRCS:%.c=$(BUILDDIR)/%.o)

SERVER_SRCS = server.c $(wildcard src/server/*.c)
SERVER_OBJS = $(SERVER_SRCS:%.c=$(BUILDDIR)/%.o)

CLIENT_SRCS = client.c $(wildcard src/client/*.c)
CLIENT_OBJS = $(CLIENT_SRCS:%.c=$(BUILDDIR)/%.o)

all: $(TARGETS)

run-%: %
	./$<

server: $(SERVER_OBJS) $(COMMON_OBJS)
	$(CC) -o $@ $^

client: $(CLIENT_OBJS) $(COMMON_OBJS)
	$(CC) -o $@ $^

$(BUILDDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BUILDDIR) $(TARGETS)

.PHONY: all clean
