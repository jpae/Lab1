all:
	g++ -ansi -pedantic -Wno-deprecated *.cpp *.cc -DGL_GLEXT_PROTOTYPES -lglut -lGL -lGLU -o a4

osx:
	g++ -ansi -pedantic -Wno-deprecated *.cpp *.cc -framework GLUT -framework OpenGL

clean:
	rm -f *~ *.o a.out
