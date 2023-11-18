#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

void fisiereReg(struct stat var, char *numefis, int fis_scrie, char *cale_pt_fisier )
{
  char info[256],informatii[256];
  int width=0,height=0;
  int fis_cit= open(cale_pt_fisier, O_RDONLY);
  //printf("%s\n",cale_pt_fisier);
  if( read(fis_cit,info,18)==-1)
    {
      perror("eroare citire fisbmp");
      // exit(-1);
    }
  else
    {
      printf("se poate citi cu read\n");
    }
  read(fis_cit,&width,4);
  read(fis_cit,&height,4);
  sprintf(informatii,"Nume: %s,",numefis);
  write(fis_scrie,informatii,strlen(informatii));

  if(strstr(numefis,".bmp"))
    {
      sprintf(informatii," fisier de tip BMP \n");
      write(fis_scrie,informatii,strlen(informatii));
      
      sprintf(informatii,"\t Inaltime: %d \n",height);
      write(fis_scrie,informatii,strlen(informatii));
      
      sprintf(informatii,"\t Lungime: %d \n",width);
      write(fis_scrie,informatii,strlen(informatii));

    }
  else
    {
      sprintf(informatii," nu e fisier BMP \n");
      write(fis_scrie, informatii,strlen(informatii));
    }

  sprintf(informatii,"\t Dimensiune: %ld \n",var.st_size);
  write(fis_scrie,informatii,strlen(informatii));
	
  sprintf(informatii,"\t Identificatorul utilizatorului: %d \n",var.st_mode);
  write(fis_scrie,informatii,strlen(informatii));

  sprintf(informatii,"\t Timpul ultimei modificari: %s", ctime(&var.st_mtime));
  write(fis_scrie,informatii,strlen(informatii));

  sprintf(informatii,"\t Contorul de legaturi: %ld \n",var.st_nlink);
  write(fis_scrie,informatii,strlen(informatii));
  

}

void fisiereDir(struct stat var, char *numefis, int fis_scrie)
{
  char informatii[256];
  sprintf(informatii,"Nume: %s\n",numefis);
  write(fis_scrie,informatii,strlen(informatii));

  sprintf(informatii,"\t Identificatorul utilizatorului: %d\n",var.st_uid);
  write(fis_scrie,informatii,strlen(informatii));
}

void fisiereLink(struct stat var, char *numefis, char *cale_link, int fis_scrie)
{
  char informatii[500];
  char link_target[256];
  struct stat target_stat;
  //printf("%sCALELINK@\n",cale_link);
  ssize_t cit_link=readlink(cale_link,link_target,sizeof(link_target)-1);

  sprintf(informatii,"Nume: %s\n",numefis);
  write(fis_scrie,informatii,strlen(informatii));
  
  if(cit_link==-1)
    {
      perror("eroare la readlink\n");
      // exit(-1);
    }
  else
    {
      printf("merge link\n");
      link_target[cit_link]='\0';
      sprintf(informatii,"\t Nume legatura: %s\n",link_target);
      write(fis_scrie,informatii,strlen(informatii));
    }
  
  sprintf(informatii,"\t Dimensiunea legaturii: %ld\n",var.st_size);
  write(fis_scrie,informatii,strlen(informatii));
    
  if(lstat(cale_link,&target_stat)==-1)
    {
      perror("nu merge lstat link\n");
    }
  else
    {
      printf("merge lstat link\n");
    }
  
  
  sprintf(informatii,"\t Dimensiune fisierului tinta: %ld\n",target_stat.st_size);
  write(fis_scrie,informatii,strlen(informatii));
}

