// Author: Ruxandra Barbulescu

#include <iostream>
#include <lemon/concepts/graph.h>
#include <lemon/list_graph.h>
#include <lemon/network_simplex.h>
#include <lemon/cost_scaling.h>
#include <lemon/lgf_reader.h>
#include <lemon/lgf_writer.h>
#include <lemon/connectivity.h>
#include <lemon/maps.h>
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

int64_t createFlowNetwork(ListDigraph& g, ListDigraph::ArcMap<int64_t>& arcWeight, ListDigraph::NodeMap<string>& nodeLabel, 
							ListDigraph& flowNetwork, ListDigraph::ArcMap<int64_t>& lowerMap, ListDigraph::ArcMap<int64_t>& upperMap, 
							ListDigraph::ArcMap<int64_t>& costMap, ListDigraph::NodeMap<int64_t>& supplyMap, ListDigraph::NodeMap<string>& labelMap,
							ListDigraph::NodeMap<ListDigraph::Node>& flowNodetoGraphNode);

vector<vector<string>* > solveMPCFlow(ListDigraph& g, ListDigraph::ArcMap<int64_t>& arcWeight, 
												ListDigraph::NodeMap<string>& nodeLabel, int method);