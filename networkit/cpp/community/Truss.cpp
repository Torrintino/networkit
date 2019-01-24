#include "Truss.h"

namespace NetworKit {
  
  int compute_support(Graph& g, double u, double v) {
    return 0;
  }

  MaximumKTruss::MaximumKTruss(Graph& g) {
    hasRun = false;
    this->g.push_back(g);
  }

  /*** More notes and specifications can be found in the header ***/
  Graph& ReduceToKTruss(Graph& g, int k) {
    SupportQueue sq(g.size().second);
        
    g.forEdges([&] (node u, node v) {
	// Use triangle counting algorithm, O(m¹.5)
	int support = compute_support(g, u, v); 
	sq.push(u, v, support);
      });
    sq.sort();
    

    while(sq.top().support <= k - 2) {
      SupportEdge e = sq.top();
      sq.pop();
      g.forNeighborsOf(e.u, [&] (node w) {
	  if(sq.isEdge(e.v, w)) {
	    sq.reduce(e.v, w);
	    sq.reduce(e.u, w);
	  }
	});
    }

    return g;
  }

  void MaximumKTruss::run() {
    MaximumKTruss(g[0]);
    // print g
  }

  SupportQueue::SupportQueue(int size) {
    q.reserve(size);
    index = 0;
  }

  // Would it be smarter to use pointers for the hash table, instead of indeces?
  void SupportQueue::push(double u, double v, int support) {
    q.push_back(SupportEdge(u, v, support));
    h[unpair(u, v)] = q.size() - 1;
  }

  void SupportQueue::reduce(double u, double v) {
    int pos = h[unpair(u, v)];
    q[pos].support--;

    // Reorder
    while(pos > index) {
      if(q[pos].support < q[pos - 1].support) {
	
	// Exchange the element with its sucessor
	SupportEdge temp(q[pos]);
	q[pos] = q[pos-1];
	h[unpair(q[pos].u, q[pos].v)] = pos;
	pos--;
	q[pos] = temp;
	h[unpair(q[pos].u, q[pos].v)] = pos;
      } else {
	break;
      }
    }
  }

}
