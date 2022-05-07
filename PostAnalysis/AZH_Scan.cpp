#include <fstream>
#include <iostream>
#include <string>

#include "AZHGrid.h"
#include "AZHNLL.h"
#include "TFile.h"
#include "TTree.h"

using namespace std;

int main(int argc, char const *argv[]) {
    //* 0: exe name
    //* 1: root file of the EWPT points
    //* 2: output
    Distribution_t dist_bkg("../DATADIR/Distribution/BKG/COMB/AZH_Distribution_bkg_total_BKG_COMB_DEFAULT_3l.txt", 0);
    TFile *f1 = new TFile(argv[1]);
    TTree *t1 = (TTree *)f1->Get("EWPT_Data");
    int LID;
    double MHA, MHH, tbeta, alpha, WHA, WHH;
    t1->SetBranchAddress("LID", &LID);
    t1->SetBranchAddress("mA", &MHA);
    t1->SetBranchAddress("mHh", &MHH);
    t1->SetBranchAddress("tbeta", &tbeta);
    t1->SetBranchAddress("alpha", &alpha);
    t1->SetBranchAddress("w_A", &WHA);
    t1->SetBranchAddress("w_Hh", &WHH);
    ofstream output(argv[2]);
    output << "LID\tAZHtt_NLL\tHZAtt_NLL" << endl;
    for (int i = 0; i < t1->GetEntries(); i++) {
        if ((i + 1) % 1000 == 0) cout << "Entry: " << i + 1 << endl;
        t1->GetEntry(i);
        double cba = cos(atan(tbeta) - alpha);
        AZHPoint p_sig_AZH(MHA, MHH, WHA, WHH, tbeta, cba);
        HZAPoint p_sig_HZA(MHA, MHH, WHA, WHH, tbeta, cba);
        double nll_AZH = 0;
        if (p_sig_AZH.within_region()) {
            Distribution_t &dist_sig = p_sig_AZH.get_distribution();
            AZHNLL nll_calc(&dist_sig, &dist_bkg);
            nll_AZH = nll_calc.get_nll();
        }
        double nll_HZA = 0;
        if (p_sig_HZA.within_region()) {
            Distribution_t &dist_sig = p_sig_HZA.get_distribution();
            AZHNLL nll_calc(&dist_sig, &dist_bkg);
            nll_HZA = nll_calc.get_nll();
        }
        output << LID << "\t" << nll_AZH << "\t" << nll_HZA << endl;
    }

    return 0;
}
