/** (c) 2002 by Bhaskaran Raman and Regents of the University of California */
#include <iostream>
#include <netdb.h>

#include "Util.h"

int main(int argc, char *argv[]) {
  cerr << "myIP:" << ip2str(Cfg.myIP) << endl;
  if(argc != 4) {
    cerr << "Usage: " << argv[0] << " <ip-address> <string-to-split> <file-to-check>" << endl;
    exit(1);
  }
  char *hostName = argv[1];
  char *toSplit = argv[2];
  char *fileToCheck = argv[3];
  struct in_addr ipAddr = nslookup(hostName);
  cerr << hostName << ":\t" << ip2str(ipAddr) << endl;

  vector<sc_node_t> sc_list;
  unsigned short mySCID =
    util_readSCIDMapping(Cfg.scid_mapping_file, sc_list);
  cerr << "mySCID=" << mySCID << endl;
  for(unsigned int i = 0; i < sc_list.size(); i++) {
    cerr << "scid=" << sc_list[i].scid << "\t"
	 << "IP=" << ip2str(sc_list[i].ipAddr) << endl;
  }

  vector<JString> sVec;
  int numFields = split(toSplit, sVec);
  for(int i = 0; i < numFields; i++) {
    cerr << "sVec[" << i << "]=[" << sVec[i] << ']' << endl;
  }

  JString field = get_field(toSplit, 1);
  cerr << "field=[" << field << "]" << endl;

  vector<char*> char_pVec;
  numFields = str_split_mutate(toSplit, char_pVec);
  for(int i = 0; i < numFields; i++) {
    cerr << "char_pVec[" << i << "]=[" << char_pVec[i] << ']' << endl;
  }

  if(fileExists(fileToCheck)) {
    cerr << "file \"" << fileToCheck << "\" exists\n";
  } else {
    cerr << "file \"" << fileToCheck << "\" does not exist\n";
  }

  cerr << "sizeof(fd_set)=" << sizeof(fd_set) << endl;
} // End main()
