#ifndef MainMod_H
#define MainMod_H

#include "OtherMod.h"

void newsell(struct apl **list,struct apl *p);
void newbuy(struct apl **list,struct apl *p);
void sortapl(struct apl **list,char c);
void lot(char c,int *max,struct apl *list,struct fdstr *fdlist);
void auction(struct apl *sell,struct apl *buy,int mark,
																											struct fdstr *fdlist);
void endofmonth(struct fdstr *fdlist,struct apl *sell,struct apl *buy,
                                        struct produce *prodlist,int mark);
void playerinfo(int number,struct fdstr *fdlist,int fd);
void newmonth(int *month,struct fdstr *fdlist);
void buildproc(int fd,struct fdstr *fdlist);
void addbuysell(int fd,struct fdstr *fdlist,int kol,int price,
																												struct apl **list);
void prodproc(struct fdstr *fdlist,int fd,int kol,
																								struct produce **prodlist);

#endif
