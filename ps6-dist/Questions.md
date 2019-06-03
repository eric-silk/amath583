---
title: PS6 Questions
---

### PS6 Written Questions

#### Norm

**Q**
Which combination of problem size, blocking, and synchronization shows the best performance?  Which shows the worst?  Explain, using what you know about threads, locking, OpenMP, computer architecture, etc.

Critical sections perform the worst, full stop. Atomic (which I added) seems to perform a bit better.
These both make sense as each will have to halt the parallel implementation for a period of time to run the critical section;
I think atomic is more intelligent about this (maybe?) and gets better results.

The parallel for seems to work the best, the cyclic doesn't perform well at all. I suspect this is due to
the scheduling pragma I used not working as expected. Out of time, but re-structuring the loops
to iterate through increasing the iterator by the cycle size (e.g. i+=2 instead of i++) would be the "correct"
way to do this.

Block reduction has some...weirdness going on. I suspect this is due to limited AWS resources.

#### Sparse Matrix-Vector Product
**Q**
Which matrix formats (if any) need to be protected against race conditions?
What approach did you use to ensure safety?  Did that approach affect performance?

I was unable to find it.

**Q**
(Extra Credit)
If there is a potential for a race condition in any of the sparse matrix formats, can you create some combination of number of threads, problem size, matrix data, etc to expose it?

**Q**
Which combination of matrix format, problem size, blocking, and synchronization shows the best performance?  Which shows the worst?  Explain, using what you know about threads, locking, OpenMP, computer architecture, etc.

#### Partitioning and Load Balancing
**Q**
Which scheduling approach (and which block size) give the best load balancing performance?

I can't confidently answer this -- AWS was beyond sluggish when I ran things so my results were highly irregular.

#### PageRank
**Q**
What do you need to change to effect parallelization of the matrix-vector product portion of the pagerank computation?  What parallelization approach did you choose?

I changed the overload of the sparse matrix vector multiplication operator to call
the OMP version that was already written. Manually calling this (e.g. P.omp_matvec(...))
seemed to cause the solution to diverge? I even verified I was hitting the same code
with the debugger. At any rate, I opted for that.

**Q**
(Extra Credit)
Use a parallelization approach of your choosing and report on the benefits obtained.

I additionally parallelized the remaining operations with the #pragma omp parallel for
directive, and parallelized the two norm calculation doing the same (but with the reduction()
portion as well). Both seemed to offer some speedup but neither was as significant as the
matrix vector parallelization.

**Q**
(Extra Credit)
Are there any operations in the program where Amdahl's Law would limit scalability?

Literally any situation in which the problem can be parallelized efficiently would
run into scaling problems. For instance, the two norm function, even for a vector
of size N (N >> 1) would run into a performance maximum when the number of cores/nodes/
processors, etc., meets or exceeds N. This is of course assuming an otherwise
"perfect" machine (infinite L1 cache, vanishingly small locality, etc.).

#### About PS6
**Q**
The most important thing I learned from this assignment was ...

Even with the hugely helpful nature of OpenMP, there is still a lot of work in
parallelization of programs. The nice thing about OpenMP is it removes a lot of the
kludge associated with the algorithms and allows the author to focus on the algorithm
and separating out regions that can be made parallel and marking them as such, rather than
hand-rolling the parallel implementation.

**Q**
One thing I am still not clear on is ...

I'm still fuzzy on some of the scheduling concepts (auto vs. guided vs static vs...)
All the examples don't seem to indicate true parallel performance but rather a method
for sharing a single core? When do I opt for atomic vs. critical?

Additionally, I think I just need more time to develop intuition as to breaking up
existing algorithms with various OpenMP pragmas.
