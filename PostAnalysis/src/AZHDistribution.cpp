#include "AZHDistribution.h"

#include "AZHPreAnalysis.h"
#include "TFile.h"
#include "TH2F.h"
#include "TTree.h"

using namespace std;

Distribution_t::Distribution_t(const char *root_file) {
    TFile *f = new TFile(root_file);
    TTree *t = (TTree *)f->Get("AZHPreAnalysis");
    AZHPreAnalysis *del = new AZHPreAnalysis(t);

    int NBINX = 20;  // * for mtt;
    int NBINY = 20;  // * for mztt;
    double X_LOW = 350;
    double X_HIGH = 1000;
    double Y_LOW = 400;
    double Y_HIGH = 1000;

    TH2F *hist_cs = new TH2F("hist_cs", "", NBINX, X_LOW, X_HIGH, NBINY, Y_LOW, Y_HIGH);
    TH2F *hist_mc = new TH2F("hist_mc", "", NBINX, X_LOW, X_HIGH, NBINY, Y_LOW, Y_HIGH);

    cs = 0;
    n_mc = 0;
    NBINS = NBINX * NBINY;
    for (int i = 0; i < t->GetEntries(); i++) {
        del->GetEntry(i);
        bool good = cut(del);
        if (!good) continue;
        hist_cs->Fill(del->mtt, del->mztt, del->weight * 1000);
        hist_mc->Fill(del->mtt, del->mztt);
        cs += del->weight * 1000;
        n_mc += 1;
    }

    int nbinall;
    for (int i = 0; i < NBINX; i++) {
        for (int j = 0; j < NBINY; j++) {
            nbinall = hist_cs->GetBin(i + 1, j + 1);
            data_cs.push_back(hist_cs->GetBinContent(nbinall));
            nbinall = hist_mc->GetBin(i + 1, j + 1);
            data_mc_count.push_back(hist_mc->GetBinContent(nbinall));
        }
    }
    delete hist_cs;
    delete hist_mc;
    delete del;
}

bool Distribution_t::cut(AZHPreAnalysis *del) {
    static const double mz = 91.1776;
    if (abs(del->MOSSF_BEST - mz) > 15.0) return false;
    if (del->MET < 30.0) return false;
    return true;
}
