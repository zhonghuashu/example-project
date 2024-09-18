EXEC = hello-cpp
LDFLAGS +=

all: $(EXEC)

$(EXEC): hello.cpp
	 $(CXX) $< $(LDFLAGS) $(CFLAGS) -o $(EXEC)

clean:
	 rm -rf $(EXEC) *.o

install: $(EXEC)
	mkdir -p ${DESTDIR}/${BINDIR}
	install -m 0755 $< ${DESTDIR}/${BINDIR}