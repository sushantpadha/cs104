/** (c) 2002 by Bhaskaran Raman and Regents of the University of California */
#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <iostream>

/** Implements a priority queue of integer type elements, with given
    CostType */
template <class CostType_t>
class PriorityQueue {
protected:
  struct pqNodeInfo {
    int nIndex;
    CostType_t cost;
  };
  typedef struct pqNodeInfo pqNodeInfo_t;

  // A maintains a tree, A[0] is the root, whose children are A[1],
  // and A[2].  In general, node i has 2*i+1 and 2*(i+1) as children.
  // Invariant: cost of parent is less than cost of either children
  // ==> min of all nodes is always at root.
  pqNodeInfo_t *A;
  int numElts; // #elts currently in the priority queue (in the array 'A')
  int maxNumElts; // the size of the array allocated
  // invariant: pos_in_A[i] is the index into the node in array A
  // which has nIndex 'i'.  That is, if j = pos_in_A[i], then
  // A[j].nIndex will be 'i'.
  int *pos_in_A;

  /** Swap the two elements at the two given indices of array A */
  void swap(int pos1, int pos2) {
    // swap in pos_in_A array
    int nIndex1 = A[pos1].nIndex;
    int nIndex2 = A[pos2].nIndex;
    pos_in_A[nIndex1] = pos2;
    pos_in_A[nIndex2] = pos1;
    // do the actual swap
    pqNodeInfo_t temp = A[pos1];
    A[pos1] = A[pos2];
    A[pos2] = temp;
  } // End PriorityQueue::swap()

public:
  /** The maximum value (assume cost can never get this high) */
  static CostType_t INFINITY;

  /** Constructor -- has to be given the maximum number of elements in
      the priority queue */
  PriorityQueue(int arg_maxNumElts);

  /** Destructor */
  ~PriorityQueue();

  /** Decrease the cost of the given node (insert the node in the
      priority queue, if not already present).  Return success. */
  bool decreaseCost(int nIndex, const CostType_t &cost);

  /** Return the index of the minimum vertex index (-1 if the priority
      queue is empty).  The cost of the min vertex returned in the
      given pointer.  This operation does not change the priority
      queue (specifically, does not remove the minVertex).  */
  int minVertex(CostType_t *cost);

  /** Remove the minimum node from the tree */
  void removeMin();
  /** Remove an arbitrary node from the priority queue */
  void removeVertex(int nIndex);
  /** This operation increases the cost of the min node to INFINITY --
      this makes the min node reusable, while having the size of the
      priority queue unchanged */
  void reuseMin();

  /** Return the cost of the given node in '*cost'.  Return value is
      false if node does not exist, true if it exists. */
  bool getCost(int nIndex, CostType_t *cost);

}; // End class PriorityQueue

template <class CostType_t>
PriorityQueue<CostType_t>::PriorityQueue(int arg_maxNumElts) {
  numElts = maxNumElts = arg_maxNumElts;
  // allocation
  A = new pqNodeInfo[maxNumElts];
  pos_in_A = new int[maxNumElts];
  // initialization of costs, and the reverse index array
  for(int i = 0; i < numElts; i++) {
    A[i].nIndex = i; A[i].cost = INFINITY;
    pos_in_A[i] = i;
  }
} // End PriorityQueue::PriorityQueue()

template <class CostType_t>
PriorityQueue<CostType_t>::~PriorityQueue() {
  delete[] A;
  delete[] pos_in_A;
} // End PriorityQueue::~PriorityQueue()

template <class CostType_t>
bool PriorityQueue<CostType_t>::decreaseCost(int nIndex, const CostType_t &cost) {
  // check indices, and get the index into A
  if((nIndex < 0) || (nIndex >= maxNumElts)) {
    cerr << "nIndex " << nIndex << " out of bound in decreaseCost" << endl;
    return false;
  }
  int aIndex = pos_in_A[nIndex];
  if(aIndex >= numElts) {
    //cerr << "nIndex " << nIndex << " does not exist in pq anymore" << endl;
    return false;
  }

  CostType_t oldCost = A[aIndex].cost;
  if(cost > oldCost) {
    cerr << "cannot increase cost from " << oldCost << " to " << cost
	 << endl;
    return false;
  }

  A[aIndex].cost = cost;

  // percolate up the tree
  int currIndex = aIndex;
  while(currIndex != 0) {
    int parIndex, sibIndex;
    if((currIndex%2) == 0) {
      parIndex = (currIndex/2) - 1;
      sibIndex = currIndex-1;
    } else {
      sibIndex = currIndex+1;
      parIndex = (sibIndex/2) - 1;
    }

    // check if we're okay
    if(A[currIndex].cost >= A[parIndex].cost) {
      break; // tree structure is not affected, we're done
    } else {
      swap(currIndex, parIndex); // swap with parent
    }
    currIndex = parIndex;
  } // End while(currIndex != 0)

  return true;
} // End PriorityQueue::decreaseCost()

