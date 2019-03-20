# Make environment
CC=gcc
CFLAGS=-I. -c -g -Wall
LINKARGS=-lm
LIBS=-lm

# Files
SOURCE_FILES=	asgn3.c \
				asgn3_support.c
OBJECT_FILES=	asgn3.o \
				asgn3_support.o

# Productions
all : asgn3

asgn3 : $(OBJECT_FILES)
	$(CC) $(LINKARGS) $(OBJECT_FILES) -o $@ $(LIBS)

clean : 
	rm -f cmsc257-s16-assign3 $(OBJECT_FILES)