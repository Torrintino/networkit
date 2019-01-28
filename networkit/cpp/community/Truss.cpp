#include "Truss.h"

namespace NetworKit {
  
  count compute_support(Graph& g, node u, node v) {
	count c=0;
    auto neighbors_u=g.neighbors(u);
    auto neighbors_v=g.neighbors(v);
	count i=0;
	count j=0;
	while(i<neighbors_u.size() && j<neighbors_v.size()){
		auto a=neighbors_u.at(i);
		auto b=neighbors_v.at(j);
		if(a<b){
			i++;
		} else if(a>b){
			j++;
		} else {
			i++;
			j++;
		}
		c++;
	}
	c+=neighbors_u.size()-i;
	c+=neighbors_v.size()-j;
    return c;
  }

  MaximumKTruss::MaximumKTruss(Graph& g) {
    g.sortEdges();
    hasRun = false;
    this->g.push_back(g);
  }

  /*** More notes and specifications can be found in the header ***/
  Graph& ReduceToKTruss(Graph& g, count k) {
    SupportQueue sq(g.size().second);
        
    g.forEdges([&] (node u, node v) {
	// Use triangle counting algorithm, O(m log(m))
	count support = compute_support(g, u, v); 
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

  SupportQueue::SupportQueue(count size) {
    q.reserve(size);
    support_index.support(count);
    head = 0;
  }

  // Would it be smarter to use pointers for the hash table, instead of indeces?
  void SupportQueue::push(node u, node v, count support) {
    q.push_back(SupportEdge(u, v, support));
    h[unpair(u, v)] = q.size() - 1;
  }

  void SupportQueue::reduce(node u, node v) {
    count pos = h[unpair(u, v)];
    q[pos].support--;

    // Reorder
    while(pos > head) {
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