template <class CostType_t>
int PriorityQueue<CostType_t>::minVertex(CostType_t *cost) {
  if(numElts > 0) {
    *cost = A[0].cost;
    return A[0].nIndex;
  } else {
    return -1;
  }
} // End PriorityQueue::minVertex()

template <class CostType_t>
void PriorityQueue<CostType_t>::removeMin() {
  if(numElts <= 0) {
    cerr << "no elements to remove in removeMin" << endl;
    return;
  }
  // swap the minimum element with the last element
  swap(0, numElts-1);
  int nIndex = A[numElts-1].nIndex;
  A[numElts-1].cost = INFINITY;
  pos_in_A[nIndex] = numElts-1; // indicates that this element has been removed
  numElts--; // one element less

  // percolate down
  int currIndex = 0;
  for(;;) {
    int leftChild, rightChild;
    leftChild = 2*currIndex+1;
    rightChild = 2*(currIndex+1);

    if((leftChild >= numElts) && (rightChild >= numElts)) {
      // have reached bottom of tree
      break;
    }

    if(leftChild == (numElts-1)) {
      // case when only leftChild exists, and no rightChild
      if(A[leftChild].cost < A[currIndex].cost) {
	swap(leftChild, currIndex);
      }
      break; // we're done
    }

    // at this point, both the left and right children are there
    if(A[leftChild].cost < A[currIndex].cost) {

      if(A[leftChild].cost < A[rightChild].cost) {
	// leftChild is the minimum of the three, can percolate
	// towards left and continue
	swap(leftChild, currIndex);
	currIndex = leftChild;
	continue;
      } else {
	// rightChild is the minimum of the three, can percolate
	// towards right and continue
	swap(rightChild, currIndex);
	currIndex = rightChild;
	continue;
      }

    } else if(A[rightChild].cost < A[currIndex].cost) {
      // rightChild is the minimum of the three, can percolate towards
      // right and continue
      swap(rightChild, currIndex);
      currIndex = rightChild;
      continue;

    } else {
      // currIndex is smaller than its children, can stop here
      break;
    }

  } // End for(;;)

} // End PriorityQueue::removeMin()

template <class CostType_t>
void PriorityQueue<CostType_t>::removeVertex(int nIndex) {
  // check indices, and get the index into A
  if((nIndex < 0) || (nIndex >= maxNumElts)) {
    cerr << "nIndex " << nIndex << " out of bound in removeVertex" << endl;
    return;
  }
  int aIndex = pos_in_A[nIndex];
  if(aIndex >= numElts) {
    //cerr << "nIndex " << nIndex << " does not exist in pq anymore" << endl;
    return;
  }

  // percolate given vertex up to the root
  int currIndex = aIndex;
  while(currIndex != 0) {
    int parIndex, sibIndex;
    if((currIndex%2) == 0) {
      parIndex = (currIndex/2) - 1;
      sibIndex = currIndex-1;
    } else {
      sibIndex = currIndex+1;
      parIndex = (sibIndex/2) - 1;
    }
    swap(currIndex, parIndex); // swap with parent
    currIndex = parIndex;
  } // End while(currIndex != 0)

  // now simply call removeMin
  removeMin();

} // End PriorityQueue::removeVertex()

template <class CostType_t>
void PriorityQueue<CostType_t>::reuseMin() {
  // remove the minimum
  removeMin();

  // Now, the priority queue size would have reduced by 1 -- the
  // earlier min element would now be at A[numElts].  The cost of this
  // element, as well as the pos_in_A record corresponding to this
  // would have been updated by removeMin()

  // Simply increase the size of the priority queue by 1
  numElts++;
} // End PriorityQueue::reuseMin()

template <class CostType_t>
bool PriorityQueue<CostType_t>::getCost(int nIndex, CostType_t *cost) {
  // check indices, and get the index into A
  if((nIndex < 0) || (nIndex >= maxNumElts)) {
    cerr << "nIndex " << nIndex << " out of bound in getCost" << endl;
    *cost = INFINITY;
    return false;
  }
  int aIndex = pos_in_A[nIndex];
  if(aIndex >= numElts) {
    //cerr << "nIndex " << nIndex << " does not exist in pq anymore" << endl;
    *cost = INFINITY;
    return false;
  }
  *cost = A[aIndex].cost;
  return true;
} // End PriorityQueue::getCost()

#endif
