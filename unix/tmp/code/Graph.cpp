/** (c) 2002 by Bhaskaran Raman and Regents of the University of California */

/** Implements Graph.h */

#include "Graph.h"
#include "PriorityQueue.h"

void Graph::Dijkstra(long root, Tree_t &t) {

  PriorityQueue<unsigned long> pq(nodes.size());

  // Initialization for Dijkstra's algo
  t.init(root, nodes.size());
  pq.decreaseCost(root, 0);

  unsigned long minCost;
  int minVertex = pq.minVertex(&minCost);
  while(minVertex != -1) { // Process all nodes in the pq
    pq.removeMin(); // remove the node from the priority queue

    GraphNode_t &gNode = nodes[minVertex];

    // expand minVertex
    for(unsigned int i = 0; i < gNode.incoming.size(); i++) {
      GraphEdge_t &gEdge = gNode.incoming[i];
      unsigned long oldCost;
      bool nodeExists = pq.getCost(gEdge.srcVertex, &oldCost);
      if(!nodeExists) {
	continue; // node already removed from PQ, move on to next nbr
      }
      unsigned long newCost = minCost + gEdge.arcCost;
      if((gEdge.arcCost == PriorityQueue<unsigned long>::INFINITY) ||
	 (minCost == PriorityQueue<unsigned long>::INFINITY)) {
	newCost = PriorityQueue<unsigned long>::INFINITY;
      }
      if((newCost < oldCost) ||
	 (minCost == PriorityQueue<unsigned long>::INFINITY)) {
	if(newCost < oldCost) {
	  pq.decreaseCost(gEdge.srcVertex, newCost);
	}
	t.prevNode[gEdge.srcVertex] = minVertex;
	t.minCost[gEdge.srcVertex] = newCost;
      }
    } // End for(i) expanding minVertex

    minVertex = pq.minVertex(&minCost); // get next node to look at
  }

} // End Graph::Dijkstra()
