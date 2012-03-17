#include "MainMod.h"

void newsell(struct apl **list,struct apl *p)
{
  struct apl *q,*r;
  
  q=(*list);
  r=NULL;
  while (q!=NULL)
  {
    if ((*q).price>(*p).price) break;
    r=q;
    q=(*q).next;
  }
  if (r==NULL)
  {
    (*list)=(struct apl *)malloc(sizeof(struct apl));
    (**list).pl=(*p).pl;
    (**list).kol=(*p).kol;
    (**list).price=(*p).price;
    (**list).next=q;
  }
  else
  {
    if (q==NULL)
    {
      q=(struct apl *)malloc(sizeof(struct apl));
      (*q).pl=(*p).pl;
      (*q).kol=(*p).kol;
      (*q).price=(*p).price;
      (*q).next=NULL;
      (*r).next=q;
    }
    else
    {
      struct apl *w;
      
      w=(struct apl *)malloc(sizeof(struct apl));
      (*w).pl=(*p).pl;
      (*w).kol=(*p).kol;
      (*w).price=(*p).price;
      (*w).next=q;
      (*r).next=w;
    }
  }
}

void newbuy(struct apl **list,struct apl *p)
{
  struct apl *q,*r;
  
  q=(*list);
  r=NULL;
  while (q!=NULL)
  {
    if ((*q).price<(*p).price) break;
    r=q;
    q=(*q).next;
  }
  if (r==NULL)
  {
    (*list)=(struct apl *)malloc(sizeof(struct apl));
    (**list).pl=(*p).pl;
    (**list).kol=(*p).kol;
    (**list).price=(*p).price;
    (**list).next=q;
  }
  else
  {
    if (q==NULL)
    {
      q=(struct apl *)malloc(sizeof(struct apl));
      (*q).pl=(*p).pl;
      (*q).kol=(*p).kol;
      (*q).price=(*p).price;
      (*q).next=NULL;
      (*r).next=q;
    }
    else
    {
      struct apl *w;
      
      w=(struct apl *)malloc(sizeof(struct apl));
      (*w).pl=(*p).pl;
      (*w).kol=(*p).kol;
      (*w).price=(*p).price;
      (*w).next=q;
      (*r).next=w;
    }
  }
}

void sortapl(struct apl **list,char c)
{
  struct apl *p;
  
  p=(struct apl *)malloc(sizeof(struct apl));
  (*p).pl=(**list).pl;
  (*p).kol=(**list).kol;
  (*p).price=(**list).price;
  
  (*list)=(**list).next;
  while ((*list)!=NULL)
  {
    if (c=='b') newbuy(&p,(*list));
    else newsell(&p,(*list));
    (*list)=(**list).next;
  }
  (*list)=p;
}

void lot(char c,int *max,struct apl *list,struct fdstr *fdlist)
{
  int j=1,i=1+(count(fdlist)*rand()/(RAND_MAX+1.0));
  struct apl *p;

  p=list;
  for (;j<i;j++)
  {
    list=(*list).next;
    if (list==NULL) list=p;
  }
  while ((*max)>0)
  {
    if ((*list).kol>(*max))
      (*list).kol=(*max);
    (*max)-=(*list).kol;
    if (c=='s')
    {
      (*((*list).pl)).product-=(*list).kol;
      (*((*list).pl)).money+=(*list).kol*(*list).price;
    }
    else
    {
      (*((*list).pl)).raw+=(*list).kol;
      (*((*list).pl)).money-=(*list).kol*(*list).price;
    }
    print(c,(*list).kol,(*list).price,(*((*list).pl)).number,fdlist);
    list=(*list).next;
    if (list==NULL) list=p;
  }
}

void auction(struct apl *sell,struct apl *buy,int mark,struct fdstr *fdlist)
{
  int maxbuy=
      BuySell[mark-1][0]*count(fdlist),maxsell=BuySell[mark-1][2]*count(fdlist);

  for(;;)
  {
    if (sell!=NULL)
    {
      struct apl *p,*q;
      int sum=0;

      p=sell;
      while ((*p).price==(*sell).price)
      {
        sum+=(*p).kol;
        q=p;
        p=(*p).next;
        if (p==NULL) break;
      }
      (*q).next=NULL;
      if (sum<=maxsell)
      {
        maxsell-=sum;
        while (sell!=NULL)
        {
          struct apl *t;

          (*((*sell).pl)).product-=(*sell).kol;
          (*((*sell).pl)).money+=(*sell).kol*(*sell).price;
          print('s',(*sell).kol,(*sell).price,
					                                   (*((*sell).pl)).number,fdlist);
          t=sell;
          sell=(*sell).next;
          free(t);
        }
      }
      else
        lot('s',&maxsell,sell,fdlist);
      sell=p;
      if (maxsell==0)
      {
        freeapl(sell);
        sell=NULL;
      }
    }
    else break;
  }
  for(;;)
  {
    if (buy!=NULL)
    {
      struct apl *p,*q;
      int sum=0;

      p=buy;
      while ((*p).price==(*buy).price)
      {
        sum+=(*p).kol;
        q=p;
        p=(*p).next;
        if (p==NULL) break;
      }
      (*q).next=NULL;
      if (sum<=maxbuy)
      {
        maxbuy-=sum;
        while (buy!=NULL)
        {
          struct apl *t;
          
          (*((*buy).pl)).raw+=(*buy).kol;
          (*((*buy).pl)).money-=(*buy).kol*(*buy).price;
          print('b',(*buy).kol,(*buy).price,(*((*buy).pl)).number,fdlist);
          t=buy;
          buy=(*buy).next;
          free(t);
        }
      }
      else
        lot('b',&maxbuy,buy,fdlist);
      buy=p;
      if (maxbuy==0)
      {
        freeapl(buy);
        buy=NULL;
      }
    }
    else break;
  }
}

