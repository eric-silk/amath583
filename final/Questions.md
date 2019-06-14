---
title: Final Questions
---

### Final Written Questions

#### Norm

**Q**
Per our discussions in lectures past about weak vs strong scaling, do the plots look like what you would expect?  Describe any (significant) differences (if any).

Strong scaling shows that the speedup increases to a certain point, and then fails to improve the situation. Weak scaling shows that the execution times are (roughly) constant if you scale a problem AND number of nodes. The exception is the artifical jump in the graph caused by manual insertion of the times "42" (per Dr. Lumsdaine's instructions), as the script failed to finish these runs.

**Q**
For strong scaling, at what problem size (and what number of nodes) does parallelization stop being useful?

It didn't seem to, at least on our problem sizes. The graphs show a linear relationship the whole way up.

#### Solving Laplace's equation

**Q**
Per our discussions in lectures past about weak vs strong scaling, do the plots look like what you would expect?  Describe any (significant) differences (if any).

Yes, the strong scaling increases performance to a point, and then begins to perform worse. This maxima appears to be correlated to the problem size -- bigger problems hit their peak at a higher number of processes.


**Q**
For strong scaling, at what problem size (and what number of nodes) does parallelization stop being useful?

1024 - 8 nodes
2048 - 16 nodes
4096 - possibly 32, would have to run at 64 as well to verify 32 is a maxima and not just a "kink" in the plot.

#### About the Final
**Q**
The most important thing I learned from this assignment was ...

Make damn sure you understand how to access your data structures. I had my MPI stuff all correct for update_halo() days ago, but was accessing the grid incorrectly. Derp. Asserts are super helpful, if you didn't know.

Additionally, MPI is wicked cool. Dr. Lumsdaine was right -- the first time you kick off a big job and see all those nodes chewing on the problem in parallel is immensely satisfying.

**Q**
One thing I am still not clear on is ...

I'm not sure why Dr. Lumsdaine elected to name members of the grid class the way he did, but to each their own. Certainly caused confusion for me.


#### About the Course
**Q**
The most important thing I learned from this course was ...

HPC frameworks and techniques are shockingly accesible. It seems the harder part is the IT side of things -- drivers, cluster management, etc. The actual programming was often easier than I recall my first CS class being.
**Q**
One thing I am still not clear on is ...

What it would take to get a cluster or CUDA dev environment actually up and running from scratch. Obviously outside the scope of the class, but still something I've not yet learned. Just getting GPU backed Tensorflow working at my job was enough of a pain, I imagine this is an entirely worse circle of hell :P
