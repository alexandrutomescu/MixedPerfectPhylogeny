#include "utils.h"
#include "heuristic.h"
#include "poly_alg.h"
#include "draw_tree.h"

using namespace std;
#define N_TESTS 10
#define N_ROWS_TEST 50
#define N_COLS_TEST 1000

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
		"The MIT License (MIT)."
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
	parser.add_option("-t", "--runtests") .action("store_true") .dest("run_tests") .set_default(false) .help("Tests the algorithms on " + to_string(N_TESTS) + " random matrices of size " + to_string(N_ROWS_TEST) + "x" + to_string(N_COLS_TEST));

	optparse::Values& options = parser.parse_args(argc, argv);

	inputFileName = (string) options.get("i");
	outputFileName = (string) options.get("o");

	should_run_heuristic_algorithm = (options.get("heuristic_algorithm") ? true : false);
	should_run_tests = (options.get("run_tests") ? true : false);
	verbose = (options.get("verbose") ? true : false);

	if (should_run_tests)
	{
		int n_special_case = 0;
		int n_heuristic = 0;

		double total_time_poly_alg = 0;
		double total_time_heuristic_alg = 0;

		while ((n_heuristic < N_TESTS)) // or (n_special_case < N_TESTS))
		{
			binary_matrix inputMatrix;
			binary_matrix outputMatrix;

			create_random_matrix(inputMatrix, N_ROWS_TEST, N_COLS_TEST);
			inputMatrix.hide_duplicate_columns(verbose);
			if (not inputMatrix.is_conflict_free())
			{
				if (n_heuristic < N_TESTS)
				{
					n_heuristic++;
					clock_t begin = clock();
					heuristic_algorithm(inputMatrix, outputMatrix, verbose);
					clock_t end = clock();
					total_time_heuristic_alg += double(end - begin);
				}
				
				// if (n_special_case < N_TESTS)
				// {
				// 	if (check_special_case(inputMatrix, verbose))
				// 	{
				// 		n_special_case++;
				// 		cout << "INFO: Found " << n_special_case << "/" << N_TESTS << " matrices belonging to the polynomially solvable case" << endl;
				// 		clock_t begin = clock();
				// 		polynomial_algorithm(inputMatrix, outputMatrix, verbose);
				// 		clock_t end = clock();
				// 		total_time_poly_alg += double(end - begin);
				// 	}
				// }
			}
		}
		cout << "INFO: Tested the algorithms on " << N_TESTS << " random matrices satisfying the input assumptions of the algorithms" << endl;
		// cout << "INFO: The polynomial algorithm runs on average in " << (double)total_time_poly_alg / (N_TESTS * CLOCKS_PER_SEC) << " sec" << endl;
		cout << "INFO: The heuristic algorithm runs on average in " << (double)total_time_heuristic_alg / (N_TESTS * CLOCKS_PER_SEC) << " sec" << endl;

		return EXIT_SUCCESS;
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
	inputMatrix.hide_duplicate_columns(verbose);

	// inputMatrix.print_to_file(inputFileName + ".uniquecols.csv");

	if (not inputMatrix.is_conflict_free())
	{
		if (should_run_heuristic_algorithm)
		{
			heuristic_algorithm(inputMatrix, outputMatrix, verbose);
			int lower_bound = compute_lower_bound(inputMatrix, verbose);
			cout << "INFO: " << lower_bound << " is a lower bound on the optimum number of total output rows" << endl;
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
	}
	else
	{
		cout << "INFO: Input matrix " << inputFileName << " is already conflict-free. Writing it in output" << endl;
		outputMatrix = inputMatrix;
	}


	draw_tree_in_dot_file(outputMatrix, outputFileName + ".dot");

	// outputMatrix.print_to_file(outputFileName + ".uniquecols.csv");

	// showing duplicate columns
	outputMatrix.show_duplicate_columns();

	outputMatrix.print_to_file(outputFileName);

	return EXIT_SUCCESS;
}