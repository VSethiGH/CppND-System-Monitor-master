#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "linux_parser.h"
#include "processor.h"

class Processor {
 public:
  float Utilization();  // TODO: See src/processor.cpp

  // TODO: Declare any necessary private members
 private:
  float util_;
};

#endif