CC = g++
EXEC = toptest
LDFLAGS +=

all: $(EXEC)

$(EXEC): toptest.cpp
	 $(CC) $< $(LDFLAGS) $(CFLAGS) -o $(EXEC)

clean:
	 rm -rf $(EXEC) *.o