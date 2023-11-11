#include<stdio.h>
#include<stdlib.h>
#include<fcntl.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<dirent.h>
#include<string.h>
#include<unistd.h>

int main(int argc, char *argv[])
{
  struct stat var;
  
  if(argc!=2)
    {
      perror("Usage: ./program");
      exit(-1);
    }
  if(lstat(argv[1],&var)!=0)
    {
      perror("Eroare la stat");
      exit(-1);
    }
  if(!S_ISDIR(var.st_mode))
    {
      perror(". Eroare director");
      exit(-1);
    }
  DIR *dir;
  dir=opendir(argv[1]);
  if(!dir)
    {
      perror("eroare la desch dir");
      exit(-1);
    }
  int fis_scrie=open("statistica.txt",O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
  if(fis_scrie==-1)
    {
      perror("Nu scrie in statistica\n");
      exit(-1);
    }
  else
    {
      printf("Statistica ok\n");
    }

 


  struct dirent *dir_intrare;
    struct stat fisier;
    char cale[1024];
    char buffer[1024];

    while ((dir_intrare = readdir(dir)) != NULL) {
        if (strcmp(dir_intrare->d_name, ".") == 0 || strcmp(dir_intrare->d_name, "..") == 0) {
            continue;
        }
	// printf(" intra  ");
	//trb sa verific sa nu fie . si .. ca altfel nu merge mai departe
        sprintf(cale, "%s/%s", argv[1], dir_intrare->d_name);
        sprintf(buffer, "Nume: %s", dir_intrare->d_name);
        write(fis_scrie, buffer, strlen(buffer));

        if (lstat(cale, &fisier) != 0) {
            perror("Eroare la lstat");
            continue;
        }
        

        if (S_ISREG(fisier.st_mode)) {
            sprintf(buffer, " Tip: Fisier obisnuit\n");
            write(fis_scrie, buffer, strlen(buffer));
        }
	if (S_ISDIR(fisier.st_mode)) {
            sprintf(buffer, "Tip: Director\n");
            write(fis_scrie, buffer, strlen(buffer));
        }
	if (S_ISLNK(fisier.st_mode)) {
            sprintf(buffer, "Tip: Legatura simbolica\n");
            write(fis_scrie, buffer, strlen(buffer));
        }

       

    }

   closedir(dir);
  close(fis_scrie);
     return 0;
}
