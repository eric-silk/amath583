---
title: PS3 Questions
---

### PS3 Written Questions

Add your answers to this file in plain text after each question.  Leave a blank line between the text of the question and the text of your answer.

#### argv


**Q1** What does `argv[0]` always contain?

If argc is greater than 0, the string pointed to by argv[0] represents the program name.
Source: https://stackoverflow.com/a/2051031/8341166


**Q2** Which entry of `argv` holds the first argument passed to the program?

Assuming:
./foo arg1 arg2

where arg1 is the "first argument", then:
argv[1]


**Q3** Which entry of `argv` holds the second argument passed to the program?

Assuming:
./foo arg1 arg2

where arg2 is the "second argument", then:
argv[2]


**Q4** How would you print just the last argument passed to a program?

// within main, with all headers, etc. etc...

std::cout << argv[argc-1] << std::endl;


#### float vs double

(**NB:**  Q5 and Q6 compare float and double for -O0, Q7 and Q8 for -O3.)

**Q5**  What is the difference (ratio) in execution times 
between single and double precision for
    construction **without** optimization? Explain.

My explanation for all results will be in Q9.
On average (n=3), the doubles take 1.325 times as long to construct.

**Q6**  What is the difference (ratio) in execution times
between single and double precision for
    multiplication **without** optimization? Explain.

On average, the double takes 1.02 times as long to multiply.

**Q7**  What is the difference (ratio) in execution times 
between single and double precision for
    construction **with** optimization? Explain.

On average, the double takes 2.05 times as long to construct.


**Q8**  What is the difference (ratio) in execution times 
between single and double precision for 
    multiplication **with** optimization? Explain. 

On average, the double takes 2.02 times as long to multiply.


**Q9**  What is the difference (ratio) in execution times 
for double precision
    multiplication with and without optimization? Explain. 

On average, the unoptimized double takes 3.74 times as long to multiply.

When not optimizing, loops are not unrolled, small functions are not inlined, and I suspect that
pipelining is not used, requiring the CPU to wait longer per instruction. Additionally, "true" FPU
multiplication may not be occuring for doubles.

When optimizing, the above operations ARE applied. As such, more instructions are stored in the
cache (minimizing time required to fetch them), pipelining occurs (no waiting for each instruction
to finish), and the proper use of an FPU for both floats AND doubles occurs.


#### efficiency

**Q10** What changes did you make to the 
    code, if any?

I added a printout of the final value of "c".

**Q11** Why did you make those changes?

Without a use for the value "c", the compiler identifies the loop as "dead code" and removes it.
This is counter to the intended purpose.

**Q12** What is the clock rate of your computer?

3.50GHz.

**Q13** What is the max achieved single and double precision floating point rate of your timed code (with and
    without optimization)?  What is the efficiency?

I think you're looking for efficiency as a measure of actual FLOPS per cycle compared to the
theoretical maximum if each cycle was one FLOP?

Assumptions: Adds and multiplies are one flop operation each and take 1 CPU cycle,
the CPU frequency isn't changing dynamically (i.e. none of this TurboBoost stuff),
the CPU doesn't support MAC (Multiply Accumulate) operations (i.e. the multiplication and
accumulation each take one FLOP). 

The tests were ran for 1 billion loops.  Then:

Efficiency = (FLOPS/time elapsed) / (Cycles/second)

Efficiency, non-optimized = ((2x10^9)/(2.319)/(3.5x10^9) ~= 0.264, or ~26% efficiency

Efficiency, optimized = ((2x10^9)/(0.773)/(3.5x10^9) ~= 0.73923, or ~74% efficiency

#### operator*

**Q14** Does the `operator*` performance match the hand-written variant? If not, 
explain. 

No. The hand-written version, for arguments of 1000 1000, on my machine reports 0.017; the
`operator*` version reports 0.028. Enabling optimization in the Makefile seemed to bring them
within spitting distance of each other? I'm guessing that the issue is the overhead associated
with each operator call is causing the difference in performance, and when enabling the optimizations
the function call is inlined, removing the repeated overhead. That, and/or there's some loop
unrolling that occurs.


**Extra credit:**  Can you think of a way to circumvent the problem
(if there was a problem)?

Enable optimizations or provide the "inline" keyword?


#### About PS3


**Q15** The most important thing I learned from this assignment was ...

Compiler optimization flags are great. **BUT**, they'll only get you so far.

**Q16** One thing I am still not clear on is ...

How, when there are **so many** different flags, are we supposed to realistically identify which are
responsible for any performance increases? And, if so, why aren't they always turned on? I know there
are limits to this, and sometimes higher optimization levels can actually detriment performance, but
if this is an exception to the rule...why not default to high optimization and optionally lower it?
