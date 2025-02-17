/** (c) 2002 by Bhaskaran Raman and Regents of the University of California */
#ifndef DEBUG_H
#define DEBUG_H

#include <iostream>
#include <fstream>
#include <strstream>
#include <vector>
#include <sys/time.h>

#include "LogLine.h"
#include "JString.h"

/** Debug output/log package */

class Debug {
protected:
  ofstream fout, devnullout;
  char logFile[1024];
  unsigned short id;
  enum fn_type currFn;
  char buffer[2048]; // temporary buffer
  vector<JString> log;
  vector<LogLine_t> ll_list;
  ostrstream *sout;
  long lineCount;

  /* Check if limeCount limit has been reached.  Return true if limit
     has been reached, false otherwise. */
  bool checkLogLimit();

public:
  /** Should I use files directly? */
  bool to_file;
  /** Should I use stream to print? */
  bool use_stream;

  /** Have I already been closed? */
  bool closed;

  /** This is start-time at the timeSyncServer */
  struct timeval startTime;

  /** Give the log-file to use, the program id (across machines, or
      across programs running on the same machine).  The 'startTime'
      is time w.r.t. which the time parameter should be printed */
  Debug(const char *arg_logFile, unsigned short arg_id,
	struct timeval arg_startTime, bool arg_to_file, bool arg_use_stream);

  /** Set the current function name for printing */
  void setCurrFn(enum fn_type fn);

  /** Print the time, and the id, and return the ostream reference */
  ostream &print();

  /** Add the given log-line to the list */
  void addLL(LogLine_t &ll);

  /** Close the output stream */
  void close();

  /** The destructor */
  ~Debug();

}; // End class Debug

typedef class Debug Debug_t;

#endif
