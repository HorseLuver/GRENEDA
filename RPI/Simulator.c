#include <stdio.h>
#include <time.h>
#include <string.h>
//#include <asm/io.h>

// Defenitions
#define MAX_DATA 341796
#define CREATE  50
#define FILE_PATH "DataBase"
#define SMFR_PATH "Semaphore"
#define TIME_INTERVAL 30

// Functions
void updateDate (char* date);
void fileWrite (char* path, char* data);

// MAIN
int main ()
{
    char file_name[27];
    char date[] = "2017_07_28_15:30:23";

    for(int rec_num = 0; rec_num < CREATE; rec_num++)
    {
        sprintf(file_name, "%s/%06d_%s.dat", FILE_PATH, rec_num, date);
        fileWrite(file_name, "0");
        updateDate(date);
    }

    return 0;
}

/*
    Incrementing date-time and rewriting the string [date]
*/
void updateDate (char* date)
{
    int year, month, day, hour, min, sec;

    sscanf(date, "%d_%d_%d_%d:%d:%d", &year, &month, &day, &hour, &min, &sec);
    sec += TIME_INTERVAL;
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
        day ++;
    }
    if(day > 29) {
        day -= 7;
        month ++;
    }
    if(month >= 12){
        month -= 12;
        year++;
    }
    sprintf(date, "%04d_%02d_%02d_%02d:%02d:%02d", year, month, day, hour, min, sec);
}

/*
    Writing [data] into a file specified by [path]
*/
void fileWrite (char* path, char* data)
{
    FILE* file = fopen(path,"w");
    
    
    fclose(file);
}

