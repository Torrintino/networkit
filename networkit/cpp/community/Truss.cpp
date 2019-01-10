#include "Truss.h"

namespace NetworKit {

  MaximumKTruss::MaximumKTruss(Graph& g) : g(g, 0) {
    hasRun = false;
  }

  MaximumKTruss(Graph& g, int k) {
    // This data structure is supposed to hold a sorted array of edges and their support
    // I'll assume that edge_support.first() returns a Triple with attributes u, v, support,
    // where u and v are the node id's of the edge
    // Removing the first element must be done in O(1)
    SupportQueue edge_support(g.size().second);
    std::vector<std::Pair(node u, node v)> phi;
    
    g.forEdges([&] (node u, node v) {
	int support = compute_support(g, u, v);
	edge_support.add(u, v, support);
      });
    edge_support.sort();

    while(edge_support.front().support <= k - 2) {
      std::triple e = edge_support.front();
      forNeighborsOf(e.first, [&] (node w) {
	  if(g.hasEdge(v, w)) {
	    edge_support.reduce(u, w);
	    edge_support.reduce(v, w);
	    edge_support.reorder();
	  }
	});
    }
    
  }

  MaximumKTruss::run() {
    MaximumKTruss(g[0]);
    // print g
  }

}
