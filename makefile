all:131044018

131044018:131044018.o
	g++ 131044018.o -o exe
131044018.o:131044018.cpp
	g++ -c -std=gnu++14 131044018.cpp

clean : 
	rm *.o exe

run:
	./exe
