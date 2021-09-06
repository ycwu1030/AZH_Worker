#include "Plot_Pool.h"

#include <iostream>

#include "TFile.h"

using namespace std;

Plot_Pool::Plot_Pool() : NVAR(0) {
    for (int i = 0; i < MAX_VARS; i++) {
        NBINX[i] = 0;
        X_LOW[i] = 0;
        X_HIGH[i] = 0;
        VARIABLE_X_D[i] = nullptr;
        VARIABLE_X_I[i] = nullptr;
    }
}

Plot_Pool::~Plot_Pool() {
    Clean_Plots();
    Clean_Pads();
}

void Plot_Pool::Add(const int nbinx, const double x_low, const double x_high, const char *var_name, const char *title,
                    int *var) {
    if (NVAR >= 0 && NVAR < MAX_VARS) {
        NBINX[NVAR] = nbinx;
        X_LOW[NVAR] = x_low;
        X_HIGH[NVAR] = x_high;
        TITLE_X[NVAR] = title;
        NAME_X[NVAR] = var_name;
        VARIABLE_X_I[NVAR] = var;
        NVAR++;
    } else {
        cout << "No More Room For Variables in this Plot Pool" << endl;
    }
}

void Plot_Pool::Add(const int nbinx, const double x_low, const double x_high, const char *var_name, const char *title,
                    double *var) {
    if (NVAR >= 0 && NVAR < MAX_VARS) {
        NBINX[NVAR] = nbinx;
        X_LOW[NVAR] = x_low;
        X_HIGH[NVAR] = x_high;
        TITLE_X[NVAR] = title;
        NAME_X[NVAR] = var_name;
        VARIABLE_X_D[NVAR] = var;
        NVAR++;
    } else {
        cout << "No More Room For Variables in this Plot Pool" << endl;
    }
}

double Plot_Pool::Get_Value_Var(int i) {
    double tmp = -99999;
    if (i >= 0 && i < NVAR) {
        if (VARIABLE_X_D[i]) {
            tmp = *VARIABLE_X_D[i];
        } else if (VARIABLE_X_I[i]) {
            tmp = *VARIABLE_X_I[i];
        }
    }
    return tmp;
}

void Plot_Pool::Clean_Plots() {
    for (int i = 0; i < HIST_SIG.size(); i++) {
        delete HIST_SIG[i];
    }
    for (int i = 0; i < HIST_BKG.size(); i++) {
        delete HIST_BKG[i];
    }

    HIST_SIG.clear();
    NORM_SIG.clear();
    HIST_BKG.clear();
    NORM_BKG.clear();
}
void Plot_Pool::Clean_Pads() {
    for (int i = 0; i < STK.size(); i++) {
        delete STK[i];
    }
    // for (int i = 0; i < STK_NORM.size(); i++) {
    //     delete STK_NORM[i];
    // }
    for (int i = 0; i < CANVAS.size(); i++) {
        delete CANVAS[i];
    }
    // for (int i = 0; i < CANVAS_NORM.size(); i++) {
    //     delete CANVAS_NORM[i];
    // }

    for (int i = 0; i < LG.size(); i++) {
        delete LG[i];
    }

    STK.clear();
    // STK_NORM.clear();
    CANVAS.clear();
    // CANVAS_NORM.clear();
    LG.clear();
}

void Plot_Pool::Prepare_Plots() {
    Clean_Plots();
    TH1F *p;
    char name[200];
    char title[200];
    for (int i = 0; i < NVAR; i++) {
        // * Histogram for Signal
        sprintf(name, "HSIG_VAR-%d_%s", i + 1, NAME_X[i].c_str());
        p = new TH1F(name, "", NBINX[i], X_LOW[i], X_HIGH[i]);
        p->Sumw2();
        p->GetXaxis()->SetTitle(TITLE_X[i].c_str());
        p->SetLineWidth(2);
        HIST_SIG.push_back(p);
        NORM_SIG.push_back(0);

        // * Histogram for Background
        sprintf(name, "HBKG_VAR-%d_%s", i + 1, NAME_X[i].c_str());
        p = new TH1F(name, "", NBINX[i], X_LOW[i], X_HIGH[i]);
        p->Sumw2();
        p->GetXaxis()->SetTitle(TITLE_X[i].c_str());
        p->SetLineWidth(2);
        HIST_BKG.push_back(p);
        NORM_BKG.push_back(0);
    }
}

