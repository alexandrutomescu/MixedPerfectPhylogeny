// Author Ruxandra Barbulescu

#include "bp.h"

int minCostMaxMatching(ListGraph& g, ListGraph::EdgeMap<int64_t>& edgeWeight, 
														ListGraph::NodeMap<string>& nodeLabel, ListGraph::EdgeMap<bool>& matchingMap)
{
	ListGraph maxGraph;
	ListGraph::NodeMap<string> labelMaxMap(maxGraph);
	ListGraph::EdgeMap<int64_t> maxWeightMap(maxGraph);
	ListGraph::EdgeMap<ListGraph::Edge> maxEdgetoGraphEdge(maxGraph);

	// map between nodes in maxGraph and nodes in g
    ListGraph::NodeMap<ListGraph::Node> maxNodetoGraphNode(maxGraph);
    ListGraph::NodeMap<ListGraph::Node> graphNodetoMaxNode1(g);
    ListGraph::NodeMap<ListGraph::Node> graphNodetoMaxNode2(g);

    ListGraph::Node firstNode, currentNode;
	int64_t infinite = 1;

	// logbp.open(logbpfilename, std::ios_base::app);

	//get infinite number
	for (ListGraph::EdgeIt edge(g); edge != INVALID; ++edge) 
	{
		infinite += edgeWeight[edge];
	}
	//logbp << "Infinite for 'bp' graph: " << infinite << endl;

    // node iteration
	for (ListGraph::NodeIt graphNode(g); graphNode != INVALID; ++graphNode) 
	{
		// create two copies of the original node
		ListGraph::Node maxNode1 = maxGraph.addNode();
		ListGraph::Node maxNode2 = maxGraph.addNode();

		maxNodetoGraphNode.set(maxNode1,graphNode);
		graphNodetoMaxNode1.set(graphNode,maxNode1);
		maxNodetoGraphNode.set(maxNode2,graphNode);
		graphNodetoMaxNode2.set(graphNode,maxNode2);
		
		labelMaxMap.set(maxNode1,"1_"+nodeLabel[graphNode]);
		labelMaxMap.set(maxNode2,"2_"+nodeLabel[graphNode]);

		ListGraph::Edge a = maxGraph.addEdge(maxNode1, maxNode2);
		maxWeightMap[a] = -infinite;
	}

	for (ListGraph::EdgeIt edge(g); edge != INVALID; ++edge)
	{
		ListGraph::Edge a = maxGraph.addEdge(graphNodetoMaxNode1[g.u(edge)], graphNodetoMaxNode1[g.v(edge)]);
		maxWeightMap[a] = -edgeWeight[edge];
		maxEdgetoGraphEdge[a] = edge;
		ListGraph::Edge b = maxGraph.addEdge(graphNodetoMaxNode2[g.u(edge)], graphNodetoMaxNode2[g.v(edge)]);
		maxWeightMap[b] = -edgeWeight[edge];
	}
/*
	for (ListGraph::EdgeIt maxEdge(maxGraph); maxEdge != INVALID; ++maxEdge)
	{
		logbp << labelMaxMap[maxGraph.u(maxEdge)] << " -> " << labelMaxMap[maxGraph.v(maxEdge)] << " = "<< maxWeightMap[maxEdge] << endl;
	}
*/
	// Max Weighted Matching
	MaxWeightedPerfectMatching<ListGraph, ListGraph::EdgeMap<int64_t> > maxMatcher(maxGraph, maxWeightMap);
	maxMatcher.run();

	// build matching map
	//logbp << "Matching bp: " << endl;
	for (ListGraph::EdgeIt rMaxEdge(maxGraph); rMaxEdge != INVALID; ++rMaxEdge)
	{
		// matchingMap[maxEdgetoGraphEdge[rMaxEdge]] = false;
		if (maxMatcher.matching(rMaxEdge))
		{
			if ((labelMaxMap[maxGraph.u(rMaxEdge)].substr(0,2) == "1_") 
				and (labelMaxMap[maxGraph.v(rMaxEdge)].substr(0,2) == "1_"))
			{
				matchingMap[maxEdgetoGraphEdge[rMaxEdge]] = true;
				//logbp << labelMaxMap[maxGraph.u(rMaxEdge)] << " -> " << labelMaxMap[maxGraph.v(rMaxEdge)] << " = " << maxWeightMap[rMaxEdge] << endl;
			}
		}
	}

	// logbp.close();
	
	return 0;
}

