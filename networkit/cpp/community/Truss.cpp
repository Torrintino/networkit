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

}
