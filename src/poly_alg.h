
#ifndef POLY_ALG_H_INCLUDED
#define POLY_ALG_H_INCLUDED

#include "utils.h"

bool check_special_case(const binary_matrix& m, bool verbose = false);

void polynomial_algorithm(const binary_matrix& m, binary_matrix& result, bool verbose = false);

#endif // POLY_ALG_H_INCLUDED