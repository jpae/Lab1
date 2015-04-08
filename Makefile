FILES=bend camera gameobject component GLSL main renderer tiny_obj_loader world
OBJECTS=${FILES:=.o}

CC=g++
CFLAGS=-ansi -pedantic -Wno-deprecated
FRAMEWORKS=-framework Cocoa -framework IOKit -framework OpenGL
LIBRARIES=-lglew -lglfw -lnoise
INCLUDE=-Iinclude

all: osx


linux:
	g++ -ansi -pedantic -Wno-deprecated *.cpp *.cc -DGL_GLEXT_PROTOTYPES -lglut -lGL -lGLU -o a4

osx: ${OBJECTS}
	g++ -pedantic -Wno-deprecated ${OBJECTS} ${FRAMEWORKS} -L. ${LIBRARIES}

%.o: %.cpp %.h
	$(CC) -c $(CFLAGS) $< -o $@ ${INCLUDE}

clean:
	rm -f *~ *.o a.out
