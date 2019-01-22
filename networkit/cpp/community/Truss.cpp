#include "Truss.h"

namespace NetworKit {

  MaximumKTruss::MaximumKTruss(Graph& g) : g(g, 0) {
    hasRun = false;
  }

  /*** More notes and specifications can be found in the header ***/
  MaximumKTruss(Graph& g, int k) {
    SupportQueue q(g.size().second);
    
    g.forEdges([&] (node u, node v) {
	// Use triangle counting algorithm, O(m¹.5)
	int support = compute_support(g, u, v); 
	q.add(u, v, support);
      });
    q.sort();

    while(q.front().support <= k - 2) {
      std::triple e = q.front();
      q.pop();
      forNeighborsOf(e.u, [&] (node w) {
	  if(q.lookUp(e.u, w) != -1) { 
	    q.reduce(e.u, w);
	    q.reduce(e.v, w);
	    q.reorder(e.u, w);
	    q.reorder(e.v, w);
	  }
	});
    }
    
  }

  MaximumKTruss::run() {
    MaximumKTruss(g[0]);
    // print g
  }

  SupportQueue::SupportQueue(int size) {
    q.reserve(size);
    index = 0;
  }

  // Would it be smarter to use pointers for the hash table, instead of indeces?
  SupportQueue::add(double u, double v, int support) {
    q.push_back(SupportEdge(u, v, support));
    store(u, v, q.size - 1);
  }

  void SupportQueue::sort() {
    sort(q.begin(), q.end(), compareSupport);
  }

  void reduce(double u, double v) {
    int pos = h[unpair(double u, double v)];
    q[pos]--;
  }

  void SupportQueue::reorder(double u, double v) {
    int pos = h[unpair(double u, double v)];

    while(pos > index) {
      if(q[pos].third < q[pos - 1]) {
	// Exchange the element with its sucessor
	SupportEdge temp(q[pos]);
	q[pos] = q[pos-1];
	store(q[pos].u, q[pos].v, pos);
	pos--;
	q[pos] = temp;
	store(q[pos].u, q[pos].v, pos);
      } else {
	break;
      }
    }
  }

  // I think the traversing here is really inefficient, in the worst case could be O(n)
  // We should probably use more space for the hash table
  void store(int u, int v, int pos) {
    int key = unpair(u, v);
    if(h.find(key) != h.end())
      h.remove(key);
    h[key] = pos;
  }
  
}

bool compareSupport(SupportEdge e, SupportEdge f) {
  return (e.support < f.support);
}

// For a pair of integers, compute a single integer that uniquely identifies that pair
int unpair(int u, int v) {
  return ((u + v) * (u + v + 1)/2) + v;
}
