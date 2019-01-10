#ifndef TRUSS_H
#define TRUSS_H

#include "../base/Algorithm.h"
#include "../graph/Graph.h"

namespace NetworKit {

  class MaximumKTruss : public Algorithm {

  public:
    // Initialize algorithm
    // g may be overwritten
    MaximumKTruss(Graph& g);

    // Find all k-trusses in g
    // The input graph will be overwritten and functions as the result
    // All edges and nodes, which are not part of a k-truss will be deleted
    Graph& ReduceToKTruss(Graph& g, int k);

    // Call ReduceToKTruss iteratively, increasing k, until no edges are left
    // For each k, all edges which are part of a k-truss will be printed to stdout
    void run() override;

    // In the simple variant, this will be of size 1 and hold the input graph
    // Later it will hold disjunct components of the graph
    std::vector<Graph> g(1);
  };

  // This will be used for testing
  // Returns true, if g is a k-truss
  bool isKTruss(const Graph& g, int k);
  
}

#endif
