#ifndef AZH_DISTRIBUTION_H_
#define AZH_DISTRIBUTION_H_

#include <string>
#include <vector>

#include "AZHPreAnalysis.h"

class Distribution_t {
public:
    Distribution_t(const char* root_file);
    // Distribution_t(std::stringstream &sstr);
    ~Distribution_t(){};

    bool cut(AZHPreAnalysis*);
    std::vector<double> data;
    double cs;  // * For the distribution
    int NBINS;
};

#endif  // AZH_DISTRIBUTION_H_