void drepturiDeAcces(struct stat var, char *numefis, int fis_scrie)
{
  char informatii[256];
  sprintf(informatii,"\t Drepturi de acces user: ");
  write(fis_scrie,informatii, strlen(informatii));
  if(S_IRUSR & var.st_mode)
    {
      sprintf(informatii,"R");
      write(fis_scrie,informatii,strlen(informatii));
    }
  else
    {
      sprintf(informatii,"-");
      write(fis_scrie,informatii,strlen(informatii));
    }
  if(S_IWUSR & var.st_mode)
    {
      sprintf(informatii,"W");
      write(fis_scrie,informatii,strlen(informatii));
    }
  else
    {
      sprintf(informatii,"-");
      write(fis_scrie,informatii,strlen(informatii));
    }
  if(S_IXUSR & var.st_mode)
    {
      sprintf(informatii,"X\n");
      write(fis_scrie,informatii,strlen(informatii));
    }
  else
     {
      sprintf(informatii,"-\n");
      write(fis_scrie,informatii,strlen(informatii));
    }

  
  sprintf(informatii,"\t Drepturi de acces grup: ");
  write(fis_scrie,informatii,strlen(informatii));
   if(S_IRGRP & var.st_mode)
    {
      sprintf(informatii,"R");
      write(fis_scrie,informatii,strlen(informatii));
    }
  else
    {
      sprintf(informatii,"-");
      write(fis_scrie,informatii,strlen(informatii));
    }
  if(S_IWGRP & var.st_mode)
    {
      sprintf(informatii,"W");
      write(fis_scrie,informatii,strlen(informatii));
    }
  else
    {
      sprintf(informatii,"-");
      write(fis_scrie,informatii,strlen(informatii));
    }
  if(S_IXGRP & var.st_mode)
    {
      sprintf(informatii,"X\n");
      write(fis_scrie,informatii,strlen(informatii));
    }
  else
     {
      sprintf(informatii,"-\n");
      write(fis_scrie,informatii,strlen(informatii));
    }

  sprintf(informatii,"\t Drepturi de acces altii: ");
  write(fis_scrie,informatii,strlen(informatii));
   if(S_IROTH & var.st_mode)
    {
      sprintf(informatii,"R");
      write(fis_scrie,informatii,strlen(informatii));
    }
  else
    {
      sprintf(informatii,"-");
      write(fis_scrie,informatii,strlen(informatii));
    }
  if(S_IWOTH & var.st_mode)
    {
      sprintf(informatii,"W");
      write(fis_scrie,informatii,strlen(informatii));
    }
  else
    {
      sprintf(informatii,"-");
      write(fis_scrie,informatii,strlen(informatii));
    }
  if(S_IXOTH & var.st_mode)
    {
      sprintf(informatii,"X\n");
      write(fis_scrie,informatii,strlen(informatii));
    }
  else
     {
      sprintf(informatii,"-\n");
      write(fis_scrie,informatii,strlen(informatii));
    }


}
void parcurgeDinDir(char *dir, char *cale, int fis_scrie)
{
  struct stat var;
  struct dirent *fis_intrare;
  struct stat fisier;
  char cale_pt_fisier[515];
  char buffer[256];
  DIR *d=opendir(dir);
  if(d==NULL)
    {
      perror("Nu deschide dir\n");
      exit(-1);
    }
  else
    {
      printf("dir deschis\n");
    }
  while((fis_intrare=readdir(d))!=NULL)
    {
      lstat(fis_intrare->d_name,&var);

      if(strcmp(fis_intrare->d_name,".")==0 || strcmp(fis_intrare->d_name,"..")==0)
	{
	  continue;
	}
      char buffer2[512];
      getcwd(buffer,sizeof(buffer));
      //printf("%sBUFFER\n",buffer);
      sprintf(cale_pt_fisier,"%s/%s/%s",buffer,cale,fis_intrare->d_name);
      // printf("1\n");
      //printf("%sCALEFISIER\n",cale_pt_fisier);
      sprintf(buffer2,"%s/%s",buffer,dir);
      //printf("%sBUFFER2\n",buffer2);
      if(lstat(cale_pt_fisier,&fisier)==-1)
	{
	  perror("nu intra in lstat la fisier\n");
	  exit(-1);
	}
      else
	{
	  printf("intra in lstat fisier\n");
	}
      if(S_ISREG(fisier.st_mode))
	{
	      fisiereReg(fisier,fis_intrare->d_name, fis_scrie,cale_pt_fisier);
              drepturiDeAcces(fisier, fis_intrare->d_name,fis_scrie);
	   printf("%s\n",fis_intrare->d_name);
	}
      if(S_ISDIR(fisier.st_mode))
	{
	  fisiereDir(fisier, fis_intrare->d_name, fis_scrie);
	  drepturiDeAcces(fisier, fis_intrare->d_name, fis_scrie);
	}
      if(S_ISLNK(fisier.st_mode))
	{
	  char cale_link[300];
          sprintf(cale_link,"%s/%s",cale,fis_intrare->d_name);
	  fisiereLink(fisier,fis_intrare->d_name, cale_link, fis_scrie);
	  drepturiDeAcces(fisier,fis_intrare->d_name,fis_scrie);
	  
	}
      
    }
	  
  
}


int main(int argc, char *argv[])
{
  struct stat var;
  int fis_scrie=open("statistica.txt",O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  
  char cale[20];
  sprintf(cale,"%s",argv[1]);
  // printf("%s\n",cale);
  
  if(argc!=2)
    {
      perror("usage ./program");
      exit(-1);
    }
  else
    {
      printf("2 argumente\n");
    }
  if(lstat(argv[1], &var)==-1)
    {
      perror("eroare la lstat\n");
      exit(-1);
    }
  else
    {
      printf("merge lstat\n");
    }
  if(!S_ISDIR(var.st_mode))
    {
      perror("Nu e director");
      exit(-1);
    }
  else
    {
      printf("e director\n");
    }

  if(fis_scrie==-1)
    {
      perror("eroare la statistica\n");
      exit(-1);
    }
  else
    {
      printf("merge statistica\n");
    }
 
  parcurgeDinDir(argv[1],cale,fis_scrie);
  close(fis_scrie);
  
  return 0;
}
