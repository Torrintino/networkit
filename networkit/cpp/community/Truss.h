#ifndef TRUSS_H
#define TRUSS_H

#include "../base/Algorithm.h"
#include "../graph/Graph.h"

#include <unordered_map>

/* Authors: Niklas Deckers, Lennart Weiß */

namespace NetworKit {

  
  // This is just a convenience class, working with triples is pretty ugly oftentimes
  class SupportEdge {

  public:
    double u, v;
    count support;

    SupportEdge(double u, double v, count support) : u(u), v(v), support(support) {};
  };

  
  bool compareSupport(SupportEdge e, SupportEdge f) { return (e.support < f.support); }

  count compute_support(const Graph& g, node u, node v);

  // For a pair of integers, compute a single integer that uniquely identifies that pair
  int unpair(int u, int v);



  
  /* This data structure assumes, that there is one initialization phase,
       where all elements get added and then the elements will only be reordered.
     A special index is used, that marks the front element. So elements actually
       wont be removed from the queue, instead the index will be increased. */
  
  class SupportQueue {

  public:

    SupportQueue(count size); // Initialize index to 0
    void push(node u, node v, count support);
    void pop();
    SupportEdge& top() { return q[head]; }
    void sort() { std::sort(q.begin(), q.end(), compareSupport);}

    // Decrease support of edge at given position and then reorder
    void reduce(node u, node v);
    
    bool isEdge(node u, node v);

    /* Assuming all edges were added and sorted, create references to all positions in the
       queue, where a certain support is starting
       WARNING: if there is no element with support s in q,
       then the value of support_index[s] is undefined and shall never be used! */
    void init_support_index();

    // This has to be called after sorting, or the positions will be incorrect
    void init_hash_table();
    

    count head;
    
    std::vector<SupportEdge> q;
    std::vector<count> support_index;
    std::unordered_map<count, count> h;
  };
  

  /* The implementation is according to "Truss Decomposition in Massive Networks" */
  class MaximumKTruss : public Algorithm {

  public:
    MaximumKTruss(Graph& g);

    // Call ReduceToKTruss iteratively, increasing k, until no edges are left
    // For each k, all edges which are part of a k-truss will be printed to stdout
    void run() override;
    
    // All edges and nodes, which are not part of a k-truss will be deleted
    void ReduceToKTruss(Graph& g, count k);

    // After the algorithm was run, this holds the maximum k-truss
    Graph g;

    // The largest k, for which the Graph is a k-Truss
    count k;

    SupportQueue sq;
  };

  // This will be used for testing
  // Returns true, if g is a k-truss
  bool isKTruss(const Graph& g, int k);

}

#endif
