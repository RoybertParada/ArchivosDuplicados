#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


struct node{
   struct stat info;
   int is_dir;
   struct dirent *dt;
   struct node *next;
};

struct dir{
   struct stat info;
   int visitado;
   struct dirent *dt;
   struct dir *vnext;
};

struct node *head = NULL;
struct node *current = NULL;
struct dir *vhead = NULL;
struct dir *vcurrent = NULL;

void insertVisitado(struct stat info, int is_dir, struct dirent *dt){
   //create a link
   struct node *link = (struct node*) malloc(sizeof(struct node));
   
   link->info = info;
   link->is_dir = is_dir;
   link->dt = dt;
   printf("Vi: %-20s %ld \n",link->dt->d_name,link->info.st_size);

   link->next = head;
   head = link;
}

void insertAvisitar(struct stat info, int visitado, struct dirent *dt){
   //create a link
   struct dir *vlink = (struct dir*) malloc(sizeof(struct dir));
   
   vlink->info = info;
   vlink->visitado = visitado;
   vlink->dt = dt;

   printf("Av: %-20s %ld \n",vlink->dt->d_name,vlink->info.st_size);

   vlink->vnext = vhead;
   vhead = vlink;
}



void lista_directorio(char *nombre);

int main(int argc, char *argv[]){
 	
 	
	lista_directorio("/home/roybert/Documentos/");
	return 0;
}
 
//Función que permite listar un directorio de manera recursiva
void lista_directorio(char *nombre){
 
	//Declaramos variables, estructuras
	struct stat estru;
	struct dirent *dt;
	DIR *dire;

	dire = opendir(nombre); 
	printf("abriendo el directorio %s\n",nombre);
	printf("Si imprime el otro imprijme este: %-20s \n",dt->d_name);



	while((dt=readdir(dire))!=NULL){
		
		if((strcmp(dt->d_name,".")!=0)&&(strcmp(dt->d_name,"..")!=0)){
			stat(dt->d_name,&estru);
 			
			if(dt->d_type == DT_DIR){
				printf("DIRECTORIO \n");
				insertAvisitar(estru,0,dt);
				printf("Final: %-20s \n",dt->d_name);
				lista_directorio(dt->d_name);

			}else{
				insertVisitado(estru,0,dt);
			}
		}

	}
	closedir(dire);
}
