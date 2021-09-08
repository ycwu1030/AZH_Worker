#ifndef __NLL_CALCULATOR_H__
#define __NLL_CALCULATOR_H__
#include <string>

#include "TH2F.h"

class NLL_Calculator {
private:
    TH2F *h_sig;
    TH2F *h_bkg;

public:
    NLL_Calculator(const char *sig_filename, const char *bkg_filename, const int NBINX = 20, const double X_LOW = 350,
                   const double X_HIGH = 1000, const int NBINY = 20, const double Y_LOW = 400,
                   const double Y_HIGH = 1000);
    ~NLL_Calculator();

    double Get_NLL(const double lumi = 3000, const double mu_S = 1);
    double Get_mu_at_95CL(const double lumi = 3000);
};

#endif  //__NLL_CALCULATOR_H__
