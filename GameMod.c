#include "GameMod.h"

int active(struct fdstr *fdlist,int fd,char *com,struct apl **sell,
                        struct apl **buy,int mark,struct produce **prodlist)
{
  int kol,price;

  if (getflag(fd,fdlist)==1)
  {
    write(fd,"Your vigorous activity this month was over\r\n\r\n> ",48);
    return 0;
  }
  if (mystrcmp(com,"prod ")==5)
  {
    com[strlen(com)-1]=0;
    if ((kol=strtoint(com+5))==-1) return (-1);
    else prodproc(fdlist,fd,kol,prodlist);
    return 0;
  }
  if (mystrcmp(com,"sell ")==5)
  {
    if (devide(com+5,&kol,&price)==-1) return (-1);
    if (correctsell(fd,price,mark)==-1) return 0;
    addbuysell(fd,fdlist,kol,price,sell);
    return 0;
  }
  if (mystrcmp(com,"buy ")==4)
  {
    if (devide(com+4,&kol,&price)==-1) return (-1);
    if (correctbuy(fd,price,mark)==-1) return 0;
    addbuysell(fd,fdlist,kol,price,buy);
    return 0;
  }
  if (strcmp("build\n",com)==0)
  {
    buildproc(fd,fdlist);
    return 0;
  }
  return (-1);
}

int answer(struct fdstr *fdlist,int fd,char *com,int mark,struct apl **sell,
                  struct apl **buy,struct produce **prodlist,int allplayers)
{
  if (strcmp("help\n",com)==0)
  {
    help(fd);
    return 0;
  }
  if (strcmp("market\n",com)==0)
  {
    char s[256];

    sprintf(s,"%d/%d players still active\r\n",count(fdlist),allplayers);
    sprintf(s+strlen(s),"Bank sells: items  min. price\r\n");
    sprintf(s+strlen(s),"%17.0f %11.0f\r\n",
                       BuySell[mark-1][0]*count(fdlist),BuySell[mark-1][1]);
    sprintf(s+strlen(s),"Bank buys:  items  max. price\r\n");
    sprintf(s+strlen(s),"%17.0f %11.0f\r\n\r\n> ",
                       BuySell[mark-1][2]*count(fdlist),BuySell[mark-1][3]);
    write(fd,s,strlen(s));
    return 0;
  }
  if (strcmp("turn\n",com)==0)
  {
    write(fd,"Your vigorous activity this month was over\r\n\r\n> ",48);
    if (getflag(fd,fdlist)==0)
    {
      endofturn(fd,fdlist);
      return 1;
    }
    return 0;
  }
  if (mystrcmp(com,"player ")==7)
  {
    int num;
    
    com[strlen(com)-1]=0;
    if ((num=strtoint(com+7))!=-1)
    {
      playerinfo(num,fdlist,fd);
      return 0;
    }
  }
  if (active(fdlist,fd,com,sell,buy,mark,prodlist)==0) return 0;
  write(fd,"Wrong command. Try 'help'\r\n\r\n> ",31);
  return 0;
}

void chmark(int *mark)
{
  const int level_change[5][5]=
  {
    { 4, 4, 2, 1, 1 },
    { 3, 4, 3, 1, 1 },
    { 1, 3, 4, 3, 1 },
    { 1, 1, 3, 4, 3 },
    { 1, 1, 2, 4, 4 }
  };
  int r,sum=0,i=0;

  r=1+(int)(12.0*rand()/(RAND_MAX+1.0));
  do
  {
    sum+=level_change[(*mark)-1][i];
    i++;
  } while (sum<r);
  (*mark)=i;
}

void cost(struct fdstr *fdlist)
{
  while (fdlist!=NULL)
  {
    (*fdlist).money-=((*fdlist).raw)*300;
    (*fdlist).money-=((*fdlist).product)*500;
    (*fdlist).money-=((*fdlist).factory)*1000;

    if ((*((*fdlist).buildnum)).kol>0)
    {
      struct build *p,*q;
      
      Start1:
      q=p=(*((*fdlist).buildnum)).buildfact;
      if (p!=NULL)
      {
        if ((*p).time==4)
        {
          (*((*fdlist).buildnum)).buildfact=(*p).next;
          ((*fdlist).factory)++;
          ((*((*fdlist).buildnum)).kol)--;
          free(p);
          goto Start1;
        }
        while (p!=NULL)
          switch ((*p).time)
          {
            case 4:
              ((*fdlist).factory)++;
              ((*((*fdlist).buildnum)).kol)--;
              (*q).next=(*p).next;
              free(p);
              p=(*q).next;
              break;
            case 3:
              (*fdlist).money-=2500;
            default:
              ((*p).time)++;
              q=p;
              p=(*p).next;
          }
      }
    }
    fdlist=(*fdlist).next;
  }
}

