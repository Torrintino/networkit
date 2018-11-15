#include "ComputeDistance.h"

#include <iostream>
#include <vector>

namespace NetworKit {

  ComputeDistance::ComputeDistance(const Graph& g, node start) : g(g) {
    this->start = start;
    hasRun = false;
  }

  void ComputeDistance::run() {
    int sum = 0;
    // for each neighbour
    // add 1 to the sum
    // visit neighbours and repeat (only adding if node wasn't visited already)
    unsigned long size = g.size().first;
    std::vector<bool> visited(size);
    for(int i=0; i<size; i++)
      visited[i] = false;
    visited[start] = true;
    
    std::queue<node> q;
    q.push(start);

    int l = 0; //  the index of the current item in the current level
    int level = 1;
    int levelSize = 1;
    int nextLevelSize = 0;
    while(!q.empty()) {
      
      g.forNeighborsOf(q.front(), [&](node t) {
	if(!visited[t]) {
	  nextLevelSize++;
	  q.push(t);
	  visited[t] = true;
	  sum += level;
	}
      });

      if(l == levelSize - 1) {
	l = 0;
	level++;
	levelSize = nextLevelSize;
	nextLevelSize = 0;
      } else {
	l++;
      }

      q.pop();
    }
    std::cout << "Sum: " << sum << std::endl;
    
    hasRun = true;
  }
}
