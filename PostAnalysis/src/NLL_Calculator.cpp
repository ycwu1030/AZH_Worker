#include "NLL_Calculator.h"

#include <iostream>

#include "AZHPreAnalysis.h"
#include "TFile.h"
#include "Utilities.h"
using namespace std;
NLL_Calculator::NLL_Calculator(const char *sig_filename, const char *bkg_filename, const int NBINX, const double X_LOW,
                               const double X_HIGH, const int NBINY, const double Y_LOW, const double Y_HIGH) {
    TFile *sig = new TFile(sig_filename);
    TFile *bkg = new TFile(bkg_filename);
    TTree *t_sig = (TTree *)sig->Get("AZHPreAnalysis");
    TTree *t_bkg = (TTree *)bkg->Get("AZHPreAnalysis");
    AZHPreAnalysis *del_sig = new AZHPreAnalysis(t_sig);
    AZHPreAnalysis *del_bkg = new AZHPreAnalysis(t_bkg);

    h_sig = new TH2F("h_sig", "", NBINX, X_LOW, X_HIGH, NBINY, Y_LOW, Y_HIGH);
    h_bkg = new TH2F("h_bkg", "", NBINX, X_LOW, X_HIGH, NBINY, Y_LOW, Y_HIGH);

    for (int i = 0; i < t_sig->GetEntries(); i++) {
        del_sig->GetEntry(i);
        if ((i + 1) % 1000 == 0) {
            cout << "NSIG: " << i + 1 << "\r";
        }
        h_sig->Fill(del_sig->mtt, del_sig->mztt, del_sig->Weight * 1000);
    }
    cout << endl;
    for (int i = 0; i < t_bkg->GetEntries(); i++) {
        del_bkg->GetEntry(i);
        if ((i + 1) % 1000 == 0) {
            cout << "NBKG: " << i + 1 << "\r";
        }
        h_bkg->Fill(del_bkg->mtt, del_bkg->mztt, del_bkg->Weight * 1000);
    }
    cout << endl;
}

NLL_Calculator::~NLL_Calculator() {
    delete h_sig;
    delete h_bkg;
}

double NLL_Calculator::Get_NLL(const double lumi, const double mu_S) {
    double nll = 0;
    int nbinall;
    double N_SIG;
    double N_BKG;
    for (int i = 0; i < h_sig->GetNbinsX(); i++) {
        for (int j = 0; j < h_sig->GetNbinsY(); j++) {
            nbinall = h_sig->GetBin(i + 1, j + 1);
            N_SIG = h_sig->GetBinContent(nbinall) * lumi * mu_S;
            nbinall = h_bkg->GetBin(i + 1, j + 1);
            N_BKG = h_bkg->GetBinContent(nbinall) * lumi;
            nll += NLL(N_SIG + N_BKG, N_BKG);
            // cout << " MTT = " << h_sig->GetXaxis()->GetBinCenter(i + 1);
            // cout << " MZTT = " << h_sig->GetYaxis()->GetBinCenter(j + 1);
            // cout << "N_SIG: " << N_SIG << " N_BKG: " << N_BKG << " NLL: " << NLL(N_SIG + N_BKG, N_BKG) << endl;
        }
    }
    return nll;
}

double NLL_Calculator::Get_mu_at_95CL(const double lumi) {
    static const double NLL_at_95CL = 3.84;
    double mu_min = -1;
    double mu_max = -1;
    double tmp_nll = Get_NLL(lumi, 1.0);
    double tmp_mu;
    if (tmp_nll < 3.84) {
        mu_min = 1.0;
        tmp_mu = 2.0;
    } else {
        mu_max = 1.0;
        tmp_mu = 0.5;
    }
    while (true) {
        tmp_nll = Get_NLL(lumi, tmp_mu);
        if (tmp_nll < 3.84) {
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

NLL_Calculator_VEC::NLL_Calculator_VEC(const std::vector<double> VSIG_, const std::vector<double> VBKG_)
    : VSIG(VSIG_), VBKG(VBKG_){};

double NLL_Calculator_VEC::Get_NLL(const double lumi, const double mu_S) {
    double nll = 0;
    int nbinall;
    double N_SIG;
    double N_BKG;
    for (int i = 0; i < VSIG.size(); i++) {
        N_SIG = VSIG[i] * lumi * mu_S;
        N_BKG = VBKG[i] * lumi;
        nll += NLL(N_SIG + N_BKG, N_BKG);
    }
    return nll;
}

double NLL_Calculator_VEC::Get_mu_at_95CL(const double lumi) {
    static const double NLL_at_95CL = 3.84;
    double mu_min = -1;
    double mu_max = -1;
    double tmp_nll = Get_NLL(lumi, 1.0);
    double tmp_mu;
    if (tmp_nll < 3.84) {
        mu_min = 1.0;
        tmp_mu = 2.0;
    } else {
        mu_max = 1.0;
        tmp_mu = 0.5;
    }
    while (true) {
        tmp_nll = Get_NLL(lumi, tmp_mu);
        if (tmp_nll < 3.84) {
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
