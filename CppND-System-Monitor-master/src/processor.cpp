#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
 float percent;
  long TotalJ = LinuxParser::Jiffies();
  long ActiveJ = LinuxParser::ActiveJiffies();
  long IdleJ = LinuxParser::IdleJiffies();
   percent = ((float) TotalJ - (float) IdleJ) / (float) TotalJ;
  return percent;
}