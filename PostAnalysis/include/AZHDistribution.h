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
    std::vector<double> data_cs;
    std::vector<double> data_mc_count;
    double cs;    // * For the distribution
    double n_mc;  // * number of mc events
    int NBINS;
};

#endif  // AZH_DISTRIBUTION_H_
