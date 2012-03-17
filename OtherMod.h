#ifndef OtherMod_H
#define OtherMod_H

extern const float BuySell[5][4];

#include "GameMod.h"

int strtoint(char *s);
void addfdpl(struct fdstr *fdlist,fd_set *readfds,int *max);
void addfd (struct fdstr **fdlist,int fd);
void addprod(struct produce **prodlist,struct fdstr *fdlist);
void freebuildfact(struct build *list);
void delclient(int fd,struct fdstr **fdlist);
int getnumber(int fd,struct fdstr *fdlist);
int getcom(int fd,char *str,struct fdstr **fdlist);
int count(struct fdstr *fdlist);
int devide(char *str,int *kol,int *price);
int mystrcmp(char *s1,char *s2);
void givenumbers(struct fdstr *fdlist);
void doline(struct fdstr *fdlist);
void endofturn(int fd,struct fdstr *fdlist);
int getflag(int fd,struct fdstr *fdlist);
void print(char c,int kol,int money,int number,struct fdstr *fdlist);
void freeapl(struct apl *list);
void help(int fd);
int correctsell(int fd,int price,int mark);
int correctbuy(int fd,int price,int mark);

#endif