bool hasMatchingEdges(ListGraph& g, ListGraph::Node node, ListGraph::EdgeMap<bool>& matchingMap)
{
	bool hasMatchEdge = false;
	for(ListGraph::IncEdgeIt incEdge(g,node); incEdge != INVALID; ++incEdge)
	{
		if (matchingMap[incEdge] == true) 
		{
			hasMatchEdge = true;
			return hasMatchEdge;
		}
	}
	return hasMatchEdge;
}

vector<vector<string>* > solveMPCBp(ListDigraph& g, ListDigraph::ArcMap<int64_t>& arcWeight, ListDigraph::NodeMap<string>& nodeLabel)
{
	ListGraph bpGraph;
	ListGraph::NodeMap<string> labelBpMap(bpGraph);
	ListGraph::EdgeMap<int64_t> bpWeightMap(bpGraph);
	ListGraph::EdgeMap<bool> matchingMap(bpGraph);

	ListGraph::Node currentNodeA, currentNodeB;
	int64_t totalBpCost = 0;
	int64_t bpPaths = 0;

	vector<ListDigraph::NodeMap<ListDigraph::Arc>* > precMapVector;
	ListDigraph::NodeMap<ListDigraph::Arc> precNodeMap(g);
	ListDigraph::Node precNode;
	ListDigraph::NodeMap<int64_t> nodePos(g);
	int64_t pos;
	vector<vector<string>* > paths;
	vector<string> matchingPath, path;

	// logbp.open(logbpfilename, std::ios_base::app);

	// handle one-node graph
    if(countNodes(g) == 1)
    {
    	for (ListDigraph::NodeIt graphNode(g); graphNode != INVALID; ++graphNode) 
		{
			paths.clear(); paths.push_back(new vector<string>);
			path.clear(); path.push_back(nodeLabel[graphNode]); path.push_back("0");
			*paths[0] = path;
			return paths;
		}
    }

	// map between nodes in bpGraph and nodes in g
    ListGraph::NodeMap<ListDigraph::Node> bpNodetoGraphNode(bpGraph);
    ListDigraph::NodeMap<ListGraph::Node> graphNodetoBpNode1(g);
    ListDigraph::NodeMap<ListGraph::Node> graphNodetoBpNode2(g);

    // node iteration
	for (ListDigraph::NodeIt graphNode(g); graphNode != INVALID; ++graphNode) 
	{
		// create two copies of the original node
		ListGraph::Node bpNode1 = bpGraph.addNode();
		ListGraph::Node bpNode2 = bpGraph.addNode();

		bpNodetoGraphNode.set(bpNode1,graphNode);
		graphNodetoBpNode1.set(graphNode,bpNode1);
		bpNodetoGraphNode.set(bpNode2,graphNode);
		graphNodetoBpNode2.set(graphNode,bpNode2);
		labelBpMap.set(bpNode1,nodeLabel[graphNode]+"_1");
		labelBpMap.set(bpNode2,nodeLabel[graphNode]+"_2");
	}

	time_t  timebsp = time(NULL);
	cout << "Before dag_sp: " << ctime(&timebsp) << endl;
    // dag shortest paths: dynamic programming -> O(V(E+V))
    pos = -1;
	for (ListDigraph::NodeIt graphNode(g); graphNode != INVALID; ++graphNode) 
	{
		pos++;
		// DAG Shortest Path
		ListDigraph::NodeMap<int64_t> shPathMap(g);
		DagShortestPath<ListDigraph, ListDigraph::ArcMap<int64_t> > dag_SP(g, arcWeight);
		dag_SP.distMap(shPathMap).predMap(precNodeMap).run(graphNode);

		nodePos[graphNode] = pos;
		// save predMap into vector of maps
		precMapVector.push_back(new ListDigraph::NodeMap<ListDigraph::Arc>(g));
		for(ListDigraph::NodeIt copyNode(g); copyNode != INVALID; ++copyNode)
		{
			(*precMapVector[pos])[copyNode] = precNodeMap[copyNode];
		}

    	//logbp << "Root node: " << nodeLabel[graphNode] << endl;
    	for (ListDigraph::NodeIt nn(g); nn != INVALID; ++nn)
    	{
    		//logbp << nodeLabel[g.source((*precMapVector[pos])[nn])] << " " << endl;
    		//logbp << "Current node: "<< nodeLabel[nn] << " distance: "<< shPathMap[nn] << endl;
    		if ((shPathMap[nn] > 0) and (shPathMap[nn] != int64_t_MAX))
    		{
    			ListGraph::Edge a = bpGraph.addEdge(graphNodetoBpNode1[graphNode], graphNodetoBpNode2[nn]);
    			bpWeightMap[a] = shPathMap[nn];
    		}
    	}
	}

	time_t  timeasp = time(NULL);
	cout << "After dag_sp: " << ctime(&timeasp) << endl;
/*
	// list bp graph
	//logbp << "Bp graph has " << countNodes(bpGraph) << " nodes and " << countEdges(bpGraph) << " arcs." << endl;
	for (ListGraph::EdgeIt bpEdge(bpGraph); bpEdge != INVALID; ++bpEdge)
	{
		logbp << labelBpMap[bpGraph.u(bpEdge)] << " -> " << labelBpMap[bpGraph.v(bpEdge)] << ": " << bpWeightMap[bpEdge] << endl;
	}
*/
	// logbp.close();
	// Min Cost Max Bipartite Matching using Max Weighted Matching
	minCostMaxMatching(bpGraph, bpWeightMap, labelBpMap, matchingMap);

	time_t  timeamm = time(NULL);
	cout << "After dag_sp: " << ctime(&timeamm) << endl;
	// logbp.open(logbpfilename, std::ios_base::app);
/*
	// Total cost
	for (ListGraph::EdgeIt edge(bpGraph); edge != INVALID; ++edge)
	{
		if (matchingMap[edge])
		{
			totalBpCost += bpWeightMap[edge];
		}
	}
*/
	// number of paths: sum of nodes in B with no incident edges
	//logbp << "Bp paths..." << endl;
	paths.clear();
	for (ListGraph::NodeIt node(bpGraph); node != INVALID; ++node)
	{
		if (labelBpMap[node].substr(labelBpMap[node].length()-2, 2) == "_2")
		{
			// Node B: (no inc edges) or (has inc edges but none matching) ==> node A is start of a path
			if (!hasMatchingEdges(bpGraph,node,matchingMap)) 
			{
				bpPaths++;
				totalBpCost = 0; path.clear();
				paths.push_back(new vector<string>);
				if(!hasMatchingEdges(bpGraph,graphNodetoBpNode1[bpNodetoGraphNode[node]],matchingMap)) // path with single node A
				{
					matchingPath.clear();
					matchingPath.push_back(nodeLabel[bpNodetoGraphNode[node]]);
					path.insert(path.end(), matchingPath.begin(), matchingPath.end());
				}
				else
				{
					currentNodeA = graphNodetoBpNode1[bpNodetoGraphNode[node]];
					int64_t flag = 0;

					while(countIncEdges(bpGraph,currentNodeA) > 0)
					{
						//logbp << "Current node A: " << labelBpMap[currentNodeA] << endl;
						matchingPath.clear(); flag++;
						if(!hasMatchingEdges(bpGraph, currentNodeA, matchingMap)) // node A is end of path
						{
							break;
						} else
						{
							for(ListGraph::IncEdgeIt incEdge(bpGraph,currentNodeA); incEdge != INVALID; ++incEdge)
							{
								//logbp << "  Edge to: " << labelBpMap[bpGraph.v(incEdge)];
								if (matchingMap[incEdge] == true)
								{
									totalBpCost += bpWeightMap[incEdge];
									currentNodeB = bpGraph.v(incEdge);
									precNode = bpNodetoGraphNode[currentNodeB];
									//logbp << " is a matching edge." << endl;
									// build original path
									while(nodeLabel[precNode] != nodeLabel[bpNodetoGraphNode[currentNodeA]])
									{
										// cout << endl << "Matching path adding node " << nodeLabel[precNode] << endl;
										matchingPath.push_back(nodeLabel[precNode]);
										precNode = g.source((*precMapVector[nodePos[bpNodetoGraphNode[currentNodeA]]])[precNode]);
										//logbp << "   Precedent node is : " << nodeLabel[precNode] << endl;
									}
									break;
								}
								//logbp << endl;
							}
							if (flag == 1) // node A is start of path
							{
								matchingPath.push_back(nodeLabel[bpNodetoGraphNode[currentNodeA]]);
							}
							reverse(matchingPath.begin(), matchingPath.end());
							path.insert(path.end(), matchingPath.begin(), matchingPath.end());
							currentNodeA = graphNodetoBpNode1[bpNodetoGraphNode[currentNodeB]];
						}
					}
				}
				stringstream sum; sum << totalBpCost;
				path.push_back(sum.str());
				*paths[bpPaths-1] = path;
			}
		}
	}

	// logbp.close();
	
	return paths;
}
