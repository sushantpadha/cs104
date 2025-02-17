/** (c) 2002 by Bhaskaran Raman and Regents of the University of California */

/** Implements Tree.h */

#include "Tree.h"
#include "PriorityQueue.h"

const long Tree::INVALID_NODE = -1;
double Tree::TREE_CACHE_TIMEOUT = 0*1000; // 0 sec

void Tree::init(long arg_root, unsigned long numNodes) {
  root = arg_root;

  // initialize prevNode and minCost
  while(prevNode.size() > 0) { prevNode.pop_back(); }
  while(minCost.size() > 0) { minCost.pop_back(); }
  for(unsigned long i = 0; i < numNodes; i++) {
    prevNode.push_back(INVALID_NODE);
    minCost.push_back(PriorityQueue<unsigned long>::INFINITY);
  }

  // set the prevNode and minCost of the root
  prevNode[root] = root;
  minCost[root] = 0;

} // End Tree:init()
