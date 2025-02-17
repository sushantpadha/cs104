/** (c) 2002 by Bhaskaran Raman and Regents of the University of California */

#ifndef TREE_H
#define TREE_H

#include <vector>

struct Tree {
  /** The invalid node index */
  const static long INVALID_NODE;

  /** The timeout value for path caches (in ms) */
  static double TREE_CACHE_TIMEOUT;

  long root; // the root of the tree
  vector<long> prevNode; // parent relationship, indexed on node-id
  vector<unsigned long> minCost; // cost of path to root, indexed on node-id

  /** The last time this tree was built */
  struct timeval buildTime;

  /** Init the tree structure */
  void init(long arg_root, unsigned long numNodes);

}; // End struct Tree
typedef struct Tree Tree_t;

#endif
