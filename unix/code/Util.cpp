/** (c) 2002 by Bhaskaran Raman and Regents of the University of California */
/** Set of utility routines */

#include <stdio.h>
#include <fstream>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <netdb.h>
#include <ctype.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <math.h>

#include "Util.h"

int rand_int(double rand_01, int lower, int upper) {
  return lower + (int)(floor(rand_01*(upper-lower)));
} // End rand_int()

struct in_addr nslookup(const char *host) {
  struct hostent *hp = gethostbyname(host);
  if(hp == 0) {
    fprintf(stderr, "could not abtain address of %s\n", host);
    exit(250);
  }
  struct in_addr toRet;
  bcopy(hp->h_addr_list[0], (caddr_t)&toRet, hp->h_length);
  return toRet;
} // End nslookup()

int sock_and_bind(int port) {
  // socket for listening
  int listen_fd = socket(PF_INET, SOCK_DGRAM, IPPROTO_UDP);
  if(listen_fd < 0) {
    perror("sock_and_bind");
    return listen_fd;
  }

  // bind to given port
  struct sockaddr_in listen_addr;
  bzero((char *)&listen_addr, sizeof(listen_addr));
  listen_addr.sin_family =  AF_INET;
  listen_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  listen_addr.sin_port = htons(port);
  if(bind(listen_fd, (struct sockaddr *)&listen_addr,
	  sizeof(listen_addr)) < 0) {
    fprintf(stderr, "Can't bind to port %d\n", port);
    perror("bind failed");
    close(listen_fd);
    return -1;
  }
  return listen_fd;
} // End sock_and_bind()

int sock_and_connect(const char *host, int port) {
  return sock_and_connect(nslookup(host), port);
} // End sock_and_connect(const char *host, int port)

int sock_and_connect(struct in_addr server_ip, int port) {
  int sock_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(sock_fd < 0) { perror("socket failed"); return -1; }
  struct sockaddr_in serv_addr;

  /* connect */
  serv_addr.sin_addr.s_addr = server_ip.s_addr;
  serv_addr.sin_family = PF_INET;
  serv_addr.sin_port = htons(port);
  int res = connect(sock_fd, (struct sockaddr *)&serv_addr,
		    sizeof(serv_addr));
  if(res != 0) { perror("connect failed"); close(sock_fd); return -1; }
  return sock_fd;
} // End sock_and_connect()

int sock_and_listen(int port) {
  int sock_fd = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP);
  if(sock_fd < 0) { perror("socket failed"); return -1; }
  struct sockaddr_in serv_addr;

  /* bind, listen */
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(port);
  if(bind(sock_fd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    perror("bind"); close(sock_fd); return -1;
  }

  if(listen(sock_fd, SOMAXCONN) < 0) {
    perror("listen"); close(sock_fd); return -1;
  }
  return sock_fd;
} // End sock_and_listen()

double time2double(struct timeval t) {
  return t.tv_sec*1000.0 + t.tv_usec/1000.0;
} // End time2double()

double now(struct timeval start) {
  struct timeval t;
  gettimeofday(&t, NULL);
  return (t.tv_sec - start.tv_sec)*1000.0 +
    (t.tv_usec - start.tv_usec)/1000.0;
} // End now()

struct timeval past(unsigned long t) {
  struct timeval tv;
  gettimeofday(&tv, NULL);
  unsigned long t_sec = t/1000;
  unsigned long t_micros = (t%1000)*1000;
  tv.tv_sec -= t_sec;
  if(tv.tv_usec >= t_micros) {
    tv.tv_usec -= t_micros;
  } else {
    tv.tv_sec--;
    tv.tv_usec += (1000*1000 - t_micros);
  }
  return tv;
} // End past()

long int timestamp() {
  struct timeval t;
  gettimeofday(&t, NULL);
  return t.tv_sec;
} // End timestamp()

