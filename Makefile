all:
	g++ -ansi -pedantic -Wno-deprecated *.cpp *.cc -DGL_GLEXT_PROTOTYPES -lglut -lGL -lGLU -o a4

osx:
	g++ -ansi -pedantic -Wno-deprecated *.cpp *.cc -framework Cocoa -framework IOKit -framework OpenGL -L. -lglew -lglfw -lnoise -Iinclude/ -std=c++11

clean:
	rm -f *~ *.o a.out
