all:
	g++ -ansi -pedantic -Wno-deprecated *.cpp *.cc -DGL_GLEXT_PROTOTYPES -lglut -lGL -lGLU -o a4

osx:
	g++ -ansi -pedantic -Wno-deprecated *.cpp *.cc -lglew -LGLFW -lglfw -framework Cocoa -framework IOKit -framework OpenGL -Llibnoise -lnoise -Iinclude/ -std=c++11

clean:
	rm -f *~ *.o a.out
