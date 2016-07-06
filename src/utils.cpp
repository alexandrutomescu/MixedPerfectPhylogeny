#include "utils.h"

std::istream& safeGetline(std::istream& is, std::string& t)
{
    t.clear();

    // The characters in the stream are read one-by-one using a std::streambuf.
    // That is faster than reading them one-by-one using the std::istream.
    // Code that uses streambuf this way must be guarded by a sentry object.
    // The sentry object performs various tasks,
    // such as thread synchronization and updating the stream state.

    std::istream::sentry se(is, true);
    std::streambuf* sb = is.rdbuf();

    for(;;) {
        int c = sb->sbumpc();
        switch (c) {
        case '\n':
            return is;
        case '\r':
            if(sb->sgetc() == '\n')
                sb->sbumpc();
            return is;
        case EOF:
            // Also handle the case when the last line has no line ending
            if(t.empty())
                is.setstate(std::ios::eofbit);
            return is;
        default:
            t += (char)c;
        }
    }
}

bool binary_matrix::read_from_file(string inputFileName)
{
	cout << "*** Reading the matrix from file " << inputFileName << endl;

	ifstream inputFile;
	inputFile.open(inputFileName);

	string line;

	// reading the first line
	safeGetline(inputFile, line);

	bool read_first_token = false;
	while (line.find(";", 0) != string::npos)
	{
		size_t pos = line.find(";", 0); //store the position of the delimiter
		string token = line.substr(0, pos);      //get the token
		line.erase(0, pos + 1);          //erase it from the source 
		if (read_first_token)
		{
			column_names.push_back(token);                //and put it into the array	
		}
		else
		{
			read_first_token = true;
		}
	}
	column_names.push_back(line);           //the last token is all alone 
	// finished reading column names
	n_columns = column_names.size();

	int line_index = 0;
	while (not safeGetline(inputFile, line).eof())
	{
		// getting the row name
		size_t pos = line.find(";", 0); //store the position of the delimiter
		string token = line.substr(0, pos);      //get the token
		line.erase(0, pos + 1);          //erase it from the source
		row_names.push_back(token);

		vector<bool> line_vector;
		if (get_vector_from_string(line, line_vector))
		{
			if (n_columns != line_vector.size())
			{
				cout << "ERROR: Row " << token << " has a different number of entries (" << line_vector.size() << ") as the previous rows (" << n_columns << ")" << endl;
				return false;
			}
			cell.push_back(line_vector);
		}
		else
		{
			return false;
		}

		line_index++;
	}

	n_rows = cell.size();
	if (n_rows == 0)
	{
		cout << "ERROR: Empty input matrix" << endl;
		return false;
	}

	cout << "*** Input matrix " << n_rows << "x" << n_columns << endl;

	inputFile.close();

	return true;
}

bool binary_matrix::print_to_file(string outputFileName) const
{
	cout << "*** Printing the solution matrix to file " << outputFileName << endl;

	ofstream outputFile;
	outputFile.open(outputFileName);

	// printing column names
	for (int j = 0; j < n_columns; j++)
	{
		outputFile << ";" << column_names[j];
	}
	outputFile << endl;

	for (int i = 0; i < n_rows; i++)
	{
		for (int j = 0; j < n_columns; j++)
		{	
			if (j == 0)
			{
				outputFile << row_names[i] << ";";
			}

			outputFile << cell[i][j];
			if (j < n_columns - 1)
			{
				outputFile << ";";
			}
			else
			{
				outputFile << endl;
			}
		}
	}

	outputFile.close();
	return true;
}

bool binary_matrix::is_included_in(uint col_i, uint col_j) const
{
	assert(col_i < n_columns);
	assert(col_j < n_columns);

	for (int k = 0; k < n_rows; k++)
	{
		if (cell[k][col_i] > cell[k][col_j])
		{
			return false;
		}
	}
	return true;
}

bool binary_matrix::are_equal(uint col_i, uint col_j) const
{
	assert(col_i < n_columns);
	assert(col_j < n_columns);

	for (int k = 0; k < n_rows; k++)
	{
		if (cell[k][col_i] != cell[k][col_j])
		{
			return false;
		}
	}
	return true;
}

bool binary_matrix::are_conflicting(uint col_i, uint col_j) const
{
	assert(col_i < n_columns);
	assert(col_j < n_columns);

	bool intersection_non_empty = false;
	bool i_not_included_in_j = false;
	bool j_not_included_in_i = false;

	for (int k = 0; k < n_rows; k++)
	{
		if (cell[k][col_i] + cell[k][col_j] == 2)
		{
			intersection_non_empty = true;
		}
		if (cell[k][col_i] < cell[k][col_j])
		{
			j_not_included_in_i = true;
		}
		if (cell[k][col_i] > cell[k][col_j])
		{
			i_not_included_in_j = true;
		}
		if (intersection_non_empty and i_not_included_in_j and j_not_included_in_i)
		{
			return true;
		}
	}
	return false;
}

