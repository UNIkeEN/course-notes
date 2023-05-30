CC = cc
LDFLAGS = -lpthread
OBJS := $(subst .c,.o,$(wildcard */*.c))

.PHONY: all
all: $(OBJS)

%.o:%.c
	$(CC) -o $(basename $@) $< $(LDFLAGS)

.PHONY: clean
clean:
	rm $(basename $(OBJS))