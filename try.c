#include <stdio.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>


int count=1;


struct directoy{
    char *name;
    int visitado;
};

struct directoy pool[40];

struct node{
   struct stat info;
   struct dirent *dt;
   struct node *next;
};

struct dir{
   struct stat info;
   int is_dir;
   int visitado;
   struct dirent *dt;
   struct dir *vnext;
};


struct node *head = NULL;
struct node *current = NULL;
struct dir *vhead = NULL;
struct dir *vcurrent = NULL;

void insertVisitado(struct stat info, struct dirent *dt){
   //create a link
   struct node *link = (struct node*) malloc(sizeof(struct node));
   
   link->info = info;
   link->dt = dt;
   printf("Vi: %-20s %ld \n",link->dt->d_name,link->info.st_size);

   link->next = head;
   head = link;
}

void insertAvisitar(struct stat info, int is_dir, struct dirent *dt){
   //create a link
   struct dir *vlink = (struct dir*) malloc(sizeof(struct dir));
   
   vlink->info = info;
   vlink->is_dir = is_dir;
   vlink->dt = dt;

  // printf("Av: %-20s %d %u \n",vlink->dt->d_name,vlink->is_dir,vlink->dt->d_type);

   vlink->vnext = vhead;
   vhead = vlink;
}

int is_directory_we_want_to_list(const char *parent, char *name, struct dirent *dt) {
    struct stat st_buf;
    if (!strcmp(".", name) || !strcmp("..", name))
        return 0;

    char *path = alloca(strlen(name) + strlen(parent) + 2);
    sprintf(path, "%s/%s", parent, name);
    stat(path, &st_buf);

    if (S_ISDIR(st_buf.st_mode)){
        insertAvisitar(st_buf,1,dt);
        return S_ISDIR(st_buf.st_mode);
    }/*else if(S_ISLNK(st_buf.st_mode)){
        return S_ISLNK(st_buf.st_mode);
    }else{       
        insertAvisitar(st_buf,0,dt);
        return S_ISDIR(st_buf.st_mode);
    }*/

        return 0;
}


int list(const char *name) {
    struct stat info;
    DIR *dir = opendir(name);
    struct dirent *ent;

    while (ent = readdir(dir)){
            stat(ent->d_name, &info);
            char *entry_name = ent->d_name;

        if (is_directory_we_want_to_list(name, entry_name, ent)){
            // You can consider using alloca instead.
            char *next = malloc(strlen(name) + strlen(entry_name) + 2);
            sprintf(next, "%s/%s", name, entry_name);
            pool[count].name = malloc(strlen(next));
            pool[count].name = next;
            pool[count].visitado = 0;
            count++;
        }else if((ent->d_type == DT_REG)&&(info.st_size>0)){
            printf("%s %ld \n",ent->d_name,info.st_size);
        }
    }
    closedir(dir);
}

void printstruct(){
    for (int i = 0; i < count; i++){
         printf("Nombre: %s visitado: %d\n", pool[i].name, pool[i].visitado);
    }
}
   


int main(int argc, char *argv[]){
      
    list("/home/roybert/Escritorio/Proyecto1/ArchivosDuplicados");
    

    //printstruct();
    return 0;
}