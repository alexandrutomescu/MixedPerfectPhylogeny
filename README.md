# MixedPhylogeny
This repository contains implementations of the two algorithms from: 

*Ademir Hujdurović, Urša Kačar, Martin Milanič, Bernard Ries, and Alexandru I. Tomescu, Complexity and algorithms for finding a perfect phylogeny from mixed tumor samples, 2016, submitted*

A shorter version of this paper appeared in the proceedings of WABI 2015:

*Ademir Hujdurović, Urša Kačar, Martin Milanič, Bernard Ries, and Alexandru I. Tomescu, Finding a Perfect Phylogeny from Mixed Tumor Samples. WABI 2015, LNCS 9289, pp. 80-92, whose version is available at [http://arxiv.org/abs/1506.07675](http://arxiv.org/abs/1506.07675)*

Both algorithms are given a MxN binary matrix, and output a minimum conflict-free row-split of it. Some slides about this problem are available [**here**](https://www.cs.helsinki.fi/u/tomescu/perfect-phylogeny-tumors.pdf).

## 1. Input format

The input matrix must be in .csv format, with ';' as value separator. Such files can be edited with e.g. Excel. The first column must contain the row (i.e., sample) names, and the first row must contain the column (i.e., mutation location) names. For example, the following table 

  |c1|c2|c3|c4|c5|c3'
--|--|--|--|--|--|--
r1| 1| 0| 0| 0| 0| 0
r2| 1| 1| 1| 1| 0| 1
r3| 0| 1| 0| 1| 0| 0
r4| 0| 1| 1| 0| 1| 1
r5| 0| 1| 0| 0| 0| 0

is encoded as:

	;c1;c2;c3;c4;c5;c3'
	r1;1;0;0;0;0;0
	r2;1;1;1;1;0;1
	r3;0;1;0;1;0;0
	r4;0;1;1;0;1;1
	r5;0;1;0;0;0;0