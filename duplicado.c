#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


int count=1;             // Contador global para ir agregando los directorios al pool
char    md5[10];    
char    name[20];        // Nombre del archivo al que se le realizara el hash
char    readbuffer[32];  // Contendra el hash 
FILE *fp;                // Descriptor para el pipe generado por popen


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


int verifacion(const char *name) {                                        // Verifica si es directorio o archivo 
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
            count++;

        }else if((ent->d_type == DT_REG)&&(info.st_size>0)){              // Si es un archivo calcula el hash md5
          // Hash md5 en modo binario 
            strcpy(md5,  "./md5 ");
            strcat(md5, ent->d_name);
            fp = popen(md5,"r");
            fgets(readbuffer,32,fp);
            insertVisitado(info,ent,readbuffer);
          // Hash md5 en modo binario 
        }
    }
    closedir(dir);
}

/*void printstruct(){
    for (int i = 0; i < count; i++){
         printf("Nombre: %s visitado: %d\n", pool[i].name, pool[i].visitado);
    }
}*/

int main(int argc, char *argv[]){
  
    system("make");    
    verifacion("/home/roybert/Escritorio/Proyecto1/ArchivosDuplicados/");
    
   // printstruct();
    return 0;
}