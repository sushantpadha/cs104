/** (c) 2002 by Bhaskaran Raman and Regents of the University of California */
#ifndef RTP_H
#define RTP_H

#define RTP_VERSION		2
#define RTP_PT_GSM		3
#define RTP_PT_PCMU		0
#define RTP_PT_G723		4
#define RTP_PT_PCMSW 77

struct rtphdr {
  unsigned short int rh_flags; /* T:2 P:1 X:1 CC:4 M:1 PT:7 */
  unsigned short int rh_seqno; /* sequence number */
  unsigned long rh_ts; /* media-specific time stamp */
  unsigned long rh_ssrc; /* synchronization src id */
  /* data sources follow per cc */
};
typedef struct rtphdr rtphdr_t;

struct myRTPpacket {
  struct rtphdr rh;
  char data[512];
};
typedef struct myRTPpacket myRTPpacket_t;

#endif
