#include <string>
#include <iostream>  
#include <sstream> 
#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
     // OUTPUT: HH:MM:SS   

// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long seconds) {
   int hours = 0;
   int mins = 0;
  
    hours = seconds / 3600; 
    seconds = seconds % 3600;
    mins = seconds / 60;
    mins = mins % 60;
  
  string h = std::to_string(hours);
  string m = std::to_string(mins);
  string s = std::to_string(seconds);
  
  string line = h + ":" + m + ":" + s;
  return line; 
}