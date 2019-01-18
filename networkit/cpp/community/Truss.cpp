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
      forNeighborsOf(e.first, [&] (node w) {
	  if(q.lookUp(e.first, w) != -1) { 
	    q.reduce(e.first, w);
	    q.reduce(e.second, w);
	    q.reorder(e.first, w);
	    q.reorder(e.second, w);
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
    // How much space do we need for the hash table?
    // At least O(m), should be at max O(m * log m), O(m^2) would be the extreme
  }

  // Would it be smarter to use pointers for the hash table, instead of indeces?
  SupportQueue::add(double u, double v, int support) {
    q.push_back(std::Triple<double, double, int>(u, v, support));
    // compute hash of (u,v)
    h[hash] = q.size - 1;
  }

  int SupportQueue::lookUp(double u, double v) {
    // What hash function do we use?
  }

  void SupportQueue::sort() {
    sort(q.begin(), q.end(), compareSupport);
  }
  
  
}

bool compareSupport(std::Triple<double, double, int> u, std::Triple<double, double, int> u) {
  return (u.third < v.third);
}
