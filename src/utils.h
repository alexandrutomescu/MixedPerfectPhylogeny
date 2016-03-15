
#ifndef UTILS_H_INCLUDED
#define UTILS_H_INCLUDED

#include <string>
#include <fstream>
#include <unordered_set>
#include <assert.h>
#include <ctime>
#include <stdlib.h>

#include "OptionParser.h"

using namespace std;

class binary_matrix
{
public:	
	vector< vector<bool> > cell;
	uint n_rows;
	uint n_columns;
	vector< vector<string> > column_multiplicities; 
	// column_multiplicities[i] = {names of columns that are identical to the i-th column, excluding i}
	vector<string> column_names;
	vector<string> row_names;

	bool read_from_file(string inputFileName);
	bool print_to_file(string outputFileName) const;
	bool is_conflict_free() const;

	bool is_included_in(uint col_i, uint col_j) const;	
	bool are_conflicting(uint col_i, uint col_j) const;
	bool are_equal(uint col_i, uint col_j) const;

	void hide_duplicate_columns(bool verbose = false); 
	void show_duplicate_columns();
};

bool get_vector_from_string(string s, vector<bool>& result);

void create_random_matrix(binary_matrix& matrix, uint m, uint n);

#endif // UTILS_H_INCLUDED
