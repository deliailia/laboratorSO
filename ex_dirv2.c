#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
   struct stat var;
    if (argc != 2)
      {
        perror("Usage: ./program <director_intrare>");
        exit(-1);
      }

    lstat(argv[1], &var);
    if (!S_ISDIR(var.st_mode))
      {
        perror("Eroare: Argumentul nu este un director");
        exit(-1);
      }

    DIR *dir = opendir(argv[1]);
    if (!dir)
      {
        perror("Eroare la deschiderea directorului");
        exit(-1);
      }

    int fis_scrie = open("statistica.txt", O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fis_scrie == -1)
      {
        perror("Eroare la deschiderea fisierului statistica.txt");
        exit(-1);
      }

    struct dirent *dir_intrare;
    struct stat fisier;
    char buffer[1024];
    char cale[1024];
    while ((dir_intrare = readdir(dir)) != NULL)
      {
        if (strcmp(dir_intrare->d_name, ".") != 0 && strcmp(dir_intrare->d_name, "..") != 0)
	  {
            sprintf(cale, "%s/%s", argv[1], dir_intrare->d_name);
	    
            if (lstat(cale, &fisier) != 0)
	      {
                perror("Eroare la lstat");
                continue;
	      }

            sprintf(buffer, "Nume: %s ", dir_intrare->d_name);
            write(fis_scrie, buffer, strlen(buffer));

            if (S_ISREG(fisier.st_mode))
	      {
		sprintf(buffer, "Tip: Regular");
                write(fis_scrie, buffer, strlen(buffer));
		if(strstr(dir_intrare->d_name,".bmp"))
		  {
		    sprintf(buffer,", este fisier tip BMP\n");
		    write(fis_scrie,buffer,strlen(buffer));

		    //construire cale completa dir_intrare->d_name e doar numele
		    char cale_poza[1024];
		    sprintf(cale_poza,"./%s",dir_intrare->d_name);
		    printf("%s",cale_poza);
		    int poza=open(cale_poza,O_RDONLY);
		    if(poza==-1)
		      {
			perror("eroare deschidere poza");
			exit(-1);
		      }
		    else
		      {
			printf("poza deschisa");
		      }
		    
		    



		    
		  }
		else
		  {
		    sprintf(buffer,", nu este BMP\n");
		    write(fis_scrie,buffer,strlen(buffer));
		  }




		
                
	      }
	    if (S_ISDIR(fisier.st_mode))
	      {
                sprintf(buffer, "Tip: Director\n");
                write(fis_scrie, buffer, strlen(buffer));
	      }
	    if (S_ISLNK(fisier.st_mode))
	      {
                sprintf(buffer, "Tip: Legatura simbolica ");
                write(fis_scrie, buffer, strlen(buffer));

                char dest[18];
                if (readlink(cale,dest, 18) != -1)
		  {
                    sprintf(buffer, " catre: %s\n", dest);
                    write(fis_scrie, buffer, strlen(buffer));
		  }
	      }   

           
        }
    }

    closedir(dir);
    close(fis_scrie);

    return 0;
}
