#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
//#include <asm/io.h>

// Defenitions
#define MAX_DATA 9155
#define CREATE  5000
#define SEMAPHORE 702
#define DB_PATH "DataBase"
#define SMFR_PATH "Semaphore"
#define TIME_INTERVAL 60

// Functions
void updateDate (char* date);
void fileWrite (char* path, char* data);

// MAIN
int main ()
{
    char file_name[100]; 
    char date[] = "2018-03-04_16:10:30";

    /* Copying a picture using proper name formatting*/
    for (int rec_num = 0; rec_num < CREATE; rec_num++)
    {
        updateDate(date);
        sprintf(file_name, "cp Windmill.jpg %s/%06d_%s.jpg", DB_PATH, rec_num, date);
        //printf("%s\n", file_name);
        system(file_name);
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
        day -= 29;
        month ++;
    }
    if(month >= 12){
        month -= 12;
        year++;
    }
    sprintf(date, "%04d_%02d_%02d_%02d:%02d:%02d", year, month, day, hour, min, sec);
}

