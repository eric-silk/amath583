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

**Q11** Why did you make those changes?

**Q12** What is the clock rate of your computer?

**Q13** What is the max achieved single and double precision floating point rate of your timed code (with and
    without optimization)?  What is the efficiency?


#### operator*

**Q14** Does the `operator*` performance match the hand-written variant? If not, 
explain. 


**Extra credit:**  Can you think of a way to circumvent the problem
(if there was a problem)?


#### About PS3


**Q15** The most important thing I learned from this assignment was ...


**Q16** One thing I am still not clear on is ...
