EXEC = toptest
LDFLAGS +=

all: $(EXEC)

$(EXEC): toptest.c
	 $(CC) $< $(LDFLAGS) $(CFLAGS) -o $(EXEC)

clean:
	 rm -rf $(EXEC) *.o