#include "OtherMod.h"

const float BuySell[5][4]=
{
  {1.0 , 800 , 3.0 , 6500},
  {1.5 , 650 , 2.5 , 6000},
  {2.0 , 500 , 2.0 , 5500},
  {2.5 , 400 , 1.5 , 5000},
  {3.0 , 300 , 1.0 , 4500},
};

int strtoint(char *s)
{
  int i=0,k=0;

  for (;i<strlen(s);i++)
  {
    if ((s[i]>='0')&&(s[i]<='9')) k=k*10+s[i]-'0';
    else return (-1);
  }
  return k;
}

void addfdpl(struct fdstr *fdlist,fd_set *readfds,int *max)
{  
  while (fdlist!=NULL)
  {
    FD_SET((*fdlist).fd,readfds);
    if ((*fdlist).fd>(*max)) (*max)=(*fdlist).fd;
    fdlist=(*fdlist).next;
  }
}

void addfd (struct fdstr **fdlist,int fd)
{
  if ((*fdlist)==NULL)
  {
    (*fdlist)=(struct fdstr *)malloc(sizeof(struct fdstr));
    (**fdlist).next=NULL;
    (**fdlist).fd=fd;
    (**fdlist).bufwght=0;
    (**fdlist).number=0;
    (**fdlist).money=10000;
    (**fdlist).raw=4;
    (**fdlist).factory=2;
    (**fdlist).product=2;
    (**fdlist).buildnum=(struct num *)malloc(sizeof(struct num));
    (*((**fdlist).buildnum)).kol=0;
    (*((**fdlist).buildnum)).buildfact=NULL;
    (**fdlist).flag=0;
    (**fdlist).buf=(char *)malloc(128*sizeof(char));
  }
  else
  {
    struct fdstr *p,*q;
    
    p=(*fdlist);
    while ((*p).next!=NULL) p=(*p).next;
    q=(struct fdstr *)malloc(sizeof(struct fdstr));
    (*p).next=q;
    (*q).next=NULL;
    (*q).fd=fd;
    (*q).bufwght=0;
    (*q).number=0;
    (*q).money=10000;
    (*q).raw=4;
    (*q).factory=2;
    (*q).product=2;
    (*q).buildnum=(struct num *)malloc(sizeof(struct num));
    (*((*q).buildnum)).kol=0;
    (*((*q).buildnum)).buildfact=NULL;
    (*q).flag=0;
    (*q).buf=(char *)malloc(128*sizeof(char));
  }
}

void addprod(struct produce **prodlist,struct fdstr *fdlist)
{
  if (*prodlist==NULL)
  {
    (*prodlist)=(struct produce *)malloc(sizeof(struct produce));
    (**prodlist).kol=0;
    (**prodlist).next=NULL;
    (**prodlist).pl=fdlist;
  }
  else
  {
    struct produce *p,*q;

    p=*prodlist;
    while ((*p).next!=NULL)
      p=(*p).next;
    q=(struct produce *)malloc(sizeof(struct produce));
    (*q).kol=0;
    (*q).next=NULL;
    (*q).pl=fdlist;
    (*p).next=q;
  }
}

void freebuildfact(struct build *list)
{
  struct build *p;

  p=(*list).next;
  while(p!=NULL)
  {
    free(list);
    list=p;
    p=(*p).next;
  }
  free(list);
}

void delclient(int fd,struct fdstr **fdlist)
{
  struct fdstr *p,*q;

  shutdown(fd,2);
  close(fd);
  p=q=*fdlist;
  if ((*p).fd==fd)
  {
    (*fdlist)=(*p).next;
    free((*p).buf);
    if ((*((*p).buildnum)).kol>0)
      freebuildfact((*((*p).buildnum)).buildfact);
    free((*p).buildnum);
    free(p);
    return;
  }
  p=(*p).next;
  while (p!=NULL)
  {
    if (((*p).fd)==fd)
    {
      (*q).next=(*p).next;
      free((*p).buf);
      if ((*((*p).buildnum)).kol>0)
        freebuildfact((*((*p).buildnum)).buildfact);
      free((*p).buildnum);
      free(p);
      return;
    }
    else
    {
      q=p;
      p=(*p).next;
    }
  }
}

int getnumber(int fd,struct fdstr *fdlist)
{
	while (fdlist!=NULL)
	{
		if ((*fdlist).fd==fd) return (*fdlist).number;
		fdlist=(*fdlist).next;
  }
	return 0;
}

int getcom(int fd,char *str,struct fdstr **fdlist)
{
  int i,fl=0;
  char *s;

  i=read(fd,str,63);
  if (i==0)
  {
		int delnum=getnumber(fd,*fdlist);

    delclient(fd,fdlist);
    return delnum;
  }
  str[i]=0;
  s=str;
  while ((*s)!=0)
  {
    if ((*s)=='\r') strcpy(s,s+1);
    else
      switch (fl)
      {
        case 1:
          if (((*s)==' ')||((*s)=='\t')||((*s)=='\n'))
            strcpy(s-1,s);
          else
          {
            fl=0;
            s+=1;
          }
          break;
        case 0:
          if (((*s)==' ')||((*s)=='\t'))
          {
            fl=1;
            if ((*s)=='\t') (*s)=' ';
          }
          s+=1;
      }
  }
  return 0;
}

