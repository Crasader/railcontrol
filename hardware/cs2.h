#ifndef HARDWARE_CS2_H
#define HARDWARE_CS2_H

#include <cstring>

#include "hardware.h"

namespace hardware {

  class cs2 {
    public:
      int start(struct params &params);
      int stop();
      std::string name();
    private:
      void receiver();
      void sender();
      volatile unsigned char run;
  };

} // namespace

#endif // HARDWARE_CS2_H

