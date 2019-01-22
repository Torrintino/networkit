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
    h.reserve(size);
    for(int i=0; i<size; i++) h[i] = -1;
    // How much space do we need for the hash table?
    // At least O(m), should be at max O(m * log m), O(m^2) would be the extreme
  }

  // Would it be smarter to use pointers for the hash table, instead of indeces?
  SupportQueue::add(double u, double v, int support) {
    q.push_back(std::Triple<double, double, int>(u, v, support));
    store(u, v, q.size - 1);
  }

  int SupportQueue::lookUp(double u, double v) {
    int pos = hash(u, v) % q.size;
    while(q[pos].first != u && q[pos].second != v) pos++;
    return pos;
  }

  void SupportQueue::sort() {
    sort(q.begin(), q.end(), compareSupport);
  }

  void SupportQueue::reorder(double u, double v) {
    int pos = lookUp(double u, double v);

    while(pos > 0) {
      if(q[pos].third < q[pos - 1]) {
	// Exchange the element with its sucessor
	std::triple<double, double, int> temp(q[pos]);
	q[pos] = q[pos-1];
	store(q[pos].first, q[pos].second, pos);
	pos--;
	q[pos] = temp;
	store(q[pos].first, q[pos].second, pos);
      } else {
	break;
      }
    }
  }

  // I think the traversing here is really inefficient, in the worst case could be O(n)
  // We should probably use more space for the hash table
  void store(int u, int v, int value) {
    int pos = hash(u, v) % q.size;
    while(h[pos] != -1) pos++;
    h[pos] = value;
  }
  
}

bool compareSupport(std::Triple<double, double, int> u, std::Triple<double, double, int> u) {
  return (u.third < v.third);
}

int hash(int u, int v) {
  return ((u + v) * (u + v + 1)/2) + v;
}
