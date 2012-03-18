#ifndef TechMod_H
#define TechMod_H

#include "GameMod.h"
#include "OtherMod.h"

void infowait(struct fdstr*,int,int);
void waitdialog(struct fdstr**,fd_set,int*,int);
void waitpl(struct fdstr**,int,int);
void dolistensock(int*,int);
void gameserv(int,int);
int main(int,char**);

#endif
