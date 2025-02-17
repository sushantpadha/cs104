/** (c) 2002 by Bhaskaran Raman and Regents of the University of California */
/** Program to test Priority Queue implementation */

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <iostream>

#include "PriorityQueue.h"
#include "Util.h"

void usage(const char *progname) {
  fprintf(stderr, "Usage: %s <num-elts-in-priority-queue>\n", progname);
} // End usage()

int main(int argc, char *argv[]) {
  if(argc != 2) { usage(argv[0]); exit(1); }

  int numElts = atoi(argv[1]);
  class PriorityQueue<unsigned short> pq(numElts);

  fprintf(stderr, "pq init with %d elements done\n", numElts);

  // set random seed
  struct timeval tv;
  gettimeofday(&tv, NULL);
  srandom(tv.tv_usec);
  fprintf(stderr, "set random seed done with %ld\n", tv.tv_usec);

  // all elts would have been initialized with cost INFINITY

  for(;;) {
    // decreaseCost a bunch of times
    int numDecTimes = random() % numElts;
    fprintf(stderr, "----------------\n");
    fprintf(stderr, "going to decrease cost %d times\n", numDecTimes);

    for(int i = 0; i < numDecTimes; i++) {
      int nIndex = random() % numElts;
      unsigned short newCost = random() % 23749;
      unsigned short oldCost;
      pq.getCost(nIndex, &oldCost);
      pq.decreaseCost(nIndex, newCost);
      fprintf(stderr, "%03d: %u --> %u\n", nIndex, oldCost, newCost);
    }

    fprintf(stderr, "----------------\n");
    int minIndex = -1;
    unsigned short minCost = PriorityQueue<unsigned short>::INFINITY;
    for(int i = 0; i < numElts; i++) {
      unsigned short cost;
      pq.getCost(i, &cost);
      fprintf(stderr, "%03d: %u\n", i, cost);
      if((minIndex == -1) || (cost < minCost)) {
	minIndex = i; minCost = cost;
      }
    }
    fprintf(stderr, "----------------\n");
    fprintf(stderr, "minIndex=%d, minCost=%u\n", minIndex, minCost);

    unsigned short pq_minCost = PriorityQueue<unsigned short>::INFINITY;
    int pq_minIndex = pq.minVertex(&pq_minCost);
    fprintf(stderr, "pq_minIndex=%d, pq_minCost=%u\n",
	    pq_minIndex, pq_minCost);

    fprintf(stderr, "----------------\n");
    fprintf(stderr, "Enter 1 for removeMin\n");
    fprintf(stderr, "Enter 2 for removeVertex\n");
    fprintf(stderr, "Enter 3 to continue without removing any vertex\n");
    fprintf(stderr, "Enter choice: ");
    char readIn[64];
    cin.getline(readIn, sizeof(readIn));

    if(strcmp(readIn, "1") == 0) {
      fprintf(stderr, "removing min node in PQ\n");
      pq.removeMin();
    } else if(strcmp(readIn, "2") == 0) {
      int nIndex = random() % numElts;
      fprintf(stderr, "removing vertex %d\n", nIndex);
      pq.removeVertex(nIndex);
    } else if(strcmp(readIn, "3") == 0) {
      continue;
    } else {
      fprintf(stderr, "Unrecognized choice %s; assuming choice 3\n", readIn);
    }

  } // End for(;;)

} // End main()