int count(struct fdstr *fdlist)
{
  int k=0;
  
  while (fdlist!=NULL)
  {
    k++;
    fdlist=(*fdlist).next;
  }
  return k;
}

int devide(char *str,int *kol,int *price)
{
  int i=0;
  char *s;
  
  s=(char *)malloc(64*sizeof(char));
  while (str[i]!=0)
  {
    if (str[i]==' ') break;
    i++;
  }
  if (str[i]==0) return (-1);
  strcpy(s,str+i+1);
  str[i]=0;
  if (((*kol)=strtoint(str))==-1) return (-1);
  i=0;
  while (s[i]!=0)
  {
    if (s[i]=='\n') break;
    i++;
  }
  if (s[i]==0) return (-1);
  s[i]=0;
  if (((*price)=strtoint(s))==-1) return (-1);
  return 0;
}

int mystrcmp(char *s1,const char *s2)
{
  int min,i=0;

  min=strlen(s1)>strlen(s2)?strlen(s2):strlen(s1);
  for (;i<min;i++)
    if (s1[i]!=s2[i]) break;

  return i;
}

void givenumbers(struct fdstr *fdlist)
{
  int i=1;
  char s[128];
  
  while (fdlist!=NULL)
  {
    (*fdlist).number=i++;
    sprintf(s,"\r\nGame began!\r\n\r\n");
		sprintf(s+strlen(s),"Your number is %d\r\n\r\n",(*fdlist).number);
		sprintf(s+strlen(s),"If you'll need a help, try 'help'\r\n\r\n");
    write((*fdlist).fd,s,strlen(s));
    fdlist=(*fdlist).next;
  }
	write(0,"Game began!\n\n",13);
}

void doline(struct fdstr *fdlist)
{
  while (fdlist!=NULL)
  {
    write((*fdlist).fd,"\r\n------------------------------------------\r\n",
		                                                                    46);
    fdlist=(*fdlist).next;
  }
  write(0,"\r\n----------------------------------------------\r\n\r\n",52);
}

void endofturn(int fd,struct fdstr *fdlist)
{
  while ((*fdlist).fd!=fd)
    fdlist=(*fdlist).next;
  (*fdlist).flag=1;
}

int getflag(int fd,struct fdstr *fdlist)
{
  while ((*fdlist).fd!=fd)
    fdlist=(*fdlist).next;
  return ((*fdlist).flag);
}

void print(char c,int kol,int money,int number,struct fdstr *fdlist)
{
  char s[64];
  
  if (c=='s')
	{
    sprintf(s,"Player %d sold %d items of products for $%d per item\n",
                                                          number,kol,money);
    while (fdlist!=NULL)
    {
      write((*fdlist).fd,s,strlen(s));
      fdlist=(*fdlist).next;
    }
	}
  if (c=='b')
	{
    sprintf(s,"Player %d bought %d items of raw materials for $%d per\
 item\n",number,kol,money);
    while (fdlist!=NULL)
    {
      write((*fdlist).fd,s,strlen(s));
      fdlist=(*fdlist).next;
    }
	}
	write(0,s,strlen(s));
}

void freeapl(struct apl *list)
{
  while (list!=NULL)
  {
    struct apl *p;
    
    p=list;
    list=(*list).next;
    free(p);
  }
}

void help(int fd)
{
  char s[1024];
  
  sprintf(s,"This game has next commands:\n");
  sprintf(s+strlen(s),
                "1. 'market' - Get information about market this month.\n");
  sprintf(s+strlen(s),
           "2. 'player N' - Get information about player with N number.\n");
  sprintf(s+strlen(s),"3. 'prod N' - Produce N items of products at $2000 for each item. Applications for production are summed\n");
  sprintf(s+strlen(s),"4. 'buy N P' - Purchase N items of raw materials at P price. You can use this command once a month.\n");
  sprintf(s+strlen(s),"5. 'sell N P' - Sell N items of products at P price. You can use this command once a month.\n");
  sprintf(s+strlen(s),"6. 'build' - Build a new factory at $2500 instantly and $2500 after three months of construction. You will be able to use it after four months.\n");
  sprintf(s+strlen(s),"7. 'turn' - Finish vigorous activity this month (you couldn't purchase, sale and build this month).\n");
  sprintf(s+strlen(s),"8. 'help' - Get help.\n\n> ");
  write(fd,s,strlen(s));
}

int correctsell(int fd,int price,int mark)
{
  if (price>BuySell[mark-1][3])
  {
    write(fd,"Too high price. Try 'market'\r\n\r\n> ",34);
    return (-1);
  }
  return 0;
}

int correctbuy(int fd,int price,int mark)
{
  if (price<BuySell[mark-1][1])
  {
    write(fd,"Too low price. Try 'market'\r\n\r\n> ",33);
    return (-1);
  }
  return 0;
}

