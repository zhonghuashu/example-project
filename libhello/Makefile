LIB=libhello.so

all: $(LIB)

$(LIB): hellolib.o
	 $(CC) $< -Wl,-soname,$(LIB).1 -fPIC $(LDFLAGS) -shared -o $(LIB).1.0

%.o: %.c
	 $(CC) -c $<

clean:
	 rm -rf *.o *.so*