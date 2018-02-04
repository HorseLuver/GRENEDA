#include <stdio.h>
#include <time.h>
#include <string.h>
//#include <asm/io.h>

#define MAX_DATA 341796
#define CREATE  50
#define FILE_PATH "DataBase"

int main ()
{
    char file_name[27];
    int rec_num, sec = 10, min = 30, hour = 15;
    char date[] = "2017_07_28";
    FILE* file;
    
    for(rec_num = 0; rec_num < CREATE; rec_num++)
    {
        sprintf(file_name, "%s/%06d_%s_%02d:%02d:%02d.dat", FILE_PATH, rec_num, date, hour, min, sec);
        file = fopen(file_name,"w");
        fclose(file);

        sec += 30;
        if(sec >= 60){
            sec -= 60;
            min ++;
        }
        if(min >= 60){
            min -= 60;
            hour ++;
        }
        if(hour >= 24){
            hour -= 24;
        }
    }

    return 0;
}