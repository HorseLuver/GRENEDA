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

#define DB_PATH   "DataBase"
#define SMFR_PATH "Semaphore"

#define Y1  0
#define Y2  1
#define Y3  2
#define Y4  3
#define M1  4
#define M2  5
#define D1  6
#define D2  7

// Function that returs a semaphore value from 
int get_semaphore();

// Function that parse a string Date into Data structure
void parseTime(int*, char* );

// Check the validity of a string
int validityCheck (const char* str);

// Checks command line arguments
int checkArduments(FILE*, int, char**);

// Builds regex search query based on Data structure
void buildRegex (char*, int*, int*, int);

// Function used internally to create a range string
void rangeString (char*, int, int);

// Rewriting Database files into CSV
int rewrite(FILE* );

///////////////////////////////////////////
//                                       //
//                  MAIN                 //
//                                       //                   
///////////////////////////////////////////

int main (int argc, char *argv[])
{
  int d1[8], d2[8];                           // Array to hold date digits
  int semaphore = -2;                         // Sensor DB semaphore
  char crit_sec[3][10];                       // Critical section to be avoided
  char quiry[100], regex[100];                // System call quiry
  FILE* out_file = fopen("output.csv", "w");  // File for output data storage
  FILE* temp_file;                            // File to read quiry output file names

  // Checking arguments provided by the user
  if(checkArduments(out_file, argc, argv) == 1) return 0;
  
  // get semaphore file and calculate critical section
  semaphore = get_semaphore();
  sprintf(&crit_sec[0][0], "%06d", semaphore-1);
  sprintf(&crit_sec[1][0], "%06d", semaphore);
  sprintf(&crit_sec[2][0], "%06d", semaphore+1);

  // Parsing first and second arguments
  parseTime(d1, argv[1]);
  if(argc == 3) parseTime(d2, argv[2]);

  // Generating Regular expression based on argumenets
  if(argc == 2) buildRegex(regex, d1, d2, 1);
  else if(argc == 3) buildRegex(regex, d1, d2, 2);
  
  // Printing the full search query
  sprintf(quiry, "ls DataBase | grep -v \"%s\\|%s\\|%s.*\" | grep %s > list", crit_sec[0], crit_sec[1], crit_sec[2], regex);
  printf("%s\n", quiry);

  // System call for the search queiry
  system(quiry);

  // Rewriting results into CSV file
  rewrite(out_file);

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
  char query[50];
  
  sprintf(query, "ls %s > temp.txt", SMFR_PATH);
  system(query);                      // Pipe into a file
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
void parseTime(int* date, char* str)
{
  int year = 0, month = 0, day = 0;

  sscanf(str, "%d_%d_%d", &year, &month, &day);

  // Extracting Year
  date[Y4] = year%10;
  date[Y3] = year/10%10;
  date[Y2] = year/100%10;
  date[Y1] = year/1000%10;
  // Extracting Month
  date[M2] = month%10;
  date[M1] = month/10%10;
  // Extracting Day
  date[D2] = day%10;
  date[D1] = day/10%10;
}

/*
  Build a query based on Date parameters provided
  if number of dates = 1 used only Date 1 structure
  returns a pointer to a string search query
*/
void buildRegex (char* output, int* d1, int* d2, int two_dates)
{
  char r[8][10];   // Container to hold range parameres
  // Two date arguments provided
  if(two_dates == 1)
  {
    sprintf(output, "\".*_%d%d%d%d_%d%d_%d%d_.*\\.dat$\"", d1[Y1], d1[Y2], d1[Y3], d1[Y4], d1[M1], d1[M2], d1[D1], d1[D2]);
    // grep \".*_2017_0[78]_[02][91]_.*\\.dat$\" > temp", 
  }
  else // One date arguments provided
  {
    // Generating range string [1-9]
    for(int i=0; i < 8; i++) rangeString(r[i], d1[i], d2[i]);
    sprintf(output, "\".*_%s%s%s%s_%s%s_%s%s_.*\\.dat$\"", r[Y1], r[Y2], r[Y3], r[Y4], r[M1], r[M2], r[D1], r[D2]);
  }
}

/*
  Function used internally to create a range string
  Input [int] 4, 2 -> Output [char] 2-4
*/
void rangeString (char* output, int n1, int n2)
{
  if(n1 == n2) sprintf(output, "%01d", n1);
  else if (n1 > n2) sprintf(output, "[%01d-%01d]", n2, n1);
  else if (n2 > n1) sprintf(output, "[%01d-%01d]", n1, n2);
}

/*
  Rewriting Database files pointed by [temp] into output CSV
  Printing any errors, and closing the file
  Success = 0, Failure = 1
*/
int rewrite(FILE* csv)
{
  char buff[200], name[50], full_name[50], dateTime[50];
  float s[6] = {0, 0, 0, 0, 0, 0};

  FILE* db_file;
  FILE* list = fopen("list", "r");         // File pointer to list of file names
  // Error checking
  if(list == NULL)
  {
    fprintf(csv, "FAILED,count file was not created\n");
    fclose(csv);
    return 1;     // Finish Execution
  }
  // Loop to extract data
  while(fgets(name, sizeof(name), list) != NULL)
  {
    // Getting file Name
    name[strlen(name) - 1] = '\0';    // Getting rid extra newline charater
    sprintf(full_name, "%s/%s", DB_PATH, name);
    // Openning file for rewriting
    db_file = fopen(full_name, "r");
    if(db_file != NULL)
    {
      fscanf(db_file, "%s %f %f %f %f %f %f", dateTime, &s[0], &s[1], &s[2], &s[3], &s[4], &s[5]);
      fprintf(csv, "%s,%f,%f,%f,%f,%f,%f\n", dateTime, s[0], s[1], s[2], s[3], s[4], s[5]);
      fclose(db_file);
    }
  }
  // Closing files
  fclose(list);
  fclose(csv);
  return 0;
}