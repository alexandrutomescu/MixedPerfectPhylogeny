// Author Ruxandra Barbulescu

#ifndef BP_H_INCLUDED
#define BP_H_INCLUDED

#include <iostream>
#include <lemon/concepts/graph.h>
#include <lemon/list_graph.h>
#include <lemon/matching.h>
#include <lemon/lgf_reader.h>
#include <lemon/lgf_writer.h>
#include <lemon/connectivity.h>
#include <lemon/maps.h>
#include "dag_shortest_path.h"
#include <vector>
#include <queue>
#include <map>
#include <set>
#include <string>
#include <math.h>
#include <fstream>
#include <sstream>

#define int64_t_MAX std::numeric_limits<int64_t>::max()

using namespace lemon;
using namespace std;


int minCostMaxMatching(ListGraph& g, ListGraph::EdgeMap<int64_t>& edgeWeight, 
														ListGraph::NodeMap<string>& nodeLabel, ListGraph::EdgeMap<bool>& matchingMap);

bool hasMatchingEdges(ListGraph& g, ListGraph::Node node, ListGraph::EdgeMap<bool>& matchingMap);

vector<vector<string>* > solveMPCBp(ListDigraph& g, ListDigraph::ArcMap<int64_t>& arcWeight, ListDigraph::NodeMap<string>& nodeLabel);

#endif // BP_H_INCLUDED
