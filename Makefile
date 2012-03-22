CC= gcc
CFLAGS= -g -Wall
SRCMODULES= OtherMod.c MainMod.c GameMod.c
OBJMODULES= $(SRCMODULES:.c=.o)

default: Prog

%.o: %.c %.h
	$(CC) $(CFLAGS) -c $< -o $@

Prog: TechMod.c $(OBJMODULES)
	$(CC) $(CFLAGS) $^ -o $@

ifneq (clean, $(MAKECMDGOALS))
-include deps.mk
endif
deps.mk: $(SRCMODULES)
	$(CC) -MM $^ > $@

clean:
	rm -f *.o Prog *~

