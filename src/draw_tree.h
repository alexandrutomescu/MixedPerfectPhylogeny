
#ifndef DRAW_TREE_H_INCLUDED
#define DRAW_TREE_H_INCLUDED

#include <string>
#include <fstream>
#include <unordered_set>
#include <assert.h>
#include <map>

#include "utils.h"

using namespace std;

void draw_tree_in_dot_file(const binary_matrix& m, string outputFileName);

#endif // DRAW_TREE_H_INCLUDED