void bankrupt(struct fdstr **fdlist)
{
  struct fdstr *p,*q;
  char s[64];

  Start2:
  if ((*fdlist)!=NULL)
  {
    if ((((**fdlist).money)<0)||(((**fdlist).product)<0))
    {			
			sprintf(s,"\r\nPlayer number %d is bankrupt!\r\n\r\n",
			                                    getnumber((**fdlist).fd,*fdlist));
			write((**fdlist).fd,s,strlen(s));
			write(0,s,strlen(s));
      playerinfo((**fdlist).number,*fdlist,(**fdlist).fd);
      delclient((**fdlist).fd,fdlist);
      goto Start2;
    }
    q=(*fdlist);
    p=(**fdlist).next;
    while (p!=NULL)
    {
      if ((((*p).money)<0)||((((*p).product))<0))
      {
				sprintf(s,"\r\nPlayer number %d is bankrupt!\r\n\r\n",
			                                          getnumber((*p).fd,*fdlist));
				write((*p).fd,s,strlen(s));
				write(0,s,strlen(s));
        playerinfo((*p).number,*fdlist,(*p).fd);
        delclient((*p).fd,fdlist);
      }
      else q=p;
      p=(*q).next;
    }
  }
}

int commands(struct fdstr **fdlist,fd_set readfds,int *i,int mark,
   																struct apl **sell,struct apl **buy,
																	struct produce **prodlist,int allplayers)
{
  char s[64];
  struct fdstr *p;
  
  p=(*fdlist);
  while (p!=NULL)
  {
    if (FD_ISSET((*p).fd,&readfds))
    {
      int delnum;

			if ((delnum=getcom((*p).fd,s,fdlist))==0)
      {
        strcpy((*p).buf+(*p).bufwght,s);
        (*p).bufwght+=strlen(s);
        if ((*p).buf[(*p).bufwght-1]=='\n')
        {
          (*i)+=answer(*fdlist,(*p).fd,s,mark,sell,buy,prodlist,allplayers);
          (*p).buf[0]=0;
          (*p).bufwght=0;
        }
      }
      else
      {
        struct fdstr *q;
        char s[64];

				sprintf(s,"Player number %d lost connection with server\r\n\r\n> ",
					                                                         delnum);
				q=(*fdlist);
        while (q!=NULL)
        {
					write((*q).fd,s,strlen(s));
          q=(*q).next;
        }
				write(0,s,strlen(s));
      }
    }
    p=(*p).next;
  }
  if ((*i)==count(*fdlist)) return (-1);
  return 0;
}

void recvcommands(struct fdstr **fdlist,int mark,struct apl **sell,
                  struct apl **buy,struct produce **prodlist,int allplayers)
{
  fd_set readfds;
  int max=0,i=0;

  for(;;)
  {
    FD_ZERO(&readfds);
    addfdpl(*fdlist,&readfds,&max);
    if (select(max+1,&readfds,NULL,NULL,NULL)<1)
    {
      perror("select");
      exit(1);
    }
    if (commands(fdlist,readfds,&i,mark,sell,buy,prodlist,allplayers)==-1)
			break;
  }
}

void game(struct fdstr *fdlist,int allplayers) /* from TechMod */
{
  int month=0,mark=3;
  struct apl *sell,*buy;
  struct produce *prodlist;

  srand(time(NULL));
  givenumbers(fdlist);
  for(;;)
  {
    if (count(fdlist)<2) break;
    newmonth(&month,fdlist);
    sell=buy=NULL;
    prodlist=NULL;
    recvcommands(&fdlist,mark,&sell,&buy,&prodlist,allplayers);
    endofmonth(fdlist,sell,buy,prodlist,mark);
    cost(fdlist);
    bankrupt(&fdlist);
    chmark(&mark);
  }
  if (count(fdlist)==1)
  {
    char s[64];

		sprintf(s,"\r\nPlayer number %d wins this game!\r\n\r\n",
		                                                      (*fdlist).number);
		write((*fdlist).fd,s,strlen(s));
		write(0,s,strlen(s));
    playerinfo((*fdlist).number,fdlist,(*fdlist).fd);
    delclient((*fdlist).fd,&fdlist);
  }
}
