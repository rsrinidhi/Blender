#
#	Makefile for OpenGL example from Chapter 1 in Red Book
#

CC = g++ 
CFLAGS = -D_DEBUG
CPPFLAGS = -g

.cpp.o:
	$(CC) -c $(CFLAGS) $(CPPFLAGS) -o $@  $<

LIBS = -framework OpenGL -framework GLUT -lglew -lglut

SRCS = program2.cpp LoadShaders.cpp
OBJS = program2.o LoadShaders.o

example1: $(OBJS)
	g++ -g -o program2 $(OBJS) $(LIBS)

clean:
	rm -f program2 *.o
