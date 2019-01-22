#include "Truss.h"

// For a pair of integers, compute a single integer that uniquely identifies that pair
int unpair(int u, int v) {
  return ((u + v) * (u + v + 1)/2) + v;
}

namespace NetworKit {

  MaximumKTruss::MaximumKTruss(Graph& g) : g(g, 0) {
    hasRun = false;
  }

  /*** More notes and specifications can be found in the header ***/
  MaximumKTruss(Graph& g, int k) {
    SupportQueue sq.size().second;
    std::unordered_map<int, int> h;
        
    g.forEdges([&] (node u, node v) {
	// Use triangle counting algorithm, O(m¹.5)
	int support = compute_support(g, u, v); 
	sq.push(SupportEdge(u, v, support));
      });
    sq.sort();
    

    while(SupportEdge e = sq.top().support <= k - 2) {
      sq.pop();
      forNeighborsOf(e.u, [&] (node w) {
	  if(int pos_u = h.find(unpair(e.v, w)) != h.end()) {
	    int pos_v = h.find(unpair(e.u, w);
	    sq.reduce(pos_u);
	    sq.reduce(pos_v);
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
  void SupportQueue::push(SupportEdge e) {
    q.push_back(SupportEdge(u, v, support));
    h[unpair(u, v)] = q.size - 1;
  }

  void SupportQueue::reduce(double u, double v) {
    int pos = h[unpair(double u, double v)];
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