unsigned int ulongHash(const unsigned long &l) {
  register unsigned long key = l;
  // 2654435761 is the golden ratio of 2^32. The right shift of 3 bits
  // assumes that all objects are aligned on the 8 byte boundary. If a
  // system aligns on the 4 byte boundary, then a right shift of 2
  // bits should be done.
  return (key >> 3) * 2654435761U;
} // End ulongHash()

unsigned int ushortHash(const unsigned short &u) {
  return u;
} // End ushortHash()

unsigned int addrHash(const char_p &addr) {
  register unsigned long key;
  key = (unsigned long) addr;
  // 2654435761 is the golden ratio of 2^32. The right shift of 3 bits
  // assumes that all objects are aligned on the 8 byte boundary. If a
  // system aligns on the 4 byte boundary, then a right shift of 2
  // bits should be done.
  return (key >> 3) * 2654435761U;
} // End addrHash()

unsigned int JStringHash(const JString &s) {
  unsigned int h=0,g;
  int i; int len = s.length();
  for(i=0;i<len;i++) {
        h=(h<<4)+(s[i]);
        if((g=(h&0xf0000000))) {
          h=h^(g>>24);
          h=h^g;
        }
  }
  return h;
} // End JStringHash()

char *my_strcpy(char *dest, const char *src) {
  int i = 0;
  while(src[i]) { dest[i] = src[i]; i++; }
  dest[i] = 0;
  return (dest+i);
} // End my_strcpy()

char *longPrint(char *s, const long argval) {
  int slen = 0;
  long val = argval;
  if(argval < 0) { val = -argval; }
  for(;;) {
    s[slen] = '0' + (val%10);
    slen++;
    val = val/10;
    if(val == 0) break;
  }
  if(argval < 0) { s[slen] = '-'; slen++; }
  // reverse JString
  for(int i = 0; i < slen/2; i++) {
    char temp = s[i];
    s[i] = s[slen-i-1];
    s[slen-i-1] = temp;
  }
  s[slen] = 0;
  return (s+slen);
} // End longPrint()

JString long2str(const long argval) {
  char s[64]; longPrint(s, argval);
  return JString(s);
} // End long2str()

char *ipPrint(char *s, const struct in_addr ip) {
  unsigned long ipLong = ntohl(ip.s_addr);
  unsigned long fourth8 = ipLong & 0x000000ff;
  ipLong = ipLong >> 8;
  unsigned long third8 = ipLong & 0x000000ff;
  ipLong = ipLong >> 8;
  unsigned long second8 = ipLong & 0x000000ff;
  ipLong = ipLong >> 8;
  unsigned long first8 = ipLong & 0x000000ff;
  char *lastPos = s;
  lastPos = longPrint(lastPos, first8); *lastPos = '.'; lastPos++;
  lastPos = longPrint(lastPos, second8); *lastPos = '.'; lastPos++;
  lastPos = longPrint(lastPos, third8); *lastPos = '.'; lastPos++;
  lastPos = longPrint(lastPos, fourth8); *lastPos = 0; lastPos++;
  return lastPos;
} // End ipPrint()

JString ip2str(const struct in_addr ip) {
  char s[128]; ipPrint(s, ip);
  return JString(s);
} // End ip2str()

int JString_atoi(const JString &s) {
  return atoi(s.cstr());
} // End JString_atoi()

int str_split(const char *a_s, vector<JString> &vec) {
  int pos = 0;
  int numPieces = 0;
  char s[4096]; strcpy(s, a_s);
  while(s[pos]) {
    if(isspace(s[pos])) {
      pos++; continue;
    } else {
      int startPos = pos;
      while((s[pos]) && !isspace(s[pos])) {
	pos++;
      }
      char temp = s[pos]; s[pos] = 0;
      vec.push_back(JString(&s[startPos]));
      s[pos] = temp;
      numPieces++;
    }
  }
  return numPieces;
} // End str_split()