void Plot_Pool::Prepare_Pads() {
    Clean_Pads();
    THStack *s;
    TCanvas *c;
    TLegend *l;
    char name[200];
    char title[200];
    for (int i = 0; i < NVAR; i++) {
        sprintf(name, "HSTK_VAR-%d_%s", i + 1, NAME_X[i].c_str());
        s = new THStack(name, "");
        STK.push_back(s);

        sprintf(name, "CANVAS_VAR-%d_%s", i + 1, NAME_X[i].c_str());
        c = new TCanvas(name, "", 800, 600);
        CANVAS.push_back(c);

        l = new TLegend(0.7, 0.7, 0.95, 0.95, "");
        l->SetBorderSize(0);
        l->SetMargin(0.25);
        l->SetFillColor(0);
        l->SetFillStyle(0);
        LG.push_back(l);
    }
}

void Plot_Pool::Fill_Plots(int S_or_B, double weight) {
    double VAL_VAR;
    for (int i = 0; i < NVAR; i++) {
        VAL_VAR = Get_Value_Var(i);
        if (S_or_B == 1) {
            HIST_SIG[i]->Fill(VAL_VAR, weight);
            NORM_SIG[i] += weight;
        } else {
            HIST_BKG[i]->Fill(VAL_VAR, weight);
            NORM_BKG[i] += weight;
        }
    }
}

void Plot_Pool::Dump_Plots(const char *dirname, const char *bkgname, const char *signame, const bool NORM_TO_ONE) {
    Prepare_Pads();
    char tmp[500];
    sprintf(tmp, "%s/Plots%s.root", dirname, NORM_TO_ONE ? "_NORM" : "");
    TFile *fPlots = new TFile(tmp, "RECREATE");
    cout << "NORM to ONE: " << NORM_TO_ONE << endl;
    for (int i = 0; i < NVAR; i++) {
        cout << "For VAR: " << NAME_X[i] << endl;
        if (NORM_TO_ONE) HIST_BKG[i]->Scale(1.0 / NORM_BKG[i]);
        HIST_BKG[i]->GetYaxis()->SetTitle(NORM_TO_ONE ? "Arbitrary Unit" : "#sigma [fb/bin]");
        HIST_BKG[i]->SetLineColor(kGray + 2);
        fPlots->cd();
        HIST_BKG[i]->Write();
        STK[i]->Add((TH1 *)HIST_BKG[i]);
        LG[i]->AddEntry(HIST_BKG[i], bkgname, "F");

        if (NORM_TO_ONE) HIST_SIG[i]->Scale(1.0 / NORM_SIG[i]);
        HIST_SIG[i]->GetYaxis()->SetTitle(NORM_TO_ONE ? "Arbitrary Unit" : "#sigma [fb/bin]");
        HIST_SIG[i]->SetLineColor(kRed);
        fPlots->cd();
        HIST_SIG[i]->Write();
        STK[i]->Add((TH1 *)HIST_SIG[i]);
        LG[i]->AddEntry(HIST_SIG[i], signame, "F");

        CANVAS[i]->cd();
        STK[i]->Draw("nostack hist");
        STK[i]->GetYaxis()->SetTitle(NORM_TO_ONE ? "Arbitrary Unit" : "#sigma [fb/bin]");
        LG[i]->Draw("SAME");
        sprintf(tmp, "%s/C_VAR-%d_%s%s.png", dirname, i + 1, NAME_X[i].c_str(), NORM_TO_ONE ? "_NORM" : "");
        CANVAS[i]->SaveAs(tmp);
        fPlots->cd();
        CANVAS[i]->Write();
    }
    fPlots->Close();
}
