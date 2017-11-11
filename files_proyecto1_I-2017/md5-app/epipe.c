// C program to demonstrate use of fork() and pipe()
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/types.h>
#include<string.h>
#include<sys/wait.h>
 
int main(){
    char *args[] = {"./md5", "mddriver.c", (char *) 0 };
    system("make");
    execv("./md5", args);
    //write(1,"ls\0",3);

    return 0;
}