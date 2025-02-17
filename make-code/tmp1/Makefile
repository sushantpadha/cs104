objects = foo.o bar.o baz.o

# Implicitly link
all: $(objects)
	$(CC) $(LDFLAGS) $^ $(LOADLIBES) $(LDLIBS) -o $@



# Explicit rule for foo.o
foo.o: foo.c foo.h
	cc -c foo.c -o foo.o

# Pattern rule
%.h:
	touch $@

foo.c:
	echo "int main() { return 0; }" > $@

%.c:
	touch $@

# Pattern Rule + Implicit link
$(objects): %.o: %.c

clean:
	rm -f *.o *.c *.h all

