/** (c) 2002 by Bhaskaran Raman and Regents of the University of California */
#ifndef UTIL_H
#define UTIL_H

#include <sys/time.h>
#include <vector>

#include "JHashtable.h"
#include "JString.h"
#include "Config.h"

/** Return a random integer in the range [lower, upper), given the
    random number in the range (0, 1) */
int rand_int(double rand_01, int lower, int upper);

/** Shorthand for DNS lookup -- exits on error! */
struct in_addr nslookup(const char *host);

/** Call socket and bind to given port.  Return socket descriptor, or
    -1 on failure. */
int sock_and_bind(int port);

/** Call socket and connect via TCP to given host/port.  Return socket
    descriptor, or -1 on failure */
int sock_and_connect(const char *host, int port);
int sock_and_connect(struct in_addr server_ip, int port);
/** socket, bind, and listen for TCP connections.  Return socket
    descriptor, or -1 on failure */
int sock_and_listen(int port);


/** Return the given time value in double format, in ms */
double time2double(struct timeval t);
/** Return the current time value in double format, in ms, relative to
    the given start time */
double now(struct timeval start);
/** Return a time that was 't' ms in the past */
struct timeval past(unsigned long t);
/** Return the current time value in long format, in seconds -- return
    only the seconds part. */
long timestamp();

/** Hash value of an unsigned long */
unsigned int ulongHash(const unsigned long &l);
/** Hash value of an unsigned short */
unsigned int ushortHash(const unsigned short &u);
/** Return the hash value of a given address */
typedef char *char_p;
unsigned int addrHash(const char_p &addr);
/** Return the hash value of a given JString */
unsigned int JStringHash(const JString &s);

/** Do string copy, and return the position of the last '\0' in the
    destn string */
char *my_strcpy(char *dest, const char *src);
/** Print onto given string, and return the position of the last '\0' */
char *longPrint(char *s, const long val);
/** Return a JString with the given number printed */
JString long2str(const long val);

/** Print onto given string, and return the position of the last '\0' */
char *ipPrint(char *s, const struct in_addr ip);
/** Return a JString version of the given IP address */
JString ip2str(const struct in_addr ip);

/** A version of atoi that takes in a JString as an argument */
int JString_atoi(const JString &s);

/** Split the given JString into a vector of JStrings.  Return the size
    of the resultant vector, -1 if error */
int str_split(const char *s, vector<JString> &vec);
/** Split the string, mutate it by replacing putting '\0' for each
    separating space, and return the vector of char pointers to
    beginning of each sub string in vec.  Return the size of the
    resultant vector, -1 if error. */
int str_split_mutate(char *s, vector<char*> &vec);
/** Split the given JString into a vector of JStrings.  Return the size
    of the resultant vector, -1 if error */
int split(const JString &s, vector<JString> &vec);
/** Get the n'th field of the given string */
JString get_field(const JString &s, unsigned int field_num);
/** Join the given vector of JStrings into one JString -- each elt of
    the vector is separated by a single space in the resulting JString */
JString join(const vector<JString> &sVec);

/** Copy the given JString onto the given buffer, and terminate it with
    a NULL (assume enough space in given buffer) */
void JString_cp(char *buf, const JString &s);

/*---- To read scid-mapping files ----*/
struct sc_node {
  unsigned short scid;
  struct in_addr ipAddr;
};
typedef struct sc_node sc_node_t;
/** Return mySCID (match with myIP) */
unsigned short util_readSCIDMapping(const char *scid_mapping,
				    vector<sc_node_t> &sc_list);

/** Parse IP:port info into given variables, return success/failure */
bool parseIPPort(const char *dest_ip_port, struct in_addr *ip, int *port);

/** Check if the given fileExists */
bool fileExists(const char *fileName);

#endif
