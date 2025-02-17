/** (c) 2002 by Bhaskaran Raman and Regents of the University of California */

#ifndef GRAPH_H
#define GRAPH_H

#include <vector>

#include "Tree.h"

/** Represents a graph edge -- is stored along with a node, which
    represents the destination vertex of the edge */
struct GraphEdge {
  long srcVertex;
  unsigned long arcCost;
  unsigned long edgeLoad;
  unsigned long maxLoad;
  GraphEdge() { edgeLoad = 0; maxLoad = 2500; }
}; // End struct GraphEdge
typedef struct GraphEdge GraphEdge_t;

struct GraphNode {
  vector<GraphEdge_t> incoming;
}; // End struct GraphNode
typedef struct GraphNode GraphNode_t;

struct Graph {
  /** The nodes in the graph */
  vector<GraphNode_t> nodes;

  /** The Dijkstra's algorithm.  The given Tree is filled with the
      output of the algo. */
  void Dijkstra(long root, Tree_t &t);
}; // End struct Graph
typedef struct Graph Graph_t;

#endif
