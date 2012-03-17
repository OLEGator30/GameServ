default: Prog

GameMod.o: GameMod.c GameMod.h
	gcc -Wall -g -c GameMod.c -o GameMod.o

MainMod.o: MainMod.c MainMod.h
	gcc -Wall -g -c MainMod.c -o MainMod.o

OtherMod.o: OtherMod.c OtherMod.h
	gcc -Wall -g -c OtherMod.c -o OtherMod.o

Prog: TechMod.c GameMod.o MainMod.o OtherMod.o
	gcc -Wall -g TechMod.c GameMod.o MainMod.o OtherMod.o -o Prog

clean:
	rm -f *.o Prog *~

