#ifndef TRUSS_H
#define TRUSS_H

#include "../base/Algorithm.h"
#include "../graph/Graph.h"

namespace NetworKit {

  /* The implementation is according to "Truss Decomposition in Massive Networks" */
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


  /* This is a Queue, which holds a Triple (u, v, s), where u and v are ID's
     of two nodes, which form an edge and s is the support of that edge.
     After the initialization phase, the elements will be ordered by their
     support.

     This data structure assumes, that there is one initialization phase,
     where all elements get added and then the elements will only be reordered.
     A special index is used, that marks the front element. So elements actually
     wont be removed from the queue, instead the index will be increased.
  */
  class SupportQueue {

  public:
    EdgeSupport(int size); // Initialize index to 0
    void push(double u, double v, int support);
    SupportEdge top() { return q[index]; }

    // Decrease support of edge at given position and then reorder
    void reduce(int pos);

    void sort() { sort(sq.begin(), sq.end(), compareSupport)};

    // Remove the front element from the Queue
    void pop() { index++; }

  private:
    std::vector<SupportEdge> q;
    int index;
  };

  // This is just a convenience class, working with triples is pretty ugly oftentimes
  class SupportEdge {

  public:
    double u, v;
    int support;

    SupportEdge(double u, double u, int support) : u(u), v(v), support(support) {};
    SupportEdge(SupportEdge copy) : u(copy.u), v(copy.v), support(copy.support) {};
  }

  bool compareSupport(SupportEdge e, SupportEdge f) { return (e.support < f.support); }
}

#endif
