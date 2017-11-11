#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <dirent.h>

struct node{
   struct stat info;
   int is_dir =0;
   struct dirent *dt;
   struct node next;
};

struct dir{
   struct stat info;
   int visitado = 0;
   struct dirent *dt;
   struct node vnext;
};

struct node *head = NULL;
struct node *current = NULL;
struct dir *vhead = NULL;
struct dir *vcurrent = NULL;

void insertAvisitar(struct stat info, int is_dir, struct dirent *dt){
   //create a link
   struct node *link = (struct node*) malloc(sizeof(struct node));
   
   link->info = info;
   link->is_dir = is_dir;
   link->dt = dt;

   link->next = head;
   head = link;
}

void insertVisitado(struct stat info, int is_dir, struct dirent *dt){
   //create a link
   struct dir *vlink = (struct dir*) malloc(sizeof(struct dir));
   
   vlink->info = info;
   vlink->is_dir = is_dir;
   vlink->dt = dt;

   vlink->vnext = vhead;
   vhead = vlink;
}

struct node* find(struct stat info) {

   //start from the first link
   struct node* current = head;

   //if list is empty
   if(head == NULL) {
      return NULL;
   }

   //navigate through list
   while(current->next != NULL) {
   
      //if it is last node
      if(S_ISDIR(info.st_mode)&&(current->is_dir==0)) {
         return current;
      } else {
         //go to next link
         current = current->next;
      }
   }      
}

bool isEmpty(){
   return head == NULL;
}





