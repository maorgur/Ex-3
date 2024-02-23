
$(CC) = gcc
$(flags) = -Wall -g

all: StrList
StrList: Main.c StrList.c StrList.h
	$(CC) $(flags) Main.c StrList.c -o Main

clean:
	rm StrList