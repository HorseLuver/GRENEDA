#include <stdio.h>
#include <time.h>
#include <string.h>
#include <stdlib.h>
//#include <asm/io.h>

// Defenitions
#define MAX_DATA 341796
#define CREATE  100
#define FILE_PATH "DataBase"
#define SMFR_PATH "Semaphore"
#define TIME_INTERVAL 30

// Functions
void updateDate (char* date);
void fileWrite (char* path, char* data);

// MAIN
int main ()
{
    char file_name[50], data[150], temp[50];
    char date[] = "2017_07_28_15:30:23";
    float random;

    srand(time(NULL));         // should only be called once

    // Loop to create each record file
    for(int rec_num = 0; rec_num < CREATE; rec_num++)
    {
        // Printing Date
        strcpy(data, date);
        // Printing sensor readings S1 S2 S3 ...     
        for(int i = 0; i < 6; i++)      
        {
            random = (float)(rand() % 20000) / 1000;      
            sprintf(temp, " %.3f", random);
            strcat(data, temp);
        }
        // Getting file Name
        sprintf(file_name, "%s/%06d_%s.dat", FILE_PATH, rec_num, date);
        fileWrite(file_name, data);
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
        day -= 29;
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
    //fprintf(file, "%s", data);
    fputs(data, file);
    fclose(file);
}

