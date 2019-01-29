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
    sq.init_support_index();

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

    for(int i=0; i<sq.head; i++)
      g.removeEdge(sq.q[i].u, sq.q[i].v);

    return g;
  }

  void MaximumKTruss::run() {
    MaximumKTruss(g[0]);
    // print g
  }

  SupportQueue::SupportQueue(count size) {
    q.reserve(size);
    support_index.reserve(size);
    support_index[0] = 0;
    head = 0;
  }

  void SupportQueue::init_support_index() {
    count current_support = 1;
    for(count i=0; i<q.size(); i++) {
      if(current_support < q[i].support)
	support_index[current_support++] = i;
    }
  }

  // Would it be smarter to use pointers for the hash table, instead of indeces?
  void SupportQueue::push(node u, node v, count support) {
    q.push_back(SupportEdge(u, v, support));
    h[unpair(u, v)] = q.size() - 1;
  }

  void SupportQueue::reduce(node u, node v) {
    count pos = h[unpair(u, v)];

    // The index to the elements with the old support moves one position to the right
    support_index[q[pos].support]++;
    
    count i = support_index[q[pos].support];
    if(pos != i) {
      // There is another element with the same support
      // As we reduce the support only by one, the new correct position for q[pos]
      //  is the index i, where elements with that support begin, so we can exchange
      //  q[pos] with q[i]
      SupportEdge temp(q[pos]);
      q[pos] = q[i];
      q[i] = temp;

      pos = i;
    }
    q[pos].support--;
    // Now we need to handle the case, that there is no index for the new support
    if(pos > 0) {
      if(q[pos - 1].support != q[pos].support)
	support_index[q[pos].support] = i;
    }
  }

}
