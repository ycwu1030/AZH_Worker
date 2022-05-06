#ifndef AZH_DISTRIBUTION_H_
#define AZH_DISTRIBUTION_H_

#include <fstream>
#include <string>
#include <vector>

#include "AZHPreAnalysis.h"
#include "TH2F.h"

class Distribution_t {
public:
    Distribution_t(int nbin);
    Distribution_t(const char* root_file);
    Distribution_t(const char* distribution_file, int null_data);
    ~Distribution_t(){};

    bool cut(AZHPreAnalysis*);
    std::vector<double> data_cs;
    std::vector<double> data_mc_count;
    double cs;  // * For the distribution
    int n_mc;   // * number of mc events
    int NBINS;

    Distribution_t operator+(const Distribution_t&);
    Distribution_t operator*(const double scale);
    Distribution_t& operator+=(const Distribution_t&);

    void set_histogram(char* hist_name, TH2F* h2);
};

#endif  // AZH_DISTRIBUTION_H_
