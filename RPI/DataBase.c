/*
  Date: Feb 18, 2017
    
*/


#include <stdio.h>
#include <stdlib.h>   // for system call: system(commmad)
#include <time.h>
#include <string.h>

#define DB_PATH "DataBase"
#define SMFR_PATH "Semaphore"

int get_semaphore();

int main ()
{
  int semaphore = -2;
  char crit_sec[3][10];
  char quiry[100];

  // get semaphore file and calculate critical section
  semaphore = get_semaphore();
  sprintf(&crit_sec[0][0], "%06d", semaphore-1);
  sprintf(&crit_sec[1][0], "%06d", semaphore);
  sprintf(&crit_sec[2][0], "%06d", semaphore+1);

  sprintf(quiry, "ls DataBase | grep -v \"%s\\|%s\\|%s.*\" | grep \".*_2017_0[78]_[02][91]_.*\\.dat$\" > output.txt", crit_sec[0], crit_sec[1], crit_sec[2]);

  system(quiry);

  return 0;
}

/*
    This function returns the value of a semaphore
    success: number located in Semaphore/xxxxxx 
    failure: -1
*/
int get_semaphore()
{
  FILE* file;
  int semaphore = -1;
 
  system("ls Semaphore > temp.txt");  // Pipe into a file
  file = fopen("temp.txt", "r+");     // Open the file
  if(file == NULL)
    return -1;
  fscanf(file, "%d", &semaphore);     // Read Value
  fclose(file);                       // Close File
  system("rm temp.txt");              // Delete the file
  return semaphore;                   // Return semaphore value
}

