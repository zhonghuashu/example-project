
LDFLAGS += 
INCLUDE += -I.

OBJS = hello.o \
helloprint.o

BIN = hello-make

all: $(BIN)

$(BIN): $(OBJS)
	 $(CC) $(CFLAGS) -o $@ $(OBJS) $(LFLAGS)

# .c.o = %.o : %.c, %< = first Prerequisite, $@ = first target 
%.o: %.c
	 $(CC) $(INCLUDE) $(CFLAGS) -c $< -o $@
clean:
	rm -rf $(BIN) *.o