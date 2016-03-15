
#ifndef HEURISTIC_H_INCLUDED
#define HEURISTIC_H_INCLUDED

#include "utils.h"
#include "flow.h"

void heuristic_algorithm(const binary_matrix& m, binary_matrix& result, bool verbose = false);

int compute_lower_bound(const binary_matrix& m, bool verbose);

#endif // HEURISTIC_H_INCLUDED