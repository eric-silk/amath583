---
title: PS4 Questions
---

### PS4 Written Questions

Add your answers to this file in plain text after each question.  Leave a blank line between the
text of the question and the text of your answer.

#### Ordering matters


**Q**
There should be some differences between the results for the three functions for multiplying
matrices (ijk, ikj, jki).  Using what you know about memory hierarchy, CPU operation, and data
layout for row-major matrices, explain the behavior you are seeing.  (Hint: The three functions
differ in the variable they iterate over in their inner loops.)

In the ijk implementation, the indexed values in B and C is contained in a new row every innermost loop.
This means the program must fetch a new row from memory as a result of a cache miss.
Conversely, the ikj implementation, the values loaded in the kth rows of B and C are iterated through in
the innermost loop. This lends itself to pipelining, as the CPU can rely on cached data and use
the pipeline to pre-fetch the next row in B and C.

In the jki implementation, the program must fetch a new row every loop for both A and C every inner loop,
and a new row for B every second innermost loop.


### Orientation

**Q**
For each of the ijk, ikj, and jki orderings for a row-oriented matrix-matrix product, what are the
equivalent orderings for a column-oriented matrix-matrix product?

ijk: ikj
ikj: jki
jki: ijk


#### Does ordering matter?

**Q** Again, using what you
know about memory hierarchy, SIMD/vectorization, CPU operation, and data
layout for row-major matrices, explain the behavior you
are seeing with `matvec_ij` and `matvec_ji`.

We see significantly better performance for the `matvec_ij` implementation. This implementation
loads a row from the matrix into and then iterates through the column elements; the `matvec_ji`
implementation loads a row into cache, accesses a single value, then must load another row.
This produces cache misses and slows performance.


#### Vector of Vectors

**Q**
Explain any differences between what you see for `VoVMatrix` and what you see for `Matrix`

`VoVMatrix` appears to run somewhat slower on average for a given ordering. I would speculate
that this is due to the overhead introduced by a higher level of abstraction in the vector
of vectors rather than  the "one layer" of abstraction in the the Matrix implementation,
wherein we're manually indexing into a single vector.



**Q**
Does `VoVMatrix` respond in the same way for different loop orderings as does `Matrix`?

It does appear to respond in a similar fashion, albeit with slower performance per loop ordering.


#### About PS4


**Q** The most important thing I learned from this assignment was ...

Loop reordering never even occured to me (which is why I lean so heavily on already optimized
libraries at work...). I thought the compiler would just optimize for that, but the
Wikipedia article on Loop Interchange seems to suggest that this is **not** typically done because
it can degrade performance.

**Q** One thing I am still not clear on is ...

Seeing as language such as Fortran opted for column major, rather than row major, representation of
Matrices, how would a language designer make that choice? Fortran seems exceptional in its choice,
was this due to it being a "mathematician's" language (similar to its choice to index starting at 1)
or was it arbitrary?
