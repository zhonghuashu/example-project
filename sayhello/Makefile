EXEC=sayhello
LDFLAGS += -lhello

all: $(EXEC)

$(EXEC): sayhello.c
	 $(CC) $< $(LDFLAGS) $(CFLAGS) -o $(EXEC)

clean:
	 rm -rf $(EXEC) *.o