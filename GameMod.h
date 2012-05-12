#ifndef GameMod_H
#define GameMod_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/socket.h>
#include <string.h>
#include <time.h>
#include <netinet/in.h>
#include <sys/wait.h>

struct build
{
  int time;
  struct build *next;
};

struct num
{
  int kol;
  struct build *buildfact;
};

struct fdstr
{
  int fd;
  int number;
  long money;
  int raw;
  int product;
  int factory;
  struct num *buildnum;
  int flag; /* 0-new month; 1-enter 'turn' */
  char *buf;
  int bufwght;
  struct fdstr *next;
};

struct apl
{
  struct fdstr *pl;
  int kol;
  int price;
  struct apl *next;
};

struct produce
{
  struct fdstr *pl;
  int kol;
  struct produce *next;
};

#include "MainMod.h"

int active(struct fdstr*,int,char*,struct apl**,struct apl**,int,
																												struct produce**);
int answer(struct fdstr*,int,char*,int,struct apl**,struct apl**,
																										struct produce**,int);
void chmark(int*);
void cost(struct fdstr*);
void bankrupt(struct fdstr**);
int commands(struct fdstr**,fd_set,int*,int,struct apl**,struct apl**,
																										struct produce**,int);
void recvcommands(struct fdstr**,int,struct apl**,struct apl**,
																										struct produce**,int);
void game(struct fdstr*,int);

#endif
