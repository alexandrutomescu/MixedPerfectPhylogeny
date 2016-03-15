// Author: Ruxandra Barbulescu

#include "flow.h"

int64_t createFlowNetwork(const ListDigraph& g, ListDigraph::ArcMap<int64_t>& arcWeight, ListDigraph::NodeMap<string>& nodeLabel, 
							ListDigraph& flowNetwork, ListDigraph::ArcMap<int64_t>& lowerMap, ListDigraph::ArcMap<int64_t>& upperMap, 
							ListDigraph::ArcMap<int64_t>& costMap, ListDigraph::NodeMap<int64_t>& supplyMap, ListDigraph::NodeMap<string>& labelMap,
							ListDigraph::NodeMap<ListDigraph::Node>& flowNodetoGraphNode)
{
    int64_t infinite = 1;
    int64_t gCountNodes = countNodes(g);

    // map correspondence between nodes in g and nodes in flowNetwork
    ListDigraph::NodeMap<ListDigraph::Node> graphNodetoFlowNode1(g);
    ListDigraph::NodeMap<ListDigraph::Node> graphNodetoFlowNode2(g);

    // logflow.open(logflowfname, std::ios_base::app);

    // get infinite number
    for (ListDigraph::ArcIt arc(g); arc != INVALID; ++arc) 
	{
		infinite += arcWeight[arc];
	}
    //logflow << "Infinite for digraph: " << infinite << endl;

	// adding star source and sink
	ListDigraph::Node s_star = flowNetwork.addNode(); 
	ListDigraph::Node t_star = flowNetwork.addNode(); 
	ListDigraph::Arc a = flowNetwork.addArc(s_star,t_star); lowerMap.set(a,0); upperMap.set(a,gCountNodes); costMap.set(a,0);
	labelMap[s_star] = "ss"; labelMap[t_star] = "ts";

	// nodes iteration
	for (ListDigraph::NodeIt graphNode(g); graphNode != INVALID; ++graphNode) 
	{
		// double every node
		ListDigraph::Node flowNode1 = flowNetwork.addNode();
		ListDigraph::Node flowNode2 = flowNetwork.addNode();
		flowNodetoGraphNode.set(flowNode1,graphNode); flowNodetoGraphNode.set(flowNode2,graphNode);
		graphNodetoFlowNode1.set(graphNode,flowNode1); graphNodetoFlowNode2.set(graphNode,flowNode2);
		labelMap.set(flowNode1,nodeLabel[graphNode]+"_1"); labelMap.set(flowNode2,nodeLabel[graphNode]+"_2");

		// create arc between the twin nodes
		ListDigraph::Arc inArc = flowNetwork.addArc(flowNode1,flowNode2);
		lowerMap.set(inArc,1); upperMap.set(inArc,gCountNodes); costMap.set(inArc,0);

		ListDigraph::Arc a = flowNetwork.addArc(s_star,flowNode1);
		lowerMap.set(a,0); upperMap.set(a,gCountNodes); costMap.set(a,infinite);
		if (countInArcs(g,graphNode) == 0)
		{
			lowerMap.set(a,1);
		}

		ListDigraph::Arc b = flowNetwork.addArc(flowNode2,t_star);
		lowerMap.set(b,0); upperMap.set(b,gCountNodes); costMap.set(b,infinite);
		if (countOutArcs(g,graphNode) == 0)
		{
			lowerMap.set(b,1); //costMap.set(b,0);
		}
	}

	// arcs iteration
	for (ListDigraph::ArcIt graphArc(g); graphArc != INVALID; ++graphArc)
	{
		ListDigraph::Arc flowArc;
		ListDigraph::Node sourceNode = graphNodetoFlowNode2[g.source(graphArc)];
		ListDigraph::Node targetNode = graphNodetoFlowNode1[g.target(graphArc)];
		
		flowArc = flowNetwork.addArc(sourceNode,targetNode);

		// set maps' values for new flow arc
		lowerMap.set(flowArc,0); upperMap.set(flowArc,gCountNodes);
		costMap[flowArc] = arcWeight[graphArc];
	}


	for (ListDigraph::NodeIt flowNode(flowNetwork); flowNode != INVALID; ++flowNode) 
	{
		supplyMap[flowNode] = 0;
	}

	int64_t supply_s_star = gCountNodes;
	int64_t supply_t_star = -gCountNodes;

	supplyMap[s_star] = supply_s_star;
	supplyMap[t_star] = supply_t_star;

	// list flow network
	//logflow << "Flow network has " << countNodes(flowNetwork) << " nodes and " << countArcs(flowNetwork) << " arcs." << endl;
	for (ListDigraph::ArcIt flowArc(flowNetwork); flowArc != INVALID; ++flowArc)
	{
		//logflow << labelMap[flowNetwork.source(flowArc)] << " -> " << labelMap[flowNetwork.target(flowArc)] << ":\t" 
		//	<< lowerMap[flowArc] << " " << upperMap[flowArc] << " " << costMap[flowArc] << endl;
	}

	// logflow.close();
	return 0;
}

