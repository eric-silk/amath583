//
// This file is part of the course materials for AMATH483/583 at the University of Washington,
// Spring 2019
//
// Licensed under Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License
// https://creativecommons.org/licenses/by-nc-sa/4.0/
//

#include <iostream>
#include <vector>
#include "Timer.hpp"

int main(int argc, char *argv[]) {
  Timer t;    
  size_t dim = argv[1]; // FIXME

  t.start();
  std::vector<double> dx(dim, 3.14);
  std::vector<double> dy(dim, 27.0);
  std::vector<double> dz(dim, 0.0);
  t.stop();
  std::cout << "Construction time for double: " << t.elapsed() << std::endl;

  t.start();
  for (size_t i = 0; i < dim; ++i) {
    dz[i] = dx[i] * dy[i];
  }
  t.stop();
  std::cout << "Multiplication time for double: " << t.elapsed() << std::endl;


  t.start();
  std::vector<double> fx(dim, 3.14);
  std::vector<double> fy(dim, 27.0);
  std::vector<double> fz(dim, 0.0);
  t.stop();
  std::cout << "Construction time for float: " << t.elapsed() << std::endl;

  t.start();
  for (size_t i = 0; i < dim; ++i) {
    fz[i] = fx[i] * fy[i];
  }
  t.stop();
  std::cout << "Multiplication time for float: " << t.elapsed() << std::endl;


  return 0;
}
