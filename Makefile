CC = gcc
CFLAGS = -std=c99
OBJS = print_tree.o

print_tree: $(OBJS)
	$(CC) -o print_tree $(OBJS) -lgraph

clean:
	rm print_tree
