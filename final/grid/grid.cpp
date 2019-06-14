#include <iostream>
#include <mpi.h>

#include "Grid.hpp"
#include "Stencil.hpp"
#include "Timer.hpp"
#include "cg.hpp"
#include "ir.hpp"
#include "jacobi.hpp"
#include "mpiStencil.hpp"

#include "Final.hpp"

enum method { with_ir, with_jacobi, with_cg };

void usage(const std::string& msg) { std::cout << "Usage: " << msg << std::endl; }

int main(int argc, char* argv[]) {

  bool   seq   = true;
  method mtd   = with_jacobi;
  size_t xsize = 128, ysize = 128;
  size_t maxiter = 128;
  double tolerance = 1.e-6;
  bool debug = false;

  try {
    for (int arg = 1; arg < argc; ++arg) {
      if (std::string(argv[arg]) == "-p") {
        seq = false;
      } else if (std::string(argv[arg]) == "-x") {
        if (argc == ++arg) usage(argv[0]);
        xsize = std::stol(argv[arg]);
      } else if (std::string(argv[arg]) == "-y") {
        if (argc == ++arg) usage(argv[0]);
        ysize = std::stol(argv[arg]);
      } else if (std::string(argv[arg]) == "-m") {
        if (argc == ++arg) usage(argv[0]);
        maxiter = std::stol(argv[arg]);
      } else if (std::string(argv[arg]) == "-j") {
        mtd = with_jacobi;
      } else if (std::string(argv[arg]) == "-c") {
        mtd = with_cg;
      } else if (std::string(argv[arg]) == "-i") {
        mtd = with_ir;
      } else if (std::string(argv[arg]) == "-d") {
	debug = true;
      } else if (std::string(argv[arg]) == "-t") {
        if (argc == ++arg) usage(argv[0]);
        tolerance = std::stod(argv[arg]);
      }
    }
  } catch (int) {
    usage(argv[0]);
    return -1;
  }

  if (debug) {
    std::cout << "Parallel " << (seq ? "No" : "Yes") << std::endl;
    std::cout << "maxiter " << maxiter << std::endl;    
    std::cout << "xsize " << xsize << std::endl;
    std::cout << "ysize " << ysize << std::endl;        
    switch (mtd) {
    case with_jacobi:
      break;
    case with_ir:
      break;
    case with_cg:
      break;
    default:
      std::cerr << "This should never happen" << std::endl;
      abort();
      break;
    }
  }

  if (seq) {
    Grid X0(xsize, ysize), X1(xsize, ysize);

    for (size_t i = 0; i < ysize+2; ++i) {
      X1(0, i) = X0(0, i) = ((double) i) / ((double) ysize+1);
    }
#if 1
    for (size_t j = 0; j < xsize+2; ++j) {
      X1(j, ysize+1) = X0(j, ysize+1) = ((double) j) / ((double) xsize+1);
    }
#endif

    Stencil A;

    DEF_TIMER(seqgrid);
    START_TIMER(seqgrid);
    switch(mtd) {
    case with_jacobi:
      jacobi(A, X1, X0, maxiter, tolerance);
      break;
    case with_ir:
      ir(A, X1, X0, maxiter, tolerance);
      break;
    case with_cg:
      cg(A, X1, X0, maxiter, tolerance);
      break;
    default:
      std::cerr << "This should never happen" << std::endl;
      abort();
      break;
    }
    STOP_TIMER(seqgrid);
  } else {

    MPI::Init();
    size_t myrank = MPI::COMM_WORLD.Get_rank();
    size_t mysize = MPI::COMM_WORLD.Get_size();
    
    if (debug) {
      std::cout << "comm size " << mysize << std::endl;
      std::cout << "comm rank " << myrank << std::endl;
    }

    size_t lsize = xsize/mysize;

    Grid X0(lsize, ysize), X1(lsize, ysize);

    if (0 == myrank) {
      for (size_t i = 0; i < ysize+2; ++i) {
	X1(0, i) = X0(0, i) = ((double) i) / ((double) ysize+1);
      }
    }

#if 1
    for (size_t j = 0; j < lsize+2; ++j) {
      size_t k = myrank*lsize + j;
      X1(j, ysize+1) = X0(j, ysize+1) = ((double) k) / ((double) xsize+1);
    }
#endif

    mpiStencil A;

    DEF_TIMER(mpigrid);
    START_TIMER(mpigrid);
    switch(mtd) {
    case with_jacobi:
      jacobi(A, X1, X0, maxiter, tolerance);
      break;
    case with_ir:
      ir(A, X1, X0, maxiter, tolerance);
      break;
    case with_cg:
      cg(A, X1, X0, maxiter, tolerance);
      break;
    default:
      std::cerr << "This should never happen" << std::endl;
      abort();
      break;
    }
    if (myrank == 0) {
      STOP_TIMER(mpigrid);
    } else {
      // I tried to get rid of the return but it seems SOMEONE is using macros that
      // prevent that...
      double foo = STOP_TIMER_QUIETLY(mpigrid);
    }
    MPI::Finalize();
  }
  return 0;
}
