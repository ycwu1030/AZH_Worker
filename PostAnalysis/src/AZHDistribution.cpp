#include "AZHDistribution.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "AZHPreAnalysis.h"
#include "TFile.h"
#include "TH2F.h"
#include "TTree.h"

using namespace std;

Distribution_t::Distribution_t(int nbin) : NBINS(nbin), data_cs(nbin, 0), data_mc_count(nbin, 0), cs(0), n_mc(0) {}

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

Distribution_t::Distribution_t(const char *distribution_file, int null_data) {
    ifstream infile(distribution_file);
    string line;
    getline(infile, line);  // first line is the title
    getline(infile, line);
    stringstream sstr(line);
    string tok;
    getline(sstr, tok, '\t');  // skip MHA
    getline(sstr, tok, '\t');  // skip MHH
    getline(sstr, tok, '\t');  // skip WHA
    getline(sstr, tok, '\t');  // skip WHH
    getline(sstr, tok, '\t');  // skip tb
    getline(sstr, tok, '\t');  // skip cba
    getline(sstr, tok, '\t');
    cs = atof(tok.c_str());
    while (getline(sstr, tok, '\t')) {
        data_cs.push_back(atof(tok.c_str()));
    }
    NBINS = data_cs.size();

    getline(infile, line);
    stringstream sstr_mc(line);
    getline(sstr_mc, tok, '\t');  // skip MHA
    getline(sstr_mc, tok, '\t');  // skip MHH
    getline(sstr_mc, tok, '\t');  // skip WHA
    getline(sstr_mc, tok, '\t');  // skip WHH
    getline(sstr_mc, tok, '\t');  // skip tb
    getline(sstr_mc, tok, '\t');  // skip cba
    getline(sstr_mc, tok, '\t');
    n_mc = atoi(tok.c_str());
    while (getline(sstr_mc, tok, '\t')) {
        data_mc_count.push_back(atof(tok.c_str()));
    }
}

bool Distribution_t::cut(AZHPreAnalysis *del) {
    static const double mz = 91.1776;
    if (abs(del->MOSSF_BEST - mz) > 15.0) return false;
    if (del->MET < 30.0) return false;
    return true;
}

Distribution_t Distribution_t::operator+(const Distribution_t &dist) {
    Distribution_t dist_rest(this->NBINS);
    for (int i = 0; i < this->NBINS; i++) {
        dist_rest.data_cs[i] = this->data_cs[i] + dist.data_cs[i];
        dist_rest.data_mc_count[i] = this->data_mc_count[i] + dist.data_mc_count[i];
    }
    dist_rest.cs = this->cs + dist.cs;
    dist_rest.n_mc = this->n_mc + dist.n_mc;
    return dist_rest;
}

Distribution_t Distribution_t::operator*(const double scale) {
    Distribution_t dist_rest(this->NBINS);
    for (int i = 0; i < this->NBINS; i++) {
        dist_rest.data_cs[i] = this->data_cs[i] * scale;
        dist_rest.data_mc_count[i] = this->data_mc_count[i];
    }
    dist_rest.cs = this->cs * scale;
    dist_rest.n_mc = this->n_mc;
    return dist_rest;
}

Distribution_t &Distribution_t::operator+=(const Distribution_t &dist) {
    // cout << "current size: " << this->NBINS << " Addon size: " << dist.NBINS << endl;
    for (int i = 0; i < this->NBINS; i++) {
        data_cs[i] += dist.data_cs[i];
        data_mc_count[i] += dist.data_mc_count[i];
    }
    cs += dist.cs;
    n_mc += dist.n_mc;
    return *this;
}
