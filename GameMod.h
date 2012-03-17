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

int active(struct fdstr *fdlist,int fd,char *com,struct apl **sell,
                           struct apl **buy,int mark,struct produce **prodlist);
int answer(struct fdstr *fdlist,int fd,char *com,int mark,struct apl **sell,
                                    struct apl **buy,struct produce **prodlist);
void chmark(int *mark);
void cost(struct fdstr *fdlist);
void bankrupt(struct fdstr **fdlist);
int commands(struct fdstr **fdlist,fd_set readfds,int *i,int mark,
                  struct apl **sell,struct apl **buy,struct produce **prodlist);
void recvcommands(struct fdstr **fdlist,int mark,struct apl **sell,
                                    struct apl **buy,struct produce **prodlist);
void game(struct fdstr *fdlist);

#endif
