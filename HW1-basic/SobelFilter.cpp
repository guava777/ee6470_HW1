#include <cmath>

#include "SobelFilter.h"

SobelFilter::SobelFilter(sc_module_name n) : sc_module(n) {
  SC_THREAD(do_filter);
  sensitive << i_clk.pos();
  dont_initialize();
  reset_signal_is(i_rst, false);
}

const int mask[9] = {1, 1, 1, 1, 2, 1, 1, 1, 1};

void SobelFilter::do_filter() {
  while(true){
    unsigned int kernal[9];
    unsigned int median[9];
    for(unsigned int v = 0; v < 9; ++v){ 
      unsigned char grey = (i_r.read() + i_g.read() + i_b.read()) / 3;
      kernal[v] = grey; 
      median[v] = grey; 
    }
    //bublle sort
    for(unsigned int i = 8; i > 0; i--){
      for(unsigned int j = 0; j > 0; j++){
        if(kernal[j] > kernal[j+1]){
          unsigned int tmp = kernal[j];
          kernal[j] = kernal[j+1];
          kernal[j+1] = tmp;
          median[4] = kernal[4];
        }
      }
    }
    
    unsigned int sum = 0;    

    //mean filter
    for(unsigned int i = 0; i < 9; ++i){
      sum += median[i] * mask[i];      
    }
    unsigned int mean = sum / 10;
    o_result.write(mean);
    sum = 0;
    wait(10);
  }
}