int str_split_mutate(char *s, vector<char*> &vec) {
  int pos = 0;
  int numPieces = 0;
  while(s[pos]) {
    if(isspace(s[pos])) {
      pos++; continue;
    } else {
      int startPos = pos;
      while((s[pos]) && !isspace(s[pos])) {
	pos++;
      }
      vec.push_back(&s[startPos]);
      numPieces++;
      if(s[pos] == 0) { break; }
      else { s[pos] = 0; pos++; }
    }
  }
  return numPieces;
} // End str_split_mutate()

int split(const JString &s, vector<JString> &vec) {
  int len = s.length();
  int pos = 0;
  int numPieces = 0;
  while(pos < len) {
    if(isspace(s[pos])) {
      pos++; continue;
    } else {
      int startPos = pos;
      while((pos < len) && !isspace(s[pos])) {
	pos++;
      }
      int subLen = pos-startPos;
      vec.push_back(s.substr(startPos, subLen));
      numPieces++;
    }
  }
  return numPieces;
} // End split()

JString get_field(const JString &s, unsigned int field_num) {
  vector<JString> fields;
  unsigned int nFields = split(s, fields);
  if(field_num >= nFields) { return ""; }
  else { return fields[field_num]; }
} // End get_field()

JString join(const vector<JString> &sVec) {
  JString newJString = "";
  for(unsigned int i = 0; i < sVec.size(); i++) {
    newJString += (sVec[i] + " ");
  }
  return newJString;
} // End join()

void JString_cp(char *buf, const JString &s) {
  for(unsigned int j = 0; j < s.length(); j++) {
    buf[j] = s[j];
  }
  buf[s.length()] = 0;
} // End JString_cp()

unsigned short util_readSCIDMapping(const char *scid_mapping,
				    vector<sc_node_t> &sc_list) {
  unsigned short mySCID = 0;
  if(Cfg.gdbg) {
    cerr << "reading scid_mapping file " << scid_mapping << endl;
  }
  ifstream p_fin(scid_mapping);
  for(;;) {
    // read each line
    if(p_fin.eof()) break;
    char line[256];
    p_fin.getline(line, sizeof(line)); // read a line
    if(p_fin.eof()) break;
    if(p_fin.fail()) {
      cerr << "Error reading " << scid_mapping << endl;
      exit(99);
    }

    // line has the format: "scid  hostName"
    vector<JString> sVec;
    int numFields = split(line, sVec);
    if(numFields != 2) {
      cerr << "Format of " << scid_mapping << " is wrong" << endl;
      exit(97);
    }

    // get the scid value
    int scidInt = JString_atoi(sVec[0]);
    if(scidInt <= 0) {
      cerr << "scid is " << scidInt << " (has to be > 0)" << endl;
      exit(98);
    }

    // put the mapping onto the list of sc nodes
    sc_node_t scn;
    scn.scid = (unsigned short)scidInt;
    scn.ipAddr = nslookup(sVec[1].cstr()); // DNS lookup
    sc_list.push_back(scn);

    // Is this mySCID?
    if(scn.ipAddr.s_addr == Cfg.myIP.s_addr) { mySCID = scn.scid; }

  } // End for(;;) reading all lines in config file
  p_fin.close();

  return mySCID;
} // End util_readSCIDMapping()

bool parseIPPort(const char *dest_ip_port, struct in_addr *ip, int *port) {
  if(dest_ip_port == NULL) { return false; }
  char *col_pos = index(dest_ip_port, ':');
  if(col_pos == NULL) {
    cerr << "Util: dest spec format error [" << dest_ip_port << "]\n";
    return false;
  }
  *col_pos = 0;
  if(ip != NULL) { inet_aton(dest_ip_port, ip); }
  if(port != NULL) { *port = atoi(col_pos+1); }
  return true;
} // End parseIPPort()

bool fileExists(const char *fileName) {
  struct stat s_buf;
  int res = stat(fileName, &s_buf);
  if(res == 0) { return true; }
  else { return false; }
} // End fileExists()
