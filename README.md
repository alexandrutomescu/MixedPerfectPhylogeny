# MixedPhylogeny
This repository contains implementations of the two algorithms from: 

*Ademir Hujdurović, Urša Kačar, Martin Milanič, Bernard Ries, and Alexandru I. Tomescu, Complexity and algorithms for finding a perfect phylogeny from mixed tumor samples, 2016, submitted.*

A shorter version of this paper appeared in the proceedings of WABI 2015:

*Ademir Hujdurović, Urša Kačar, Martin Milanič, Bernard Ries, and Alexandru I. Tomescu, Finding a Perfect Phylogeny from Mixed Tumor Samples. WABI 2015, LNCS 9289, pp. 80-92, extended version available at [http://arxiv.org/abs/1506.07675](http://arxiv.org/abs/1506.07675).*

Both algorithms are given a binary matrix, and output a minimum conflict-free row-split of it. Some slides about this problem are available [**here**](https://www.cs.helsinki.fi/u/tomescu/perfect-phylogeny-tumors.pdf).

## 1. Input format

The input matrix must be in .csv format, with ';' as value separator. Such files can be edited with e.g. Excel. The first column must contain the row (i.e., sample) names, and the first row must contain the column (i.e., mutation location) names. For example, the following table 

|   | c1| c2| c3| c4| c5| c3'|
|---|---|---|---|---|---|----|
| r1|  1|  0|  0|  0|  0|   0|
| r2|  1|  1|  1|  1|  0|   1|
| r3|  0|  1|  0|  1|  0|   0|
| r4|  0|  1|  1|  0|  1|   1|
| r5|  0|  1|  0|  0|  0|   0|

is encoded as:

	;c1;c2;c3;c4;c5;c3'
	r1;1;0;0;0;0;0
	r2;1;1;1;1;0;1
	r3;0;1;0;1;0;0
	r4;0;1;1;0;1;1
	r5;0;1;0;0;0;0
	
## 2. Output format	
The output matrix has the same format. If a row labeled **r** is split into *k* rows in the output matrix, the labels of the resulting rows will be **r_1, r_2, ..., r_k**. For example:

	;c1;c2;c3;c3';c4;c5
	r1;1;0;0;0;0;0
	r2_1;1;0;0;0;0;0
	r2_2;0;1;1;1;0;0
	r2_3;0;1;0;0;1;0
	r3;0;1;0;0;1;0
	r4;0;1;1;1;0;1
	r5;0;1;0;0;0;0

The program also outputs the perfect phylogeny tree of this output matrix, in dot format. This can be visualized by e.g., [Graphviz](http://www.graphviz.org). For example, the tree of this output matrix is

![Output tree](https://github.com/alexandrutomescu/MixedPhylogeny/blob/master/example/matrix_alg1.out.csv.png)

# 3. Installation

If you use Linux, just type:

	make

This creates the executable

	mixedphylogeny
	
Our implementation needs the free [Lemon library](http://lemon.cs.elte.hu/trac/lemon). We provide a pre-compiled version for Linux in the directory **lemon_binaries_linux**. If you use a different system, or this doesn't work for you, install Lemon as described in its manual, and modify our **makefile** to point to your installation of Lemon.
	
# 4. Running

If you want to run the first algorithm (the exact one for the polynomially solvable instances), run e.g.:

	./mixedphylogeny -i example/matrix_alg1.csv -o example/matrix_alg1.out.csv
	
If your input matrix does not belong to the polynomially-solvable case, then the implementation will tell you so. In that case, you can run our second, heuristic, algorithm, by adding the parameter **--heuristic**, e.g.:

	./mixedphylogeny -i example/matrix_alg2.csv -o example/matrix_alg2.out.csv --heuristic
