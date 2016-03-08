#include "draw_tree.h"

struct tree_leaf 
{
	string id;
	unordered_set<int> elements;
};

vector<string> node_colors = {"#73B2F9", "#EC7877", "#8ACB69", "#F5DB5D", "#BD80E5", "#F2A253", "#A37242", "#3D73A3", "#43A373", "#724AA4"};

void us_isect(unordered_set<int> &out,
    	const unordered_set<int> &in1,
    	const unordered_set<int> &in2)
{
    out.clear();
    if (in2.size() < in1.size()) {
    	us_isect(out, in2, in1);
    	return;
    }
    for (unordered_set<int>::const_iterator it = in1.begin(); it != in1.end(); ++it)
    {
    	if (in2.find(*it) != in2.end())
    		out.insert(*it);
    }
}

void draw_tree_in_dot_file(const binary_matrix& m, string outputFileName)
{
	// this function requires that there are no duplicate columns in the matrix

	binary_matrix a = m;
	// a.hide_duplicate_columns();

	ofstream outputFile;
	outputFile.open(outputFileName);
	outputFile << "digraph {" << endl;

	vector< tree_leaf > leaves;
	int next_available_color = 0;
	map<string,string> color; 

	for (int i = 0; i < a.n_rows; i++)
	{
		tree_leaf new_leaf;
		new_leaf.id = a.row_names[i];

		for (int j = 0; j < a.n_columns; j++)
		{
			if (a.cell[i][j] == 1)
			{
				new_leaf.elements.insert(j);
			}
		}

		leaves.push_back(new_leaf);

		size_t pos = new_leaf.id.find("_", 0); //store the position of the delimiter
		string type;
		if (pos != string::npos)
		{
			type = new_leaf.id.substr(0, pos);
		}
		else
		{
			type = new_leaf.id;
		}
		if (color.count(type) == 0)
		{
			color[type] = node_colors[next_available_color];	
			next_available_color++;
			if (next_available_color == node_colors.size())
			{
				next_available_color = 0;
			}
		}
		outputFile << new_leaf.id << "[shape=box,style=filled,fontsize=28,fillcolor=\"" << color[type] << "\"];" << endl;
	}

	int new_node_counter = 0;

	while (leaves.size() > 1)
	{
		unordered_set<int> max_intersection;
		int max_leaf1, max_leaf2;
		// find the two leaves with max cardinaliy of intersection
		for (int i = 0; i < leaves.size(); i++)
	    {
	    	for (int j = i + 1; j < leaves.size(); j++)
	    	{
	    		unordered_set<int> intersection;
	    		us_isect(intersection, leaves[i].elements, leaves[j].elements);
	    		if (intersection.size() >= max_intersection.size())
	    		{
	    			max_intersection = intersection;
	    			max_leaf1 = i;
	    			max_leaf2 = j;
	    		}
	    	}
	    }
	    // cout << "Largest intersection is between " << leaves[max_leaf1].id << " and " << leaves[max_leaf2].id << endl;

	    // inserting the parent of these leaves into the tree
	    tree_leaf parent_leaf;
	    //parent_leaf.id = leaves[max_leaf1].id + "_" + leaves[max_leaf2].id;
		parent_leaf.id = "Int" + to_string(new_node_counter);
	    new_node_counter++;
	    parent_leaf.elements = max_intersection;

	    outputFile << parent_leaf.id << "[shape=point,style=filled,fillcolor=black,label=\"\"];" << endl;
		string connector = " -> ";
		if (leaves[max_leaf1].id.find("Int") != string::npos)
		{
			connector = " -> ";
		}
		outputFile << parent_leaf.id << connector << leaves[max_leaf1].id << "[arrowhead=none];" << endl;			
		
		if (leaves[max_leaf2].id.find("Int") != string::npos)
		{
			outputFile << parent_leaf.id << " -> " << leaves[max_leaf2].id << "[arrowhead=none];" << endl;
		}
		else
		{
			outputFile << parent_leaf.id << " -> " << leaves[max_leaf2].id << "[arrowhead=none];" << endl;	
		}
		

	    leaves.push_back(parent_leaf);
	    // removing them from the leaves
	    for (int i = max_leaf2; i < leaves.size() - 1; i++)
	    {
	    	leaves[i] = leaves[i + 1];
	    }
	    leaves.pop_back();
	    for (int i = max_leaf1; i < leaves.size() - 1; i++)
	    {
	    	leaves[i] = leaves[i + 1];
	    }
	    leaves.pop_back();

	}

	outputFile << "}" << endl;

	outputFile.close();


}