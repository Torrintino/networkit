#include "Truss.h"

namespace NetworKit {

  int unpair(int u, int v) {
    if(u >= v) {
      return ((u + v) * (u + v + 1)/2) + v;
    } else {
      return ((u + v) * (u + v + 1)/2) + u;
    }
  }
  
  count compute_support(const Graph& g, node u, node v) {
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
			c++;
		}
	}
	//c+=neighbors_u.size()-i;
	//c+=neighbors_v.size()-j;
    return c;
  }

  MaximumKTruss::MaximumKTruss(Graph& g) {
    g.sortEdges();
    hasRun = false;
    this->g.push_back(g);
    k = 3;
  }

  void MaximumKTruss::run() {
    Graph copy(g[0]);
    ReduceToKTruss(copy, k);
    while(!copy.isEmpty()) {
      g[0] = copy;
      ReduceToKTruss(copy, ++k);
    }
    k--;
  }

  /*** More notes and specifications can be found in the header ***/
  void ReduceToKTruss(Graph& g, count k) {
    SupportQueue sq(g.size().second);
        
    g.forEdges([&] (node u, node v) {
	// Use triangle counting algorithm, O(m log(m))
	count support = compute_support(g, u, v); 
	sq.push(u, v, support);
      });
    sq.sort();
    sq.init_hash_table();
    sq.init_support_index();

    while(sq.head < sq.q.size() && sq.top().support < k - 2) {
      SupportEdge e = sq.top();
      sq.pop();
      
      g.forNeighborsOf(e.u, [&] (node w) {
	  if(sq.isEdge(e.v, w)) {
	    sq.reduce(e.v, w);
	    sq.reduce(e.u, w);
	  }
	});
      g.removeEdge(e.u, e.v);
    }

    g.forNodes([&] (node u) {
	if(g.degree(u) == 0)
	  g.removeNode(u);
      });

  }

  SupportQueue::SupportQueue(count size) {
    q.reserve(size);
    support_index.reserve(size);
    head = 0;
  }

  void SupportQueue::init_support_index() {
    count current_support = q[0].support;
    support_index[q[0].support] = 0;
    for(count i=1; i<q.size(); i++) {
      if(q[i-1].support < q[i].support)
	support_index[q[i].support] = i;
    }
  }

  void SupportQueue::init_hash_table() {
    for(int i=0; i<q.size(); i++)
      h[unpair(q[i].u, q[i].v)] = i;
  }

  // Would it be smarter to use pointers for the hash table, instead of indeces?
  void SupportQueue::push(node u, node v, count support) {
    q.push_back(SupportEdge(u, v, support));
  }

  void SupportQueue::reduce(node u, node v) {
    count pos = h[unpair(u, v)];

    // The index to the elements with the old support moves one position to the right
    count i = support_index[q[pos].support]++;
    if(pos != i) {
      // There is another element with the same support in front of q[pos]
      // As we reduce the support only by one, the new correct position for q[pos]
      //  is the index i, where elements with that support begin, so we can exchange
      //  q[pos] with q[i]
      SupportEdge temp(q[pos]);
      q[pos] = q[i];
      q[i] = temp;
      h[unpair(u, v)] = i;
      h[unpair(q[pos].u, q[pos].v)] = pos;

      pos = i;
    }
    q[pos].support--;
    // Now we need to handle the case, that there is no index for the new support
    if(pos > 0) {
      if(q[pos - 1].support != q[pos].support)
	support_index[q[pos].support] = i;
    }
  }

  void SupportQueue::pop() {
    support_index[top().support]++;
    head++;
  }

  bool SupportQueue::isEdge(node u, node v) {
    auto pos = h.find(unpair(u, v));
    return pos != h.end() && (*pos).second >= head;
  }

  bool isKTruss(const Graph& g, int k) {
    bool out=true;
    g.forEdges([&] (node u, node v) {
	auto s = compute_support(g, u, v);
	if(s < k - 2) {
	  out=false;
	  return;
	}
      });
    return out;
  }

}
