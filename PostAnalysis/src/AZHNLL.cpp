#include "AZHNLL.h"

AZHNLL::AZHNLL(Distribution_t *sig, Distribution_t *bkg) : sig_(sig), bkg_(bkg) {}

double AZHNLL::get_nll(const double lumi, const double mu_sig) {
    double nll = 0;
    double n_sig;
    double n_bkg;
    for (int i = 0; i < sig_->NBINS; i++) {
        n_sig = mu_sig * lumi * sig_->data_cs[i];
        n_bkg = lumi * bkg_->data_cs[i];
        nll += NLL(n_sig + n_bkg, n_bkg);
    }
    return nll;
}

double AZHNLL::get_mu_at_95CL(const double lumi) {
    static const double NLL_at_95CL = 3.84;
    double mu_min = -1;
    double mu_max = -1;
    double tmp_nll = get_nll(lumi, 1.0);
    double tmp_mu;
    if (tmp_nll < NLL_at_95CL) {
        mu_min = 1.0;
        tmp_mu = 2.0;
    } else {
        mu_max = 1.0;
        tmp_mu = 0.5;
    }
    while (true) {
        tmp_nll = get_nll(lumi, tmp_mu);
        if (tmp_nll < NLL_at_95CL) {
            // if (mu_min < 0 || tmp_nll > mu_min) {
            mu_min = tmp_mu;
            // }
        } else {
            // if (mu_max < 0 || tmp_nll < mu_max) {
            mu_max = tmp_mu;
            // }
        }
        if (mu_min < 0) {
            tmp_mu /= 2;
        } else if (mu_max < 0) {
            tmp_mu *= 2;
        } else {
            tmp_mu = (mu_min + mu_max) / 2.0;
        }
        if (fabs(mu_max - mu_min) < 1e-5) {
            break;
        }
    }
    return (mu_max + mu_min) / 2.0;
}
