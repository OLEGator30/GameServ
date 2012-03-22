#ifndef OtherMod_H
#define OtherMod_H

extern const float BuySell[5][4];

#include "GameMod.h"

int strtoint(char*);
void addfdpl(struct fdstr*,fd_set*,int*);
void addfd (struct fdstr**,int);
void addprod(struct produce**,struct fdstr*);
void freebuildfact(struct build*);
void delclient(int,struct fdstr**);
int getnumber(int,struct fdstr*);
int getcom(int,char*,struct fdstr**);
int count(struct fdstr*);
int devide(char*,int*,int*);
int mystrcmp(char *,const char*);
void givenumbers(struct fdstr*);
void doline(struct fdstr*);
void endofturn(int,struct fdstr*);
int getflag(int,struct fdstr*);
void print(char,int,int,int,struct fdstr*);
void freeapl(struct apl*);
void help(int);
int correctsell(int,int,int);
int correctbuy(int,int,int);

#endif
