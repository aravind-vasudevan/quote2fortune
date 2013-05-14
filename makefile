HDR = incl/*.hpp
SRC = src/parser.cpp src/main.cpp

OBJ = ${SRC:.cpp=.o}
LINK = -lcurl
#INCL =
EXE = quote2fortune.exe 
CPP=g++ -g
CPPFLAGS=-O3 -g -Iincl/
STRIP = strip
.PHONY = clean export clean-all
TEMP = $(shell ls | grep -v "makefile")

all: ${EXE}

$(EXE): ${OBJ}
		${CPP} ${CPPFLAGS} ${OBJ} ${LINK} -o ${EXE}
		${STRIP} ${EXE}


parser.o: makefile parser.cpp parser.hpp
		${CPP} ${CPPFLAGS} -c parser.cpp -o parser.o

main.o: makefile main.cpp parser.o
		${CPP} ${CPPFLAGS} -c main.cpp -o main.o

# Make modifiers
clean:
		rm -rf ${OBJ} ${EXE}
		-rm ${TEMP} #'-' at the start of the line forces make to ignore errors from this command

clean-all:
		rm -rf ${OBJ} ${EXE} input/* output/*

export:
		rm -rf ${OBJ} ${EXE}
		tar --exclude=*git* --exclude=*input* --exclude=*output* -czvf quote2fortune.tar.gz ../quote2fortune/*
