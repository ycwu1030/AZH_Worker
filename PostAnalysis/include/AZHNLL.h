#ifndef AZH_NLL_H_
#define AZH_NLL_H_

#include "AZHDistribution.h"

inline double NLL(double obs, double exp) {
    // * This is -2*log(L)
    if (exp < 1e-5) return 0;
    if (obs < 1e-5) return 2 * exp;
    return 2 * (exp - obs + obs * log(obs / exp));
}

class AZHNLL {
public:
    AZHNLL(Distribution_t *sig, Distribution_t *bkg);
    ~AZHNLL(){};

    double get_nll(const double lumi = 3000, const double mu_sig = 1.0);
    double get_mu_at_95CL(const double lumi = 3000);

private:
    Distribution_t *sig_;
    Distribution_t *bkg_;
};

#endif  // AZH_NLL_H_
