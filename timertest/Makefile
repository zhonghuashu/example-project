EXEC = timertest
LDFLAGS +=

all: $(EXEC)

$(EXEC): timer_test.c
	 $(CC) $< $(LDFLAGS) $(CFLAGS) -o $(EXEC)

clean:
	 rm -rf $(EXEC) *.o