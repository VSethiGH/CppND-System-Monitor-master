#include <string>
#include <iostream>  
#include <sstream> 
#include "format.h"

using std::string;

// TODO: Complete this helper function
// INPUT: Long int measuring seconds
     // OUTPUT: HH:MM:SS   

// REMOVE: [[maybe_unused]] once you define the function
string Format::ElapsedTime(long int seconds) {
   long hours;
   long mins;
  
    hours = seconds / 3600; 
    seconds = seconds % 3600;
    mins = seconds / 60;
    mins = mins % 60;
  seconds = seconds % 60;
  
  string h = std::to_string(hours);
   h.insert(0, 2 - h.length(), '0');
  
  string m = std::to_string(mins);
   m.insert(0, 2 - m.length(), '0');
  
  string s = std::to_string(seconds);
  s.insert(0, 2 - s.length(), '0');
  
  string line = h + ":" + m + ":" + s;
  return line; 
}