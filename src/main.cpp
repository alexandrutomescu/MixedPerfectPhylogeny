#include "utils.h"
#include "heuristic.h"
#include "poly_alg.h"
#include "draw_tree.h"

using namespace std;
#define N_TESTS 5

int main(int argc, char **argv)
{
	string inputFileName, outputFileName;
	binary_matrix inputMatrix, outputMatrix;
	bool should_run_heuristic_algorithm, should_run_tests, verbose;

	// command line argument parser
	string usage = "\n  %prog OPTIONS"
		"\n\nBrief example:"
		"\n  %prog -i <input .csv file> -o <output .csv file> [--heuristic]";
	const string version = "%prog 0.1\nCopyright (C) 2016 Alexandru Tomescu\n"
		"License GPLv3+: GNU GPL version 3 or later "
		"<http://gnu.org/licenses/gpl.html>.\n"
		"This is free software: you are free to change and redistribute it.\n"
		"There is NO WARRANTY, to the extent permitted by law.";
	const string desc = "";
	const string epilog = "";
	
	optparse::OptionParser parser = optparse::OptionParser()
    	.usage(usage)
    	.version(version)
    	.description(desc)
    	.epilog(epilog);

	parser.add_option("-i", "--input") .type("string") .dest("i") .set_default("") .help("Input file (.csv format, values separated by ';'). The first column should contain the row names, and the first row should contain the column names.");
	parser.add_option("-o", "--output") .type("string") .dest("o") .set_default("") .help("Output file (.csv format). Another .dot file is outputted with the tree of the perfect phylogeny.");
	parser.add_option("-e", "--heuristic") .action("store_true") .dest("heuristic_algorithm") .set_default(false) .help("Run the heuristic algorithm");
	parser.add_option("-v", "--verbose") .action("store_true") .dest("verbose") .set_default(false) .help("Verbose command line output");
	parser.add_option("-t", "--runtests") .action("store_true") .dest("run_tests") .set_default(false) .help("Tests the algorithms on random matrices. For each M in [4..10], N in [4..1000], it generates " + to_string(N_TESTS) + " random binary MxN matrices and checks that both algorithms return conflict-free matrices.");

	optparse::Values& options = parser.parse_args(argc, argv);

	inputFileName = (string) options.get("i");
	outputFileName = (string) options.get("o");

	should_run_heuristic_algorithm = (options.get("heuristic_algorithm") ? true : false);
	should_run_tests = (options.get("run_tests") ? true : false);
	verbose = (options.get("verbose") ? true : false);

	if (should_run_tests)
	{
		int n_special_case = 0;
		int n_total_matrices = 0;
		for (int k = 0; k < N_TESTS; k++)
		{
			for (int i = 4; i <= 10; i++)
			{
				for (int j = 4; j <= 1000; j++)
				{
					binary_matrix inputMatrix;
					binary_matrix outputMatrix;
					n_total_matrices++;

					create_random_matrix(inputMatrix, i, j);
					inputMatrix.hide_duplicate_columns();

					if (not inputMatrix.is_conflict_free())
					{
						heuristic_algorithm(inputMatrix, outputMatrix, verbose);
						if (check_special_case(inputMatrix, verbose))
						{
							n_special_case++;
							polynomial_algorithm(inputMatrix, outputMatrix, verbose);
						}
					}
				}
				cout << "*** Matrices with " << i << " rows: OK." << " Matrices belonging to the special case: " << n_special_case << "/" << n_total_matrices << endl;
			}
		}
		cout << "*** All tests: OK" << endl;
	}

	if (inputFileName == "")
	{
		cerr << "ERROR: Parameter -i | --input is required" << endl;
		cerr << "ERROR: Use --help to see OPTIONS" << endl;
		return EXIT_FAILURE;
	}
	if (outputFileName == "")
	{
		cerr << "ERROR: Parameter -o | --output is required" << endl;
		cerr << "ERROR: Use --help to see OPTIONS" << endl;
		return EXIT_FAILURE;
	}


	if (not inputMatrix.read_from_file(inputFileName))
	{
		cout << "ERROR: There was an error" << endl;
		return EXIT_FAILURE;
	}

	// hiding duplicate columns
	inputMatrix.hide_duplicate_columns();

	if (should_run_heuristic_algorithm)
	{
		heuristic_algorithm(inputMatrix, outputMatrix);
	}
	else
	{
		if (check_special_case(inputMatrix))
		{
			polynomial_algorithm(inputMatrix, outputMatrix);
		}
		else
		{
			cout << "ERROR: The input matrix does not belong to the special class of polynomial solvable matrices described in the paper." << endl;
			cout << "ERROR: Try running the heuristic algorithm instead. Add parameter --heuristic when running the tool" << endl;
			return EXIT_FAILURE;
		}
	}
	draw_tree_in_dot_file(outputMatrix, outputFileName + ".dot");

	// showing duplicate columns
	outputMatrix.show_duplicate_columns();

	outputMatrix.print_to_file(outputFileName);

	return EXIT_SUCCESS;
}