bool binary_matrix::is_conflict_free() const
{
	for (int j = 0; j < n_columns; j++)
	{
		for (int k = j + 1; k < n_columns; k++)
		{
			if (are_conflicting(j, k))
			{
				return false;
			}
		}
	}
	return true;
}

void binary_matrix::hide_duplicate_columns(bool verbose)
{
	vector<bool> is_duplicate(n_columns, false);
	vector< vector<string> > multiplicity(n_columns); // names of columns that are identical to the current one

	for (int j = 0; j < n_columns; j++)
	{
		if (is_duplicate[j])
		{
			continue;
		}

		for (int k = j + 1; k < n_columns; k++)
		{
			if (are_equal(j, k))
			{
				multiplicity[j].push_back(column_names[k]);
				is_duplicate[k] = true;
			}
		}
	}
	// cout << "*** Found the duplicate columns" << endl;

	// computing the new matrix contents and column names
	vector<string> column_names2;
	vector< vector<bool> > cell2;
	for (int i = 0; i < n_rows; i++)
	{
		vector<bool> next_row;
		for (int j = 0; j < n_columns; j++)
		{
			if (not is_duplicate[j])
			{
				next_row.push_back(cell[i][j]);

				// for the first row, fill in the column_multiplicities
				if (i == 0)
				{	
					column_multiplicities.push_back(multiplicity[j]);
					column_names2.push_back(column_names[j]);
				}
			}
		}
		cell2.push_back(next_row);
	}
	assert(column_multiplicities.size() == cell2[0].size());

	// cout << "*** Computed the unique columns" << endl;

	cell.clear();
	column_names.clear();
	cell = cell2;
	column_names = column_names2;
	n_columns = cell[0].size();

	if (verbose)
	{
		cout << "INFO: " << n_columns << " columns are pair-wise distinct" << endl;
	}
}

void binary_matrix::show_duplicate_columns()
{
	assert(column_multiplicities.size() > 0);
	assert(column_multiplicities.size() == cell[0].size());

	vector< vector<bool> > cell2;
	vector<string> column_names2;

	for (int i = 0; i < n_rows; i++)
	{
		vector<bool> next_row;
		for (int j = 0; j < n_columns; j++)
		{
			for (int k = 0; k < 1 + column_multiplicities[j].size(); k++)
			{
				next_row.push_back(cell[i][j]);
			}
			if (i == 0)
			{	
				column_names2.push_back(column_names[j]);
				for (int k = 0; k < column_multiplicities[j].size(); k++)
				{
					column_names2.push_back(column_multiplicities[j][k]);
				}
			}
		}
		cell2.push_back(next_row);
	}
	
	cell.clear();
	column_names.clear();
	cell = cell2;
	column_names = column_names2;
	n_columns = cell[0].size();
}

void binary_matrix::remove_columns_with_low_support(uint support, bool verbose)
{
	cout << "*** Removing columns that appear strictly less than " << support << " times" << endl;
	vector< uint > multiplicity(n_columns, 0); // names of columns that are identical to the current one

	for (int j = 0; j < n_columns; j++)
	{
		for (int k = 0; k < n_columns; k++)
		{
			if (are_equal(j, k))
			{
				multiplicity[j]++;
			}
		}
	}
	// cout << "*** Found the duplicate columns" << endl;

	// computing the new matrix contents and column names
	vector<string> column_names2;
	vector< vector<bool> > cell2;
	for (int i = 0; i < n_rows; i++)
	{
		vector<bool> next_row;
		for (int j = 0; j < n_columns; j++)
		{
			if (multiplicity[j] >= support)
			{
				next_row.push_back(cell[i][j]);

				if (i == 0)
				{	
					column_names2.push_back(column_names[j]);
				}
			}

		}
		cell2.push_back(next_row);
	}

	// cout << "*** Computed the unique columns" << endl;

	cell.clear();
	column_names.clear();
	cell = cell2;
	column_names = column_names2;
	n_columns = cell[0].size();

	if (verbose)
	{
		cout << "INFO: " << n_columns << " columns have support at least " << support << endl;
	}
}


bool get_vector_from_string(string s, vector<bool>& result)
{
	stringstream ss(s);
	int i;

	while (ss >> i)
    {
    	if (i == 0 or i == 1)
    	{
    		result.push_back((bool)i);
    		if (ss.peek() == ';')
    		{
            	ss.ignore();
    		}
    	}
    	else
    	{
    		cout << "ERROR: The input matrix has some entries different from 0 and 1" << endl;
    		return false;
    	}
    } 

	return true;
}

void create_random_matrix(binary_matrix& matrix, uint m, uint n)
{
	matrix.n_rows = m;
	matrix.n_columns = n;
	matrix.row_names = vector<string>(matrix.n_rows, "row");
	matrix.column_names = vector<string>(matrix.n_columns, "column");

	srand(time(NULL));

	for (int i = 0; i < matrix.n_rows; i++)
	{
		vector<bool> new_row;
		for (int j = 0; j < matrix.n_columns; j++)
		{
			int b = rand() % 2;
			new_row.push_back(b);
		}
		matrix.cell.push_back(new_row);
	}
}
