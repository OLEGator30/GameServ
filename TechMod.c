#include "TechMod.h"

void infowait(struct fdstr *fdlist,int i,int maxpl)
{
  char s[64];
  
  sprintf(s,"\r\n%d/%d players are connected\r\n\r\n",i,maxpl);
  while (fdlist!=NULL)
  {
    write((*fdlist).fd,s,strlen(s));
    fdlist=(*fdlist).next;
  }
	write(0,s,strlen(s));
}

void waitdialog(struct fdstr **fdlist,fd_set readfds,int *i,int maxpl)
{
  struct fdstr *p;

  p=*fdlist;
  while (p!=NULL)
  {
    if (FD_ISSET((*p).fd,&readfds))
    {
      int con;

      con=read((*p).fd,(*p).buf+(*p).bufwght,128-(*p).bufwght-1);
      if (con>0)
      {
        (*p).bufwght+=con;
        (*p).buf[(*p).bufwght]=0;
        if (strcmp((*p).buf+(*p).bufwght-2,"\r\n")==0)
        {
          write((*p).fd,"Please, wait\r\n",14);
          (*p).bufwght=0;
        }
      }
      else
      {
        (*i)--;
        delclient((*p).fd,fdlist);
        infowait(*fdlist,*i,maxpl);
      }
    }
    p=(*p).next;
  }
}

void waitpl(struct fdstr **fdlist,int ls,int maxpl)
{
  fd_set readfds;
  int max=ls,fd,i=0;
  struct sockaddr_in addr;
  unsigned int len=sizeof(addr);
  
  for(;;)
  {
    FD_ZERO(&readfds);
    FD_SET(ls,&readfds);
    addfdpl(*fdlist,&readfds,&max);
    if (select(max+1,&readfds,NULL,NULL,NULL)<1)
    {
      perror("select");
      exit(1);
    }
    if (FD_ISSET(ls,&readfds))
    {
      if ((fd=accept(ls,(struct sockaddr *)&addr,&len))==-1)
      {
        perror("accept");
        exit(1);
      }
      addfd(fdlist,fd);
      if (i<maxpl) i++;
      if (i==maxpl) return;
      infowait(*fdlist,i,maxpl);
    }
    waitdialog(fdlist,readfds,&i,maxpl);
  }
}

void dolistensock(int *ls,int port)
{
  struct sockaddr_in addr;
  int opt=1;
  
  if (((*ls)=socket(AF_INET,SOCK_STREAM,0))==-1)
  {
    perror("socket");
    exit(1);
  }
  addr.sin_family=AF_INET;
  addr.sin_port=htons(port);
  addr.sin_addr.s_addr=INADDR_ANY;
  setsockopt((*ls),SOL_SOCKET,SO_REUSEADDR,&opt,sizeof(opt));
  if (bind((*ls),(struct sockaddr *)&addr,sizeof(addr))==-1)
  {
    perror("bind");
    exit(1);
  }
  if (listen((*ls),5)==-1)
  {
    perror("listen");
    exit(1);
  }
}

void gameserv(int port,int maxpl)
{
  struct fdstr *fdlist;
  int ls,pipefd[2];
  fd_set readfds;

  dolistensock(&ls,port);
  for (;;)
  {
    fdlist=NULL;
    waitpl(&fdlist,ls,maxpl);
    if (pipe(pipefd)==-1)
    {
      perror("pipe");
      exit(0);
    }
    if (fork()==0)
    {
      /* child */
      
    	close(pipefd[0]);
      game(fdlist); /* goto GameMod */
      write(pipefd[1],"0",1);
      close(pipefd[1]);
			write(0,"\nGame was over!\n\n",17);
      exit(0);
    }
    else
    {
      /* parent */
      
      int max;
      struct sockaddr_in addr;
      unsigned int len=sizeof(addr);

      close(pipefd[1]);
      max=ls>pipefd[0]?ls:pipefd[0];
      for(;;)
      {
        FD_ZERO(&readfds);
        FD_SET(ls,&readfds);
        FD_SET(pipefd[0],&readfds);
        if (select(max+1,&readfds,NULL,NULL,NULL)<1)
        {
          perror("select");
          exit(1);
        }
        if (FD_ISSET(ls,&readfds))
        {
          int fd;
          
          if ((fd=accept(ls,(struct sockaddr *)&addr,&len))==-1)
          {
            perror("accept");
            exit(1);
          }
          write(fd,"\r\nGame was started. Try again later\r\n\r\n",39);
          shutdown(fd,2);
          close(fd);
        }
        if (FD_ISSET(pipefd[0],&readfds)) break; 
      }
      close(pipefd[0]);
      wait(NULL);
    }
  }
}

int main(int argc,char **argv)
{
  int port,maxpl;

  if (argc==3)
  {
    port=strtoint(argv[1]);
    maxpl=strtoint(argv[2]);
    if ((port<0)||(maxpl<=0))
    {
      printf("wrong parametrs\n");
      exit(1);
    }
    gameserv(port,maxpl);
  }
  else
  {
    printf("wrong number of parametrs\n");
    exit(1);
  }
  return 0;
}

