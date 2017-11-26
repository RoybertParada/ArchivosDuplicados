/*###########################################################################
#      Proyecto 1 de Sistemas Operativos - Semestre I-2017                  #
#      Integrantes: Parada Roybert 25418978 C1                              #
#                   Diego Chacon 26343836 C1                                #
###########################################################################*/

#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>

int j=1;
int count=1;             // Contador global para ir agregando los directorios al pool
int duplicado = 0;       // Contador de duplicados
char *verhash;           // Verifica hash duplicado
char  md5[10];    
char  name[20];          // Nombre del archivo al que se le realizara el hash
char  readbuffer[32];    // Contendra el hash 
char  *numhilos;         // Numero de hilos pasados por terminal
char  *rutaini;          // Ruta de inicio  
char  *mode;             // Indicara si el md5 es binario o por libreria
FILE *fp;                // Descriptor para el pipe generado por popen

pthread_mutex_t key;

struct directoy{         // Estructura para guardar cada directorio en el pool de directorios
    char *path;          // Ruta del directorio
    int visitado;        // Si el directorio ha sido visitado  visitado = 1 si no visitado = 0
};

struct directoy pool[40]; // El pool de directorios

struct archivo{           // struct para guardar cada uno de los archivos a comparar
   struct stat info;      
   struct dirent *dt;
   struct archivo *next; 
   char *hash;     
};


struct archivo *head = NULL;
struct archivo *current = NULL;
struct archivo *link2 = NULL;


void insertVisitado(struct stat info, struct dirent *dt, char *hash){           // Funcion de insertar de una lista para ir guardando los archivos
   
   struct archivo *link = (struct archivo*) malloc(sizeof(struct archivo));
   
   link->info = info;
   link->dt = dt;
   link->hash = hash;
   printf("Vi: %-20s %ld  %s \n",link->dt->d_name,link->info.st_size, link->hash);

   link->next = head;
   head = link;	

}


int esDirectorio(const char *parent, char *name, struct dirent *dt) {    // Retorna true si el archivo que le pasan es un directorio distinto del actual 
    struct stat st_buf;
    if (!strcmp(".", name) || !strcmp("..", name))
        return 0;

    char *path = alloca(strlen(name) + strlen(parent) + 2);
    sprintf(path, "%s/%s", parent, name);                          // Esta funcion sprintf pega la ruta actual del directorio mas el nombre del archivo 
    stat(path, &st_buf);
    return S_ISDIR(st_buf.st_mode);
}


void *verifacion( void *ptr) { 

    //printf("Bloqueando Llave\n");
    pthread_mutex_lock(&key);
    //printf("Llave Bloqueada\n");
    pool[j].visitado=1;
    const char *name = ((char*)ptr);
    //printf("Directorio a revisar %s\n",name);                                        // Verifica si es directorio o archivo 
    struct stat info;
    DIR *dir = opendir(name);
    struct dirent *ent;

    while (ent = readdir(dir)){
            stat(ent->d_name, &info);
            char *entry_name = ent->d_name;

        if (esDirectorio(name, entry_name, ent)){                           // Si es un directorio lo agrega al pool de directorios a visitar
            char *next = malloc(strlen(name) + strlen(entry_name) + 2);
            sprintf(next, "%s/%s", name, entry_name);
            pool[count].path = malloc(strlen(next));
            pool[count].path = next;
            pool[count].visitado = 0;
            //printf("Directorio obtenido %s\n", pool[count].path);
            count++;


        }else if((ent->d_type == DT_REG)&&(info.st_size>0)){              // Si es un archivo calcula el hash md5
          
        	if ( strcmp( mode, "e" )==0 ){
          // Hash md5 en modo binario 
            strcpy(md5,  "./md5 ");
            strcat(md5, ent->d_name);
            fp = popen(md5,"r");
            fgets(readbuffer,32,fp);
            insertVisitado(info,ent,readbuffer);
          // Hash md5 en modo binario 
        	}else if ( strcmp( mode, "l" )==0 ) {
          // Hash md5 en modo libreria
			strcat(md5, ent->d_name);
			int MDFile( char *md5, char readbuffer[33]);               // Funciona pero no guarda los hashes aun
            insertVisitado(info,ent,readbuffer);
          // Hash md5 en modo libreria
        	} else {
        		printf("modo incompatible\n");
        		return 0;
        	}
        }
    }
    closedir(dir);
    pthread_mutex_unlock(&key);
    //printf("Llave desbloqueada\n\n");
}

/*void printstruct(){
    for (int i = 0; i < count; i++){
         printf("Nombre: %s visitado: %d\n", pool[i].name, pool[i].visitado);
    }
}*/

//Verifica si hay duplicado e imprime
void verificar_duplicados() {
	link2 = head;
	while( link2 != NULL ){
		link2->hash = verhash;
		current = head;
		while( current != NULL ){
			if( current->hash == verhash ){
				printf("oh pol dio se repitio \n");    //INCOMPLETO :s falta poner que cuando se repite imprima las cosas y una verificacion de cuando ea con el mismo 
			}
			current = current->next;
		}
		link2 = link2->next;
	}

}
//Verifica si hay duplicado e imprime



int main(int argc, char *argv[]){

    
    int i;
    numhilos = argv[2];
    rutaini = argv[4];
    mode = argv[6];
    printf("%s\n",mode );
    pthread_t threads[50];
    system("make");  
    for ( i = 0; i < 40; ++i){
        pool[i].path=NULL;
    }
    ///home/chacon/Desktop
    verifacion(rutaini);
    

    for ( i=0; i<50; i++ ){
      
      while(1){
        if(pool[j].path==NULL){
          printf("No hay directorios\n");
          //Verifica si hay duplicado
		//verificar_duplicados();

          //Verifica si hay duplicado
          return 0;
        }else if(pool[j].visitado==0){
          //printf("visitado = 0\n");
          //printf("A visitar: %s\n", pool[j].path);  
          pthread_create(&threads[i], NULL, verifacion,(void *)pool[j].path);
          pthread_join(threads[i],NULL);
          //sleep(1);
        }else if(pool[j].visitado==1){
          //printf("visitado = 1\n");
          j++;
        }
      } 
        // sleep(1);
    }
      
    pthread_mutex_init(&key,NULL);
    
   // printstruct();
    return 0;
}
