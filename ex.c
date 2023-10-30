#include<stdio.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<unistd.h>
#include<stdlib.h>
#include<fcntl.h>


int main(int argc, char *argv[])
{ struct stat n;
  
  if(argc<2)
    {
      printf("Usage ./program ");
      exit(-1);
    }

  if(lstat(argv[1],&n)!=0)
    {
      perror("eroare1");
      exit(-1);
    }
 
  
  if(S_ISREG(n.st_mode)>0)
    {
      printf("ok   ");
    }
  else
    {
      perror("eroare2");
      exit(-1);
    }
  
  if(open("/home/student/grupa3/laboratorSO",O_RDONLY | O_CREAT))
    {
      printf("fisier gasit");
    }
  else
    {
      printf("nu e bine");
    }

  
 
  
  return 0;
}
