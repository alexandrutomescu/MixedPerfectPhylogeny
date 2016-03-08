#include "poly_alg.h"

bool check_special_case(const binary_matrix& m, bool verbose)
{
	if (verbose) cout << "*** Checking if the input matrix belong to the special class of polynomial solvable matrices described in the paper" << endl;

	// "no column is included in both columns in a pair of conflicting columns"

	for (int i = 0; i < m.n_columns; i++)
	{
		for (int j = i + 1; j < m.n_columns; j++)
		{
			if (m.are_conflicting(i, j))
			{
				for (int k = 0; k < m.n_columns; k++)
				{
					if (k != i and k != j)
					{
						if (m.is_included_in(k, i) and m.is_included_in(k, j))
						{
							if (verbose) cout << "ERROR: Column " << m.column_names[k] << " is included in both conflicting columns " << m.column_names[i] << "," << m.column_names[j] << endl;
							return false;
						}
					}
				}
			}
		}
	}

	return true;
}

void polynomial_algorithm(const binary_matrix& m, binary_matrix& result, bool verbose)
{
	if (verbose) cout << "*** Running the polynomial algorithm..." << endl;

	// computing the containment graph of the entire matrix
	bool H_M[m.n_columns][m.n_columns];
	for (int j = 0; j < m.n_columns; j++)
	{
		for (int k = 0; k < m.n_columns; k++)
		{
			if (j == k)
			{
				H_M[j][k] = false;
				continue;
			}

			if (m.is_included_in(j,k))
			{
				H_M[j][k] = true;
			}
			else
			{
				H_M[j][k] = false;
			}
		}
	}

	// creating the solution matrix
	result.cell.clear();

	result.column_names = m.column_names;
	result.row_names.clear();
	result.n_rows = 0;

	result.column_multiplicities = m.column_multiplicities;
	result.n_columns = m.n_columns;


	for (int r = 0; r < m.n_rows; r++)
	{
		// computing the sources of H_M_r
		vector<int> sources;
		for (int j = 0; j < m.n_columns; j++)
		{
			if (m.cell[r][j] == 1)
			{
				// checking if column j is source in H_M_r
				bool col_j_source = true;
				for (int k = 0; k < m.n_columns; k++)
				{
					if ((j != k) and (m.cell[r][k] == 1) and m.is_included_in(k, j))
					{
						col_j_source = false;
						break;
					}
				}
				if (col_j_source)
				{
					sources.push_back(j);
				}
			}
		}

		if (verbose) cout << "--- Row " << r << " has " << sources.size() << " sources" << endl;

		// creating the sources.size() rows of result corresponding to row r
		for (int s = 0; s < sources.size(); s++)
		{
			// creating the row name
			string new_name = m.row_names[r];
			if (sources.size() > 1)
			{
				new_name += "_" + to_string(s + 1);
			}
			result.row_names.push_back(new_name);

			// creating the new row
			vector<bool> new_row;
			for (int j = 0; j < m.n_columns; j++)
			{
				if ((m.cell[r][j] == 1) and (sources[s] == j or m.is_included_in(sources[s], j)))
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