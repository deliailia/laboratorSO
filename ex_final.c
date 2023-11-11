#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <stdint.h>
#include <sys/sysmacros.h>
#include<time.h>


int main(int argc, char *argv[])
{
  struct stat var;
 lstat(argv[1],&var);
  //progr verifica faptul ca a primit un singur argument, tipul si in caz contrar afiseaza eroare
  if(argc!=2)
    {
      perror("USage ./program");
      exit(-1);
    }
 
  if(lstat(argv[1],&var)!=0)
    {
      perror(", eroare");
      exit(-1);
    }
  
  
  if(!S_ISREG(var.st_mode))
    {
      perror(", eroare regular file");
      exit(-1);
    }
    
  //deschid fisierul ca sa pot lua info din el
  //vreau sa citesc din el sa scriu creez si daca exista sterge 
  //4 read 2 write 1 ex sau modurile S_IRUSR,S_IWUSR etc pt restu
  

  int fis_cit;
  fis_cit=open(argv[1], O_RDONLY);

    if(fis_cit==-1)
      {
       perror(". Eroare la citire");
       exit(-1);
      }
     
     char informatii[256];
     int width, height;
     read(fis_cit,informatii,18);
     /*
       if(read(fis_cit,informatii,18)==-1)
       {
       perror("eroare la cit info");
       exit(-1);
       }
       else
       {
       printf("citire ook.");
       }
      */
     read(fis_cit,&width,4);
     read(fis_cit,&height,4);
     int fis_scrie = open("statistica.txt",O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    
    if(fis_scrie==-1)
      {
	perror(".Eroare la desch statistica.\n ");
	exit(-1);
      }
    
    char info[256];
     sprintf(info,"nume fisier: %s \n",argv[1]);
     write(fis_scrie, info, strlen(info));
   
     sprintf(info,"inaltime: %d \n",height);
     write(fis_scrie,info, strlen(info));

     sprintf(info,"latime: %d \n",width);
     write(fis_scrie,info, strlen(info));

     sprintf(info,"dimensiune: %d \n",var.st_size);
     write(fis_scrie,info,strlen(info));

     sprintf(info,"identificatorul utilizatorului: %d\n",var.st_uid);
     write(fis_scrie,info,strlen(info));

     sprintf(info,"timpul ultimei modificari: %s",ctime(&var.st_mtime));
     write(fis_scrie,info,strlen(info)); 

     sprintf(info,"contor de legaturi: %d\n",var.st_nlink);
     write(fis_scrie,info,strlen(info));

     //pentru user dr de acces
     sprintf(info,"drepturi de acces user: ");
     write(fis_scrie,info,strlen(info));
     
     if(var.st_mode!=-1)
       {
	 if(S_IRUSR==00400)
	   {
	     sprintf(info,"R");
	     write(fis_scrie,info,strlen(info));
	   }
	 else
	   {
	     sprintf(info,"-");
	     write(fis_scrie,info,strlen(info));
	   }
	 if(S_IWUSR==00200)
	   {
	     sprintf(info,"W");
	     write(fis_scrie,info,strlen(info));
	   }
	 else
	   {
	     sprintf(info,"-");
	     write(fis_scrie,info,strlen(info));
	   }
       }
     
     if((var.st_mode & S_IXUSR)!=0)
	  {
            sprintf(info,"X\n");
	    write(fis_scrie,info,strlen(info));
	  }
        else
	  {
	     sprintf(info,"-\n");
	     write(fis_scrie,info,strlen(info));
	   }
    

     //pt grup
     sprintf(info,"drepturi de acces grup: ");
     write(fis_scrie,info,strlen(info));
     if(var.st_mode!=-1)
       {
	 if(S_IRGRP & 00040)
	   {
	     sprintf(info,"R");
	     write(fis_scrie,info,strlen(info));
	   }
	 else
	   {
	     sprintf(info,"-");
	     write(fis_scrie,info,strlen(info));
	   }
	 if(S_IWGRP==00020)
	   {
	     sprintf(info,"W");
	     write(fis_scrie,info,strlen(info));
	   }
	 else
	   {
	     sprintf(info,"-");
	     write(fis_scrie,info,strlen(info));
	   }
       }
     if((var.st_mode & S_IXGRP)!=0)
       {
	 sprintf(info,"X\n");
	 write(fis_scrie,info,strlen(info));
       }
     else
       {
	 sprintf(info,"-\n");
	 write(fis_scrie,info,strlen(info));
       }
     //pt other
     sprintf(info,"drepturi de acces altii: ");
     write(fis_scrie,info,strlen(info));
     if(var.st_mode!=-1)
       {
	 if(S_IROTH==00004)
	   {
	     sprintf(info,"R");
	     write(fis_scrie,info,strlen(info));
	   }
	 else
	   {
	     sprintf(info,"-");
	     write(fis_scrie,info,strlen(info));
	   }
	 if((S_IWOTH & 00002)!=0)
	   {
	     sprintf(info,"W");
	     write(fis_scrie,info,strlen(info));
	   }
	 else
	   {
	     sprintf(info,"-");
	     write(fis_scrie,info,strlen(info));
	   }
       }
     if((var.st_mode & S_IXOTH)!=0)
       {
	 sprintf(info,"X");
	 write(fis_scrie,info,strlen(info));
       }
     else
       {
	 sprintf(info,"-");
	 write(fis_scrie,info,strlen(info));
       }
     
     //nu merge sa ia drepturile pt grup le scrie gresit
     close(fis_cit);
     
      close(fis_scrie);
  
  return 0;
}