vector<vector<string>* > solveMPCFlow(const ListDigraph& g, ListDigraph::ArcMap<int64_t>& arcWeight, 
												ListDigraph::NodeMap<string>& nodeLabel, int method)
{
	// create empty flow network and associated maps
	ListDigraph flowNetwork;
	ListDigraph::ArcMap<int64_t> lowerMap(flowNetwork), upperMap(flowNetwork);
    ListDigraph::ArcMap<int64_t> costMap(flowNetwork);
    ListDigraph::NodeMap<int64_t> supplyMap(flowNetwork);
    ListDigraph::NodeMap<string> labelMap(flowNetwork);
    ListDigraph::NodeMap<ListDigraph::Node> flowNodetoGraphNode(flowNetwork);
    vector<vector<string>* > paths;

    // handle one-node graph
    if(countNodes(g) == 1)
    {
    	for (ListDigraph::NodeIt graphNode(g); graphNode != INVALID; ++graphNode) 
		{
			paths.clear();
			paths.push_back(new vector<string>);
			vector<string> path;
			path.push_back(nodeLabel[graphNode]);
			path.push_back("0");
			*paths[0] = path;
			return paths;
		}
    }

	createFlowNetwork(g, arcWeight, nodeLabel, flowNetwork, lowerMap, upperMap, costMap, supplyMap, labelMap, flowNodetoGraphNode);

	// logflow.open(logflowfname, std::ios_base::app);

	ListDigraph::ArcMap<int64_t> flowMap(flowNetwork); 
	// Network Simplex Algorithm
	switch (method) {
		case 1:
		{
			NetworkSimplex<ListDigraph, int64_t> ns(flowNetwork);
    		ns.lowerMap(lowerMap).upperMap(upperMap).costMap(costMap).supplyMap(supplyMap);
    		NetworkSimplex<ListDigraph, int64_t>::ProblemType res_ns = ns.run(NetworkSimplex<ListDigraph, int64_t>::BLOCK_SEARCH); //ALTERING_LIST  FIRST_ELIGIBLE BEST_ELIGIBLE BLOCK_SEARCH CANDIDATE_LIST

    		if (res_ns != NetworkSimplex<ListDigraph, int64_t>::OPTIMAL)
		    {
		    	cout << "ERROR: flow not found " << res_ns << endl;
		    }
		    ns.flowMap(flowMap);    
			break;
		}
		// case 2:
		// {
		// 	CapacityScaling<ListDigraph, int64_t> cs(flowNetwork);
  //   		cs.lowerMap(lowerMap).upperMap(upperMap).costMap(costMap).supplyMap(supplyMap);
  //   		CapacityScaling<ListDigraph, int64_t>::ProblemType res_cs = cs.run();

		// 	if (res_cs != CapacityScaling<ListDigraph, int64_t>::OPTIMAL)
		// 	{
		// 		cout << "ERROR: flow not found " << res_cs << endl;
		// 	}
		// 	cs.flowMap(flowMap);    
		// 	break;
		// }
	}
	
    ListDigraph::Node firstNode, currentNode;
    int64_t flowPathNr = 0, flowSum = 0;

    // list flow on every arc
    // logflow << "Arcs with flow:" << endl;
    for (ListDigraph::ArcIt flowArc(flowNetwork); flowArc != INVALID; ++flowArc)
	{/*
		if(flowMap[flowArc] > 0)
		{
			string n1 = labelMap[flowNetwork.source(flowArc)];
			string n2 = labelMap[flowNetwork.target(flowArc)];
			if(n1.substr(0,n1.length()-2) != n2.substr(0,n2.length()-2))
			{
				if(n1 == "ss") { logflow << n1 << " -> "; }
				else { logflow << n1.substr(0,n1.length()-2) << " -> "; }
				if(n2 == "ts") { logflow << n2 << ": "; }
				else { logflow << n2.substr(0,n2.length()-2) << ": "; }
				logflow << flowMap[flowArc] << endl;
			}
		}
*/
		// init source node and flow sum from source node
		if (labelMap[flowNetwork.source(flowArc)] == "ss")
		{
			firstNode = flowNetwork.source(flowArc);
			// skip paths ss -> ts
			if (labelMap[flowNetwork.target(flowArc)] != "ts")
			{
				flowPathNr += flowMap[flowArc];
			}
		}
	}
	
	// build all paths from min path cover
	// number of paths from min path cover: sum of flows on outgoing arcs from star source 
	paths.clear();
	for (int i=0; i < flowPathNr; i++)
	{
		paths.push_back(new vector<string>);
		vector<string> path;
		currentNode = firstNode;
		flowSum = 0;
		while (labelMap[currentNode] != "ts")
		{
			for (ListDigraph::OutArcIt outa(flowNetwork,currentNode); outa != INVALID; ++outa)
			{	
				// choose arc with flow
				if(flowMap[outa] > 0)
				{
					flowMap[outa]--;
					currentNode = flowNetwork.target(outa);

					string n1 = labelMap[flowNetwork.source(outa)];
					string n2 = labelMap[flowNetwork.target(outa)];
					if(n1.substr(0,n1.length()-2) != n2.substr(0,n2.length()-2))
					{
						if((n1 != "ss") and (n2 != "ts")) { flowSum += costMap[outa]; }
						path.push_back(n2.substr(0,n2.length()-2));
					}
					break;
				}
			}
		}
		path.pop_back();
		stringstream sum; sum << flowSum;
		path.push_back(sum.str());
		*paths[i] = path;
	}

	// logflow.close();

    return paths;

}
