/*
DataMaker.cpp
  Description: Sets a provided number of elements of random numbers with a set minimum
  By: Mark Christenson
  Options:
    > None produces 4 random numbers with a min of 0
    > First numeric option dictates element Count
    > Second numeric option dictates minimum
    > Ex: "DataMaker 10000 500" develops 10,000 random numbers with the minimum
          number equal to 500

*/
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

using namespace std;

int main(int argc, char const *argv[]) {
  string filename = "Count_";
  int count = 4;
  int min = 0;
  ofstream output;
  if(argc>1){
    count = stoi(argv[1]);
  }//end if count is defined
  filename += to_string(count);
  srand(time(0));
  if(argc>2){
    min = stoi(argv[2]);
  }//end if min is defined
  filename += "_min_";
  filename += to_string(min);
  filename += ".csv";
  output.open(filename);
  for (size_t i = 0; i < count; i++) {
    int randomNumber = (rand() % 100) + min + 1;
    if(i<count-1 && i != count / 2){
      output << randomNumber;
      output << ",";
    }//end usual case
    else if (i == count - 1) {
      output << randomNumber;
    }//end if last element
    else{
      output << min;
      output << ",";
    }//end else
  }//end for loop
  return 0;
}//end main