void endofmonth(struct fdstr *fdlist,struct apl *sell,struct apl *buy,
                                          struct produce *prodlist,int mark)
{
  if (sell!=NULL) sortapl(&sell,'s');
  if (buy!=NULL) sortapl(&buy,'b');
  doline(fdlist);
  auction(sell,buy,mark,fdlist);
  while (prodlist!=NULL)
  {
    struct produce *p;

    if ((*((*prodlist).pl)).product>=0)
		{
    	(*((*prodlist).pl)).money-=(*prodlist).kol*2000;
    	(*((*prodlist).pl)).raw-=(*prodlist).kol;
			(*((*prodlist).pl)).product+=(*prodlist).kol;
		}
    p=prodlist;
    prodlist=(*prodlist).next;
    free(p);
  }
  while (fdlist!=NULL)
  {
    (*fdlist).flag=0;
    fdlist=(*fdlist).next;
  }
}

void playerinfo(int number,struct fdstr *fdlist,int fd)
{
  char s[64];

  while (fdlist!=NULL)
  {
    if ((*fdlist).number==number)
    {
      sprintf(s,"Money - $%ld\r\n",(*fdlist).money);
      write(fd,s,strlen(s));
      sprintf(s,"Raw materials - %d\r\n",(*fdlist).raw);
      write(fd,s,strlen(s));
      sprintf(s,"Product - %d\r\n",(*fdlist).product);
      write(fd,s,strlen(s));
      sprintf(s,"Factories - %d\r\n",(*fdlist).factory);
      write(fd,s,strlen(s));
      sprintf(s,"Factories under construction - %d\r\n\r\n> ",
                                              (*((*fdlist).buildnum)).kol);
      write(fd,s,strlen(s));
      return;
    }
    fdlist=(*fdlist).next;
  }
  sprintf(s,"Player %d is bunkrupt\r\n\r\n",number);
  write(fd,s,strlen(s));
}

void newmonth(int *month,struct fdstr *fdlist)
{
  char s[32];

  doline(fdlist);
  sprintf(s,"\r\nMonth #%d\r\n\r\n> ",++(*month));
  while (fdlist!=NULL)
  {
    write((*fdlist).fd,s,strlen(s));
    fdlist=(*fdlist).next;
  }
	write(0,s,strlen(s)-2
	);
}

void buildproc(int fd,struct fdstr *fdlist)
{
  char s[64];
  
  while ((*fdlist).fd!=fd) fdlist=(*fdlist).next;
  (*fdlist).money-=2500;
  if ((*((*fdlist).buildnum)).kol==0)
  {
    (*((*fdlist).buildnum)).buildfact=
                                   (struct build *)malloc(sizeof(struct build));
    ((*((*((*fdlist).buildnum)).buildfact))).time=0;
    ((*((*((*fdlist).buildnum)).buildfact))).next=NULL;
  }
  else
  {
    struct build *p,*q;
    
    p=(*((*fdlist).buildnum)).buildfact;
    while (p!=NULL)
    {
      q=p;
      p=(*p).next;
    }
    p=(struct build *)malloc(sizeof(struct build));
    (*p).time=0;
    (*p).next=NULL;
    (*q).next=p;
  }
  ((*((*fdlist).buildnum)).kol)++;
  sprintf(s,"Now you have %d factories under construction\r\n\r\n> ",
                                               (*((*fdlist).buildnum)).kol);
  write(fd,s,strlen(s));
}

void addbuysell(int fd,struct fdstr *fdlist,int kol,int price,
                                                          struct apl **list)
{
  struct apl *p,*q;

  p=q=*list;
  while ((*fdlist).fd!=fd)
    fdlist=(*fdlist).next;
  while (p!=NULL)
  {
    if ((*((*p).pl)).number==(*fdlist).number) break;
    q=p;
    p=(*p).next;
  }
  if (q==NULL)
  {
    (*list)=(struct apl *)malloc(sizeof(struct apl));
    (**list).next=NULL;
    (**list).pl=fdlist;
    (**list).kol=kol;
    (**list).price=price;
    write(fd,"Your application was accepted\r\n\r\n> ",35);
  }
  else
  {
    if (p==NULL)
    {
      p=(struct apl *)malloc(sizeof(struct apl));
      (*p).next=NULL;
      (*p).pl=fdlist;
      (*p).kol=kol;
      (*p).price=price;
      (*q).next=p;
      write(fd,"Your application was accepted\r\n\r\n> ",35);
    }
    else
      write((*fdlist).fd,"You can give this command once a month\r\n\r\n",
			                                                                  42);
  }
}

void prodproc(struct fdstr *fdlist,int fd,int kol,struct produce **prodlist)
{
  struct produce *p;

  while ((*fdlist).fd!=fd)
    fdlist=(*fdlist).next;
  p=*prodlist;
  while (p!=NULL)
  {
    if ((*p).pl==fdlist) break;
    p=(*p).next;
  }
  if (p==NULL)
  {
    addprod(prodlist,fdlist);
    p=*prodlist;
    while ((*p).next!=NULL)
      p=(*p).next;
  }
  (*p).kol+=kol;
  if (((*p).kol>(*fdlist).factory)||((*p).kol>(*fdlist).raw)||
	                                          ((*p).kol*2000>(*fdlist).money))
  {
    write(fd,"You don't have enough factories, raw materials or\
money\r\n\r\n",59);
    (*p).kol-=kol;
  }
  else
    write(fd,"Your application was accepted\r\n\r\n> ",35);
}
