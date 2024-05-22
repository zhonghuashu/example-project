CC = gcc
RM = rm

CFLAGS = -c -Wall
LDFLAGS = 

INCLUEDIR = -I.
DESTDIR = 
BINDIR = /usr/bin

OBJECTS = hello.o \
helloprint.o

EXECUTABLE = hello-make

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	 $(CC) $(LFLAGS) -o $@ $(OBJECTS)

# .c.o = %.o : %.c, %< = first Prerequisite, $@ = first target 
%.o: %.c
	 $(CC) $(INCLUDE) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(EXECUTABLE) *.o

install: $(EXECUTABLE)
	mkdir -p ${DESTDIR}/${BINDIR}
	install -m 0755 $< ${DESTDIR}/${BINDIR}