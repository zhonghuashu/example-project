EXEC = timertest
LDFLAGS +=

all: $(EXEC)

# $(EXEC): timer_test.c
# 	 $(CC) $< $(LDFLAGS) $(CFLAGS) -o $(EXEC)

$(EXEC): timer_test.cpp
	 $(CXX) $< $(LDFLAGS) $(CFLAGS) -o $(EXEC)

clean:
	 rm -rf $(EXEC) *.o