---
title: PS7 Questions
---

### PS7 Written Questions

#### Warmup

**Q**
How many more threads are run in version 2 compared to version 1?  How much speedup might you expect?  How much speedup do you see in your plot?

256 threads compared to just 1. "Ideally" a 256x speedup, in reality about 25-30x increase in performance.

#### Norm

**Q**
Use nvprof (you may use the nvprof.bash from the hello subdirectory) to analyze the three cuda/thrust programs.  Is it obvious from the output and/or the program timer output which kernel needs the most tuning, and what are the opportunities for tuning?  Describe your observations about how well cuda_norm, thrust_norm, and lambda_norm are using the GPU (occupancy, processor usage, memory bandwidth).

Lambda Norm definitely seems to have the worst performance. I'm actually quite
surprised by this, Iwould have thought that lambdas would be low overhead and
thus quite fast but it seems that it performs by far the worst. If I were to
hazard a guess, my placement of the definition was not optimal and it may be
recreating it each time it loops, adding overhead. That, or somehow by using
"auto" for the type...I'm not sure.

As for the nvprof output...I got a profiling error on pretty much every single
run. I'm not sure why.

It looks like the most varied value is the memory throughput -- it seems to
roughly correlate with the level of performance (although lambda norm bucks
this trend), which I recall being told memory bandwidth is often one of the
most critical bottlenecks in GPU compute. That is seemingly supported by these
results.

#### About PS7
**Q**
The most important thing I learned from this assignment was ...

CUDA is actually shockingly accesible. Thrust makes it so easy I'm appalled I
don't see more GPU accelerated code out there. Will definitely keep it in mind
if I ever find the use Python wrapped code won't meet my performance needs at
work. I also wonder what the latency is like -- FPGA's are already starting to
be used to accelerate audio processing tasks (such as "zero latency VST's"),
but seeing as operations like the FFT are prime targets for GPU operations,
would it be feasible to use heterogenous compute techniqes in such a field? 

**Q**
One thing I am still not clear on is ...

It seemed that the red herring I was running into with my CUDA norm code was
the difference in results between the results of the CUDA norm and an
"equivalent" sequential norm. Since the answers diverged with increasing
problem size, I'm chalking this up to floating point error. Are there
optimizations (besides the use of doubles) that can be implemented to minimize
this?
