# Mixed Perfect Phylogeny v0.1
**For questions or problems with this code, contact [tomescu@cs.helsinki.fi](mailto:tomescu@cs.helsinki.fi), or open an [issue](https://github.com/alexandrutomescu/MixedPhylogeny/issues) on github.**

This repository contains implementations of the two algorithms from: 

*Ademir Hujdurović, Urša Kačar, Martin Milanič, Bernard Ries, and Alexandru I. Tomescu, Complexity and algorithms for finding a perfect phylogeny from mixed tumor samples, 2016, submitted.*

A shorter version of this paper appeared in the Proceedings of WABI 2015:

*Ademir Hujdurović, Urša Kačar, Martin Milanič, Bernard Ries, and Alexandru I. Tomescu, Finding a Perfect Phylogeny from Mixed Tumor Samples. WABI 2015, LNCS 9289, pp. 80-92, extended version available at [http://arxiv.org/abs/1506.07675](http://arxiv.org/abs/1506.07675).*

Both algorithms address the **MINIMUM CONFLICT-FREE ROW SPLIT problem**, originally proposed by *I. Hajirasouliha, B. Raphael, Reconstructing Mutational History in Multiply Sampled Tumors Using Perfect Phylogeny Mixtures. WABI 2014: 354-367 doi:[10.1007/978-3-662-44753-6_27](http://dx.doi.org/10.1007/978-3-662-44753-6_27)*.  A visual description of this problem is available in these [**slides**](https://www.cs.helsinki.fi/u/tomescu/perfect-phylogeny-tumors.pdf). Our first algorithm is exact, runs in polynomial time, but only solves a particular class of inputs. The second algorithm is heuristic, i.e. runs on all inputs in polynomial time, but may not produce an optimal solution. 

## 1. Input format

The input matrix must be in .csv format, with ';' as value separator. Such files can be written/edited with e.g. Excel. The first column must contain the row (i.e., sample) names, and the first row must contain the column (i.e., mutation location) names. For example, the following table 

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
The output matrix has the same .csv format. If a row labeled **r** is split into *k* rows in the output matrix, the labels of the resulting rows will be **r_1, r_2, ..., r_k**. For example:

	;c1;c2;c3;c3';c4;c5
	r1;1;0;0;0;0;0
	r2_1;1;0;0;0;0;0
	r2_2;0;1;1;1;0;0
	r2_3;0;1;0;0;1;0
	r3;0;1;0;0;1;0
	r4;0;1;1;1;0;1
	r5;0;1;0;0;0;0

The program also outputs the perfect phylogeny tree of this output matrix, in .dot format. This can be visualized by e.g., [Graphviz](http://www.graphviz.org). For example, the tree of this output matrix is

![Output tree](https://github.com/alexandrutomescu/MixedPhylogeny/blob/master/example/matrix_alg1.out.csv.png)

# 3. Installation

If you use Linux or Mac OS, type in the root directory:

	make

This creates the executable:

	mixedphylogeny
	
Our code requires the free [Lemon library](http://lemon.cs.elte.hu/trac/lemon). (We provided a pre-compiled version for Linux/Mac OS in the directory **lemon_binaries_linux**.) If you use a different operating system, install Lemon as described in its manual, modify our **makefile** to point to your installation of Lemon, and run **make** again.
	
# 4. Running

If you want to run the first algorithm (the exact one for the polynomially solvable instances), run e.g.:

	./mixedphylogeny -i example/matrix_alg1.csv -o example/matrix_alg1.out.csv
	
If your input matrix does not belong to the polynomially-solvable case, then the program will tell you so. In that case, you can run our second, heuristic, algorithm, by adding the parameter **--heuristic**, e.g.:

	./mixedphylogeny -i example/matrix_alg2.csv -o example/matrix_alg2.out.csv --heuristic

# 5. Experimental results

We tested the heuristic algorithm on the ten binary matrices from:

*M. Gerlinger et al., Genomic architecture and evolution of clear cell renal cell carcinomas defined by multiregion sequencing, Nature Genetics, 46(3), 2014, 225-232, doi:[10.1038/ng.2891](http://dx.doi.org/10.1038/ng.2891)*. Our conflict-free row split matrices and the corresponding phylogenetic trees are available [**here**](http://cs.helsinki.fi/u/tomescu/MixedPhylogeny/results-Gerlinger-et-al-2014.zip).
