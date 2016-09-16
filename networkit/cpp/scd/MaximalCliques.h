#ifndef CLIQUE_H_
#define CLIQUE_H_

#include "../graph/Graph.h"

#include <unordered_map>

namespace NetworKit {

class MaximalCliques {

public:
	MaximalCliques(const Graph& G);

	std::vector<std::vector<node> > run();

protected:
	const Graph& G;

	std::vector<std::vector<node> > tomita(std::vector<node>& pxvector, std::vector<index>& pxlookup, uint32_t xbound, uint32_t xpbound, uint32_t pbound, std::vector<node>& r);

	node findPivot(std::vector<node>& pxvector, std::vector<index>& pxlookup, uint32_t xbound, uint32_t xpbound, uint32_t pbound);

	std::vector<node> getDegeneracyOrdering();

};

}

#endif /* CLIQUE_H_ */
