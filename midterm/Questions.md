---
title: Midterm Questions
---

### Midterm Written Questions

Add your answers to this file in plain text after each question.  Leave a blank line between the text of the question and the text of your answer.


#### Sparse-matrix dense-matrix product


**Q**
How
does the performance (in GFLOP/s) for sparse-matrix by dense matrix product (**SPMM**) compare 
to sparse-matrix by vector product (**SPMV**)?
The performance for SPMM should be about the same as for SPMV in the case of a 1 column dense matrix.  
What is the trend with increasing numbers of columns?  

There is a slight difference, owing to my "lazy" implementation. However, this could be optimized
to not construct a vector each time and instead access the elements directly. As number of rows
increases, thre appears to be a general trend in decreasing performance, with a few exceptions.
There is also a strange dip in the performance, typically around N=16384. I don't know why this is.



**Q**
How
does the performance of SPMM (in GFLOP/s) compare to the results you got dense matrix-matrix product in previous assignments?   There
should be a fairly large difference in GFLOP/s between sparse and dense matrix
methods.  Give some reasons for such a big difference.

The sparse matrices are far faster. This is primarily due to the fact that the dense matrices require
high numbers of "useless" multiplications by zero and the memory requirements are higher (and thus the
impact on cache use is increased).



#### Compressed sparse column

**Q** 
How
does the performance of SPMM (in GFLOP/s) compare to the results you got dense matrix-matrix product in previous assignments?   There
should be a fairly large difference in GFLOP/s between sparse and dense matrix
methods.  Give some reasons for such a big difference.

For the same reasons as listed under the above section, CSC appears to perform much faster than a
regular dense matrix.


**Q**
How does the performance of CSC compare to the performance of CSR and of COO?  
Explain why (or why not) there are any significant differences.

The access pattern for an arbitrary sparse matrix during matrix vector or matrix matrix multiplication
favors CSR representation slightly, likely due to similar reasons as why changing loop ordering
in a dense matrix multiplication increases performance.

The big caveat here is that my CSC implementation isn't working quite right as far as I can tell,
so there may be a different result than a "true" CSC implementation.


#### Array of structs

**Q**
How does the performance of AOS compare to the performance of  COO?  
Explain why (or why not) there are any significant differences.

There is no significant difference as far as I can tell. The representations are "basically" the
same, and caching/accessing a struct of arrays vs. an array of structs has roughly the same
performance requirements.


#### Errata

It is worth pointing out that there is currently significant portions of repeated code, varying really only in their argument
type (COO vs. CSR, etc). Because of this, we could "easily" convert these into a template function, such that only the matvec
function for each needs to differ.

But its already Wednesday evening and had my fill of this for now.
