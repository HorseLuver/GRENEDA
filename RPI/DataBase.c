/*
  Date: Feb 21, 2017
  DataBase code to extract data from DataBase folder
  Excepts one or two date parameters 
  Extracts data based on the parameters and writes into CSV file
  output -> output.csv
*/

#include <stdio.h>
#include <stdlib.h>   // for system call: system(commmad)
#include <time.h>
#include <string.h>
#include <ctype.h>


///////////////////////////////////////////
//                                       //
//            DECLARATION                //
//                                       //                   
///////////////////////////////////////////

#define DB_PATH "DataBase"
#define SMFR_PATH "Semaphore"

// Function that returs a semaphore value from 
int get_semaphore();

// Structire to hold parse time data
struct Date {
  int Y1, Y2, Y3, Y4; 
  int M1, M2; 
  int D1, D2; 
};

// Function that parse a string Date into Data structure
void parseTime(struct Date*, char* );

// Check the validity of a string
int validityCheck (const char* str);

// Checks command line arguments
int checkArduments(FILE*, int, char**);

// Builds regex search query based on Data structure
void buildRegex (char*, struct Date*, struct Date*, int);


///////////////////////////////////////////
//                                       //
//                  MAIN                 //
//                                       //                   
///////////////////////////////////////////

int main (int argc, char *argv[])
{
  struct Date D1, D2;                         // Structure to hold date digits
  int semaphore = -2;                         // Sensor DB semaphore
  char crit_sec[3][10];                       // Critical section to be avoided
  char quiry[100], regex[100];                    // System call quiry
  FILE* out_file = fopen("output.csv", "w");  // File for output data storage
  FILE* temp_file;                            // File to read quiry output file names

  // Checking arguments provided by the user
  if(checkArduments(out_file, argc, argv) == 1) return 0;
  
  // get semaphore file and calculate critical section
  semaphore = get_semaphore();
  sprintf(&crit_sec[0][0], "%06d", semaphore-1);
  sprintf(&crit_sec[1][0], "%06d", semaphore);
  sprintf(&crit_sec[2][0], "%06d", semaphore+1);

  // Parsing and second arguments
  parseTime(&D1, argv[1]);
  if(argc == 3) parseTime(&D2, argv[2]);

  printf("%d%d%d%d_%d%d_%d%d\n", D1.Y1, D1.Y2, D1.Y3, D1.Y4, D1.M1, D1.M2, D1.D1, D1.D2);

  buildRegex(regex, &D1, &D2, 1);

  sprintf(quiry, "ls DataBase | grep -v \"%s\\|%s\\|%s.*\" | grep %s > temp", crit_sec[0], crit_sec[1], crit_sec[2], regex);
  printf("%s\n", quiry);

  //sprintf(quiry, "ls DataBase | grep -v \"%s\\|%s\\|%s.*\" | grep \".*_2017_0[78]_[02][91]_.*\\.dat$\" > temp", crit_sec[0], crit_sec[1], crit_sec[2]);
  
  system(quiry);

  return 0;
}


///////////////////////////////////////////
//                                       //
//              FUNCTIONS                //
//                                       //                   
///////////////////////////////////////////

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

/*
    Check the validity of a string
    0 if valid Date - YYYY_MM_DD
    1 if not validity
*/
int validityCheck (const char* _str)
{
  int i = 0;        // Variable to count tokens
  char temp[50], str[100];

  // Buffer overflow
  if(strlen(_str) > 100) return 1;
  // Copy to preserver original string
  strcpy(str, _str);
  // Parsing YYYY_MM_DD
  char* token = strtok(str, "_");
  for(; token != NULL; i++, token = strtok(NULL, "_"))
  {
    sprintf(temp, "%s", token);           // Printing to temporary container
    for(int j = 0; j < strlen(temp); j++) // Loop to determine if data in a string is valid
    {
      if(!isdigit((int)temp[j])) return 1;
    }
  }
  if(i != 3) return 1;
  return 0;
}

/*
  Check to see if command line arguments have a proper format
  When wrong format detected, output file generated with error string
*/
int checkArduments(FILE* out_file, int count, char** arg)
{
  // Checking arguments provided by the user
  if(count == 1) 
  {
    // Exiting execution if date parameters are not provided
    fprintf(out_file, "FAILED, Not enought arguments provided\n");
    fclose(out_file);
    return 1;
  }
  else
  {
    // Checking Fist parameter
    if(validityCheck(arg[1]))
    {
      fprintf(out_file, "FAILED, First argument wrong format\n");
      fclose(out_file);
      return 1;
    }
    // Checking Second parameter
    if(count > 2)
    {
      if(validityCheck(arg[2]))
      {
        fprintf(out_file, "FAILED, Second argument wrong format\n");
        fclose(out_file);
        return 1;
      }
    }
  }
  return 0;
}

/*
  Function that parse a string Date into Data structure
  String format = YYYY_MM_DD
*/
void parseTime(struct Date *date, char* str)
{
  int year = 0, month = 0, day = 0;

  sscanf(str, "%d_%d_%d", &year, &month, &day);

  // Extracting Year
  date->Y4 = year%10;
  date->Y3 = year/10%10;
  date->Y2 = year/100%10;
  date->Y1 = year/1000%10;
  // Extracting Month
  date->M2 = month%10;
  date->M1 = month/10%10;
  // Extracting Day
  date->D2 = day%10;
  date->D1 = day/10%10;

}

/*
  Build a query based on Date parameters provided
  if number of dates = 1 used only Date 1 structure
  returns a pointer to a string search query
*/
void buildRegex (char* output, struct Date* D1, struct Date* D2, int dates)
{
  if(dates == 1)
  {
    sprintf(output, "\".*_%d%d%d%d_%d%d_%d%d_.*\\.dat$\"", D1->Y1, D1->Y2, D1->Y3, D1->Y4, D1->M1, D1->M2, D1->D1, D1->D2);
    // grep \".*_2017_0[78]_[02][91]_.*\\.dat$\" > temp", 
  }
  else
  {

  }
}
