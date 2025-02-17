/** (c) 2002 by Bhaskaran Raman and Regents of the University of California */
/** Implements Debug.h */

#include <string.h>

#include "Debug.h"
#include "Util.h"
#include "Config.h"

Debug::Debug(const char *arg_logFile, unsigned short arg_id,
	     struct timeval arg_startTime, bool arg_to_file,
	     bool arg_use_stream) {

  strcpy(logFile, arg_logFile);

  if(fileExists(logFile) && (strcmp(logFile, "/dev/null") != 0)) {
    cerr << "Debug: logFile \"" << logFile << "\" exists\n";
    exit(25);
  }

  fout.open(logFile);
  if(fout.fail()) {
    cerr << "Debug: can't open " << logFile << endl;
    exit(23);
  }

  devnullout.open("/dev/null");
  if(devnullout.fail()) {
    cerr << "Debug: can't open /dev/null\n";
    exit(24);
  }

  id = arg_id;			startTime = arg_startTime;
  to_file = arg_to_file;	use_stream = arg_use_stream;
  currFn = INVALID_FN;		sout = NULL;
  closed = false;

  if(use_stream) {
    if(to_file) {
      fout << (long)(now(startTime)) << ' ' << id << " Debug"
	   << " start" << endl;
    } else {
      sout = new ostrstream(buffer, sizeof(buffer));
      if(sout == NULL) {
	cerr << id << ": Can't alloc for sout" << endl;
	exit(24);
      }
      (*sout) << (long)(now(startTime)) << ' ' << id << " Debug"
	      << " start" << endl;
    }
  } else {
    LogLine_t ll; ll.fn = DEBUG_FN; ll.scid = id;
    ll.time = (long)(now(startTime)); ll.desc = start_desc;
    if(to_file) { fout << ll.printValid() << endl; }
    else { ll_list.push_back(ll); }
  }

  lineCount = 1;

} // End Debug::Debug()

void Debug::setCurrFn(enum fn_type fn) {
  currFn = fn;
} // End Debug:;setCurrFn()

bool Debug::checkLogLimit() {
  if(Cfg.logLimit <= 0) { return false; } // no limits imposed
  if(lineCount < (Cfg.logLimit-1)) { return false; } // limit not reached
  if(lineCount >= Cfg.logLimit) { return true; } // already past limit
  // lineCount should now be Cfg.logLimit-1
  lineCount++;
  currFn = DEBUG_FN;

  if(use_stream) {
    if(to_file) {
      fout << (long)(now(startTime)) << ' ' << id << ' '
	   << LogLine::get_fn_name(currFn) << " log-limit" << endl;
    } else {
      // store the previous string
      buffer[sout->pcount()] = 0;
      log.push_back(buffer);
      // create new strstream
      delete sout;
      sout = new ostrstream(buffer, sizeof(buffer));
      if(sout == NULL) {
	cerr << id << ": Can't alloc for sout" << endl;
	close(); exit(25);
      }
      // print prefix before returning
      (*sout) << (long)(now(startTime)) << ' ' << id << ' '
	      << LogLine::get_fn_name(currFn) << " log-limit" << endl;
    }

    // End case if(use_stream)
  } else {
    LogLine_t ll; ll.time = (long)(now(startTime)); ll.scid = id;
    ll.fn = currFn; ll.desc = log_limit_desc;
    if(to_file) { fout << ll.printValid() << endl; }
    else { ll_list.push_back(ll); }

    // End else of if(use_stream)
  }

  return true;
} // End Debug::checkLogLimit

ostream &Debug::print() {
  if(!use_stream) {
    // why am i being called?
    cerr << id << ": using stream without setting use_stream\a\n";
    close(); exit(24);
  }

  if(checkLogLimit()) { return devnullout; } // lineCount limit reached?

  lineCount++;

  if(to_file) {
    fout << (long)(now(startTime)) << ' ' << id << ' '
	 << LogLine::get_fn_name(currFn) << ' ';
    return fout;
  }

  // store the previous string
  buffer[sout->pcount()] = 0;
  log.push_back(buffer);

  // create new strstream
  delete sout;
  sout = new ostrstream(buffer, sizeof(buffer));
  if(sout == NULL) {
    cerr << id << ": Can't alloc for sout" << endl;
    close(); exit(25);
  }

  // print prefix before returning
  (*sout) << (long)(now(startTime)) << ' ' << id << ' '
	  << LogLine::get_fn_name(currFn) << ' ';

  return (*sout);
} // End Debug::print()

void Debug::addLL(LogLine_t &ll) {
  if(use_stream) {
    // why am i being called?
    cerr << id << ": using log-line with use_stream set\a\n";
    close(); exit(24);
  }
  if(checkLogLimit()) { return; } // lineCount limit reached?
  lineCount++;
  ll.time = (long)(now(startTime)); ll.scid = id; ll.fn = currFn;
  if(to_file) { fout << ll.printValid() << endl; }
  else { ll_list.push_back(ll); }
} // End Debug::addLL()

void Debug::close() {
  if(closed) { return; }

  if(!to_file) {
    if(use_stream) {
      // store the previous string
      buffer[sout->pcount()] = 0;
      log.push_back(buffer);
      // done with strstreams
      delete sout;
    }

    // write onto file
    if(use_stream) {
      for(unsigned int i = 0; i < log.size(); i++) {
	fout << log[i];
      }
    } else {
      for(unsigned int i = 0; i < ll_list.size(); i++) {
	fout << ll_list[i].printValid() << endl;
      }
    }
  }

  fout.flush();
  fout.close();

  closed = true;
} // End Debug::close()

Debug::~Debug() {
  close();
} // End Debug:;~Debug()
