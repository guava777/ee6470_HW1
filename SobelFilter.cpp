#include <cmath>
#include <vector>
#include <algorithm>
#include "SobelFilter.h"
using namespace std;

SobelFilter::SobelFilter(sc_module_name n) : sc_module(n) {
  SC_THREAD(do_filter);
  sensitive << i_clk.pos();
  dont_initialize();
  reset_signal_is(i_rst, false);
}

const int mask[9] = {1, 1, 1, 1, 2, 1, 1, 1, 1};

void SobelFilter::do_filter() {
  unsigned int sum = 0;
  vector<unsigned int> buffer(9);
  while(true){
    for(unsigned int i = 0; i < 9; i++){
      unsigned int grey = (i_r.read() + i_g.read() + i_b.read()) / 3;
      buffer[i] = grey;
    }
    sort(buffer.begin(), buffer.end());

    for(unsigned int i = 0; i < 9; i++){
      sum += buffer[i] * mask[i];
    }
    unsigned int mean = sum / 10;
    o_result.write(mean);
    sum = 0;
    wait(1);
  }
}