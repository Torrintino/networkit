#ifndef COMPUTE_DISTANCE_H
#define COMPUTE_DISTANCE_H

#include "../base/Algorithm.h"
#include "../graph/Graph.h"

namespace NetworKit {

  class ComputeDistance : public Algorithm {

  public:
    ComputeDistance(const Graph& g, node start);
    void run() override;

  private:
    const Graph& g;
    node start;
  };
  
}

#endif
