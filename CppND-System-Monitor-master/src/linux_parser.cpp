#include <dirent.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, kernel, version;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() {  
   float free;
   float total;
   float ava;
   string val;
   string line;	
   string key;
 
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  if (filestream.is_open()) 
  {
    while (std::getline(filestream, line)) 
    {
      std::istringstream linestream(line);
      while (linestream >> key >> val) 
      {
        if (key == "memFree") 
        {
           free = std::stof(val);
        }
        else if(key == "memTotal") 
        {          
           total = std::stof(val);
        }
        else if(key == "memAvailable") 
        {          
           ava = std::stof(val);
        }
      }
    }
  }
  return (ava - free) / total; }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  int long time = 0;
  string val;
  string line;
  
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) 
  {
    while(std::getline(stream, line))
    {
    std::istringstream linestream(line);
    linestream >> val;
    }
  }
  time = std::stol(val);
  return time; }

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() 
{
  long User, Nice, System, Idle, IOwait, IRQ, SoftIRQ, Steal, Guest, GuestNice, Total;
  string line;
  
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) 
  {
    while (std::getline(filestream, line)) 
    {
     std::istringstream linestream(line);
     linestream >> User >> Nice >> System >> Idle >> IOwait >> IRQ >> SoftIRQ >> Steal >> Guest >> GuestNice;
     Total =  User + Nice + System + Idle + IOwait + IRQ + SoftIRQ + Steal + Guest + GuestNice;
    }
  }
  return Total; 
}

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid) 
{ 
   long utime, stime, cutime, cstime, total;
   int value = 1;
   string line;	
   string key;
   string p = to_string(pid);
  
  std::ifstream filestream(kProcDirectory + p + kStatFilename);
  if (filestream.is_open()) 
  {
    std::istringstream linestream(line);
    while(linestream >> key)
    {
      if(value == 14) 
        {
         utime = std::stol(key); 
        }
      if(value == 15) 
        {
         stime = std::stol(key); 
        }
      if(value == 16) 
        {
         cutime = std::stol(key); 
        }
      if(value == 17) 
        {
         cstime = std::stol(key); 
        }
		value++;
    }
  }
  total = utime + stime + cutime + cstime;
  return total; 
}

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies()
{ 
 auto j = CpuUtilization();
 return stol(j[CPUStates::kUser_]) + stol(j[CPUStates::kNice_]) + stol(j[CPUStates::kSystem_]) + stol(j[CPUStates::kIRQ_]) +  stol(j[CPUStates::kSoftIRQ_]) + stol(j[CPUStates::kSteal_]);
  
//  return 0.0;
  
}

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() 
{
  auto j = CpuUtilization();
  return  stol(j[CPUStates::kIdle_]) + stol(j[CPUStates::kIOwait_]); 
}

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() 
{
   vector<string> cpu;
   string val;
   string line;	
   string key;
 
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) 
  {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> key;
    while(linestream >> val)
    {
		cpu.push_back(val);
    }
  }
  return cpu; 
}

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses()
{
   int total = 0;
   string val;
   string line;	
   string key;
 
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) 
  {
    while (std::getline(filestream, line)) 
    {
      std::istringstream linestream(line);
      while(linestream >> key >> val)
      {
        if (key == "processes") 
        {
           total = std::stof(val);
        }
      }
    }
  }
  return total; 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() 
{ 
   int runP = 0;
   string val;
   string line;	
   string key;
 
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) 
  {
    while (std::getline(filestream, line)) 
    {
      std::istringstream linestream(line);
      while(linestream >> key >> val)
      {
        if (key == "proc_running") 
        {
           runP = std::stof(val);
        }
      }
    }
  }
  return runP; 
}

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid) 
{  
  string C;
  string line;	
  string p = to_string(pid);
  
  std::ifstream filestream(kProcDirectory + p + kCmdlineFilename);
  if (filestream.is_open()) 
  {
    std::getline(filestream, line);
    std::istringstream linestream(line);
    linestream >> C;
  }
  return C;  
}

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid) 
{
  int ram = 0;
  string r;
  string p = to_string(pid);
  string line;	
  string val;
  string key;
  
  std::ifstream filestream(kProcDirectory + p + kStatusFilename);
  if (filestream.is_open()) 
  {
    while (std::getline(filestream, line)) 
    {
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream linestream(line);
    while(linestream >> key >> val)
      {
        if (key == "VmSize") 
        {
           ram = std::stoi(val) / 1024;
        }
      }
    }
  }
  r = std::to_string(ram);
  return r;
}

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid)
{ 
  string id;
  string p = to_string(pid);
  string line;	
  string val;
  string key;
  
  std::ifstream filestream(kProcDirectory + p + kStatusFilename);
  if (filestream.is_open()) 
  {
    while (std::getline(filestream, line)) 
    {
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream linestream(line);
    while(linestream >> key >> val)
      {
        if (key == "Uid")
        {
           id = val;
        }
      }
    }
  }
  return id;
}


// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid)
{ 
 string u;
 string line;
 string key;
 string x;
 string val;
  
 std::ifstream filestream(kPasswordPath);
 if (filestream.is_open()) 
  {
    while (std::getline(filestream, line)) 
    {
    std::replace(line.begin(), line.end(), ':', ' ');
    std::istringstream linestream(line);
    while(linestream >> key >> x >> val)
      {
        if (val == Uid(pid))
        {
           u = key;
        }
      }
    }
  }
 return u;
}

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid)
{ 
  long time;
  int value = 0;
  string p = to_string(pid);
  string line;	
  string key;
  
  std::ifstream filestream(kProcDirectory + p + kStatFilename);
  if (filestream.is_open()) 
  {
    while (std::getline(filestream, line)) 
    {
    std::istringstream linestream(line);
    while(linestream >> key)
      {
        if (value == 22)
        {
           time = std::stol(key);
        }
      value++;
      }
    }
  }
  time = (time) / sysconf(_SC_CLK_TCK);
  return time;
}
