#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <string.h>
#include <unistd.h>
#include<libgen.h>

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
    char buffer[4096];
    char cale[4096];
    char cale_pt_link[400];
    char cale_pt_link2[1024];
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
	    getcwd(cale_pt_link,9999);
	    // printf("%s",cale_pt_link);
	    snprintf(cale_pt_link2,9999,"%s/%s",cale_pt_link,argv[1]);
	    //  printf("%s\n",cale_pt_link2);
	    
            sprintf(buffer, "Nume: %s ", dir_intrare->d_name);
            write(fis_scrie, buffer, strlen(buffer));

            if (S_ISREG(fisier.st_mode))
	      {
		sprintf(buffer, "Tip: Regular");
                write(fis_scrie, buffer, strlen(buffer));
		
		if(strstr(dir_intrare->d_name,".bmp"))
		  {
		    char cale_poza[1024];
		    char cale_poza_comp[4096];
		    getcwd(cale_poza,1024);
		    //printf("%s\n",cale_poza);
		    sprintf(cale_poza_comp,"%s",cale_poza);
		    //printf("%s",cale_poza_comp);
		    sprintf(buffer,", este fisier tip BMP\n");
		    write(fis_scrie,buffer,strlen(buffer));
		    sprintf(buffer,"Aceasta este calea imaginii: ");
		    write(fis_scrie,buffer,strlen(buffer));
		    snprintf(cale_poza_comp,4096,"%s/%s",cale_poza,argv[1]);
		    //printf("%s\n",cale_poza_comp);
		    write(fis_scrie,cale_poza_comp,strlen(cale_poza_comp));
		    //cale_poza_comp e aia in care al /home/argv[1]/directmare/argv[1]
		    int poza=open(cale_poza_comp,O_RDONLY);
		    if(poza==-1)
		      {
			perror("eroare deschidere poza");
			exit(-1);
		      }
		    int width,height;
		    char informatii_poza[256];
		    read(poza,informatii_poza,18);
		    if(read(poza,informatii_poza,18)==-1)
		      {
			perror("nu se pot citi info");
			exit(-1);
		      }
		    else
		      {
			printf("citire ok la poza ");
		      }
		    read(poza,&width,4);
		    if(read(poza,&width,4)==-1)
		      {
			perror(" nu merge lungimea");
			exit(-1);
		      }
		    else
		      {
			printf("%d",width);
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
                sprintf(buffer, "Tip: Director \n");
                write(fis_scrie, buffer, strlen(buffer));
		sprintf(buffer, "\t Numele directorului: %s\n",dir_intrare->d_name);
		write(fis_scrie, buffer, strlen(buffer));

		sprintf(buffer, "\t Identificatorul utilizatorului: %d\n",fisier.st_uid);
		write(fis_scrie, buffer, strlen(buffer));

		sprintf(buffer, "\t Drepturi de acces user: ");
		write(fis_scrie,buffer,strlen(buffer));
		if(fisier.st_mode!=-1)
		  {
		    if(S_IRUSR==00400)
		      {
			sprintf(buffer,"R");
			write(fis_scrie,buffer,strlen(buffer));
		      }
		    else
		      {
			sprintf(buffer,"-");
			write(fis_scrie,buffer,strlen(buffer));
		      }
		    if(S_IWUSR==00200)
		      {
			sprintf(buffer,"W");
			write(fis_scrie,buffer,strlen(buffer));
		      }
		    else
		      {
			sprintf(buffer,"-");
			write(fis_scrie,buffer,strlen(buffer));
		      }
		  }
		if((S_IXUSR & fisier.st_mode)!=0)
		  {
		    sprintf(buffer,"X\n");
		    write(fis_scrie,buffer,strlen(buffer));
		  }
		else
		  {
		    sprintf(buffer,"-\n");
		    write(fis_scrie,buffer,strlen(buffer));
		  }

		//pt grup
		sprintf(buffer, "\t Drepturi de acces grup: ");
		write(fis_scrie,buffer,strlen(buffer));
		
		if(fisier.st_mode!=-1)
		  {
		    if(S_IRGRP)
		      {
			sprintf(buffer,"R");
			write(fis_scrie,buffer,strlen(buffer));
		      }
		    else
		      {
			sprintf(buffer,"-");
			write(fis_scrie,buffer,strlen(buffer));
		      }
		    if(S_IWGRP)
		      {
			sprintf(buffer,"W");
			write(fis_scrie,buffer,strlen(buffer));
		      }
		    else
		      {
			sprintf(buffer,"-");
			write(fis_scrie,buffer,strlen(buffer));
		      }
		  }
		//nu merge 
		if((fisier.st_mode & S_IXGRP)!=0)
		  {
		    sprintf(buffer,"X\n");
		    write(fis_scrie,buffer,strlen(buffer));
		  }
		else
		  {
		    sprintf(buffer,"-\n");
		    write(fis_scrie,buffer,strlen(buffer));
		  }
		//pt other
		sprintf(buffer,"\t Drepturi de acces altii: ");
		write(fis_scrie,buffer,strlen(buffer));
		
		if(fisier.st_mode!=-1)
		  {
		    if(S_IROTH)
		      {
			sprintf(buffer,"R");
			write(fis_scrie,buffer,strlen(buffer));
		      }
		    else
		      {
			sprintf(buffer,"-");
			write(fis_scrie,buffer,strlen(buffer));
		      }
		    if(S_IWOTH)
		      {
			sprintf(buffer,"W");
			write(fis_scrie,buffer,strlen(buffer));
		      }
		    else
		      {
			sprintf(buffer,"-");
			write(fis_scrie,buffer,strlen(buffer));
		      }
		  }
		if((fisier.st_mode & S_IXOTH)!=0)
		  {
		    sprintf(buffer,"X\n");
		    write(fis_scrie,buffer,strlen(buffer));
		  }
		else
		  {
		    sprintf(buffer,"-\n");
		    write(fis_scrie,buffer,strlen(buffer));
		  }

		
		
	      }
	    if (S_ISLNK(fisier.st_mode))
	      {
		struct stat link_stat;
                sprintf(buffer, "Tip: Legatura simbolica\n");
                write(fis_scrie, buffer, strlen(buffer));

		 char dest[1024];
		
		 ssize_t link=readlink(cale,dest,sizeof(dest)-1);
                if (link!=-1)
		  {
		    dest[link]='\0';
                    sprintf(buffer, "\t Nume legatura: %s\n", dest);
                    write(fis_scrie, buffer, strlen(buffer));

		    lstat(cale_pt_link2,&link_stat);
		    printf("%s\n",cale_pt_link2); //home/argv[1]/dirmare/argv[1]
		    if(lstat(cale_pt_link2,&link_stat)==-1)
		      {
			perror("LSTAT");
			exit(-1);
		      }
		    struct stat target_stat;
		    //char cale_pt_link3[2048];
		    // snprintf(cale_pt_link3,9999,"%s/%s\n",cale_pt_link2,dir_intrare->d_name);
		    //printf("%s\n",cale_pt_link3);
		     if(lstat(cale_pt_link2,&target_stat)!=-1)
		      {
			printf("%ld",target_stat.st_size);
			if(S_ISREG(target_stat.st_mode))
			  {
			    printf("buffer");
			    sprintf(buffer,"\t Dimensiune legatura: %ld\n",target_stat.st_size );
			    //	printf("%ld ",link_stat.st_blksize);
			    //	printf("%ld ",link_stat.st_size);
			    write(fis_scrie,buffer,strlen(buffer));
			  }
			else
			  {
			    perror(" nu e fisier regular");
			  }

			sprintf(buffer, "\t Drepturi de acces user legatura: \n");
			write(fis_scrie, buffer, strlen(buffer));

			sprintf(buffer, "\t Drepturi de acces grup legatura: \n ");
			write(fis_scrie, buffer, strlen(buffer));

			sprintf(buffer, "\t Drepturi de acces altii legatura: \n");
			write(fis_scrie, buffer, strlen(buffer));
	        	 }
		     else
		        {
		       	perror("lstat2");
		       	exit(-1);
		         }
		  }
			   
		else
		  {
		    perror("nu merge link");
		    exit(-1);
		  }
		
	      }
	  }
    }

    closedir(dir);
    close(fis_scrie);

    return 0;
}
