---
title: PS5 Questions
---

### PS5 Written Questions

#### Norm!

**Q**
At what problem size do the answers between the computed norms start to differ?

The first "real" difference was observed when N=13, with an abs difference of ~4.44E-16.

**Q**
How do the absolute and relative errors change as a function of problem size?

Both increase as a function of problem size; absolute increases much faster. By N=100,000,000 the
absolute difference is on the order of 1e-09, and relative is on the order of 2e-13 (where
it seems to stay, approximately).

**Q**
Does the `Vector` class behave strictly like a member of an  abstract vector class?

No, floats/doubles do not add associatively. Thus, changing their order changes the result.

**Q**
Do you have any concerns about this kind of behavior?

Of course, but generally it can be disregarded for "real world" problems. This of course
depends on the specific problem, but generally I wouldn't be fussed if error introduced
is on the order of 10^-6 or less. The fact that relative error seems to stay exceedingly small
is likely due to the nature of floating point variables.


#### Norm! (Partition Edition)

**Q**
At what number of threads do you start to see a difference between `norm4` and `norm3`?  Note\
 that you can run as many threads as you like (recall the difference between concurrency and pa\
rallelism).

**Q**
Is this difference always the same?

**Q**
Do you have any concerns about this kind of behavior?


#### Norm! (Recursion Version)

**Q**
Was there any appreciable difference between the partitioned and the recursive versions?



#### About PS5


**Q** The most important thing I learned from this assignment was ...


**Q** One thing I am still not clear on is ...



