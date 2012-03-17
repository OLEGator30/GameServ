#ifndef TechMod_H
#define TechMod_H

#include "GameMod.h"
#include "OtherMod.h"

void infowait(struct fdstr *fdlist,int i,int maxpl);
void waitdialog(struct fdstr **fdlist,fd_set readfds,int *i,int maxpl);
void waitpl(struct fdstr **fdlist,int ls,int maxpl);
void dolistensock(int *ls,int port);
void gameserv(int port,int maxpl);
int main(int argc,char **argv);

#endif
