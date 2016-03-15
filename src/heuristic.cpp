#include "heuristic.h"

void heuristic_algorithm(const binary_matrix& m, binary_matrix& result, bool verbose)
{
	if (verbose) cout << "*** Running the heuristic algorithm..." << endl;

	// computing the containment digraph of the entire matrix
	// this is a DAG

	ListDigraph H_M;
	ListDigraph::NodeMap<string> nodeLabel(H_M);
	ListDigraph::ArcMap<int64_t> arcWeight(H_M);
	vector<int> color(m.n_columns,-1);

	vector<ListDigraph::Node> nodes;

	// adding the nodes to H_M
	for (int j = 0; j < m.n_columns; j++)
	{
		ListDigraph::Node x;
		x = H_M.addNode();
		nodes.push_back(x);
		nodeLabel[x] = to_string(j + 1);
	}

	// adding the edges to H_M
	for (int j = 0; j < m.n_columns; j++)
	{
		for (int k = 0; k < m.n_columns; k++)
		{
			if (j != k and m.is_included_in(j,k))
			{
				ListDigraph::Arc a = H_M.addArc(nodes[j], nodes[k]);
				arcWeight[a] = 0;
			}
		}
	}

	vector<vector<string>* > paths;
	paths = solveMPCFlow(H_M, arcWeight, nodeLabel, 1);

	// getting the colors of the nodes of H_M from paths
	vector<vector<string>* >::iterator it = paths.begin();
	int color_index = 0;
	for(it = paths.begin(); it != paths.end(); ++it)
	{
		vector<string> currentpath = **it;
		vector<string>::iterator itp = currentpath.begin();
		for(itp=currentpath.begin(); itp != currentpath.end(); ++itp)
		{
			int column = stoi( *itp ) - 1;
			if (verbose) cout << column << ","; 
			if ((column != -1) and (color[column] == -1))
			{
				color[column] = color_index;
			}
		} 
		if (verbose) cout << endl;
		color_index++;
	}

	if (verbose) cout << "*** Computed the coloring" << endl;

	// creating the solution matrix
	result.cell.clear();

	result.column_names = m.column_names;
	result.row_names.clear();
	result.n_rows = 0;

	result.column_multiplicities = m.column_multiplicities;
	result.n_columns = m.n_columns;

	for (int r = 0; r < m.n_rows; r++)
	{
		// computing the colors of the columns having 1 on row r
		unordered_set<int> colors_r;
		for (int j = 0; j < m.n_columns; j++)
		{
			if (m.cell[r][j] == 1)
			{
				colors_r.insert(color[j]);
			}
		}
		// converting the set into vector
		vector<int> colors_r_vector;
		for (const auto& elem: colors_r) 
		{
    		colors_r_vector.push_back(elem);
		}

		for (int s = 0; s < colors_r_vector.size(); s++)
		{
			// creating the row name
			string new_name = m.row_names[r];
			if (colors_r_vector.size() > 1)
			{
				new_name += "_" + to_string(s + 1);
			}
			result.row_names.push_back(new_name);

			// creating the new row
			vector<bool> new_row;
			for (int j = 0; j < m.n_columns; j++)
			{
				if ((m.cell[r][j] == 1) and (color[j] == colors_r_vector[s]))
				{
					new_row.push_back(1);
				}
				else
				{
					new_row.push_back(0);
				}
			}
			result.cell.push_back(new_row);
		}
	}

	result.n_rows = result.cell.size();
	assert(result.is_conflict_free());
}

int compute_lower_bound(const binary_matrix& m, bool verbose)
{
	if (verbose) cout << "*** Computing the lower bound on the number of rows in the conflict-free split row matrix..." << endl;

	// computing the containment digraph of the entire matrix
	// this is a DAG

	ListDigraph H_M;
	vector<ListDigraph::Node> nodes;

	// adding the nodes to H_M
	for (int j = 0; j < m.n_columns; j++)
	{
		ListDigraph::Node x;
		x = H_M.addNode();
		nodes.push_back(x);
	}

	// adding the edges to H_M
	for (int j = 0; j < m.n_columns; j++)
	{
		for (int k = 0; k < m.n_columns; k++)
		{
			if (j != k and m.is_included_in(j,k))
			{
				ListDigraph::Arc a = H_M.addArc(nodes[j], nodes[k]);
			}
		}
	}

	int lower_bound = 0;
	for (int i = 0; i < m.n_rows; i++)
	{
		ListDigraph::NodeMap<bool> subdigraph_node(H_M, false);
		// creating the subdigraph of H_M for row i
		for (int j = 0; j < m.n_columns; j++)
		{
			if (m.cell[i][j] == 1)
			{
				subdigraph_node[nodes[j]] = true;
			}
		}

		FilterNodes<ListDigraph> subdigraph(H_M, subdigraph_node);

		// solving the minimum path cover problem
		vector<vector<string>* > paths;
		// The following line unfortunately doesn't work
		// paths = solveMPCFlow(subdigraph, arcWeight, nodeLabel, 1);
		// so we need to copy the digraph
		ListDigraph subdigraph_LD;
		DigraphCopy<FilterNodes<ListDigraph>, ListDigraph> cg(subdigraph, subdigraph_LD);
		cg.run();

		ListDigraph::NodeMap<string> nodeLabel(H_M);
		ListDigraph::ArcMap<int64_t> arcWeight(H_M);
		for (ListDigraph::ArcIt arc(subdigraph_LD); arc != INVALID; ++arc) 
		{
			arcWeight[arc] = 0;
		}
		for (ListDigraph::NodeIt node(subdigraph_LD); node != INVALID; ++node) 
		{
			nodeLabel[node] = "x";
		}
		paths = solveMPCFlow(subdigraph_LD, arcWeight, nodeLabel, 1);

		lower_bound += paths.size();
	}

	return lower_bound;	
}