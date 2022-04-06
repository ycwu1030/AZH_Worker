#ifndef AZH_PARAMETER_H_
#define AZH_PARAMETER_H_

#include <string>

#include "AZHDistribution.h"

class AZHParameter_t {
public:
    double MHA;
    double MHH;
    double WHA;
    double WHH;
    double tb;
    double cba;
    Distribution_t dist;

    AZHParameter_t(double MHA_IN, double MHH_IN, double WHA_IN, double WHH_IN, double tb_IN, double cba_IN,
                   const char* root_file_name);
    void Dump_Distribution(const char* output_file_name);
};

#endif  // AZH_PARAMETER_H_
