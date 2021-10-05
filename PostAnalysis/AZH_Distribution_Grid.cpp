#include <fstream>
#include <iostream>
#include <string>

#include "AZHPreAnalysis.h"
#include "AZH_Parameter.h"
#include "TFile.h"
#include "TTree.h"
#include "Utilities.h"

using namespace std;

int main(int argc, char const *argv[]) {
    //* 0: exe name
    //* 1: distribution dir
    //* 2: root file of the parameter points
    //* 3: output
    //* 4: A->ZH or H->ZA: 0 for A->ZH, 1 for H->ZA

    // char data_dir[200];
    // char param_id[200];
    // char output_prefix[200];
    // sprintf(data_dir,"%s",argv[1]);
    // sprintf(data_d)
    bool FLIP = atoi(argv[4]);
    AZH_Grid grid(argv[1], FLIP);

    TFile *f1 = new TFile(argv[2]);
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
    ofstream output(argv[3]);
    if (FLIP) {
        output << "LID\tHZAtt_NLL" << endl;
    } else {
        output << "LID\tAZHtt_NLL" << endl;
    }
    for (int i = 0; i < t1->GetEntries(); i++) {
        if ((i + 1) % 1000 == 0) {
            cout << "Entry: " << i + 1 << endl;
        }
        t1->GetEntry(i);
        // cout << "Entry: " << i << endl;
        double nll = grid.Calculate_NLL(MHA, MHH, WHA, WHH, tbeta, alpha);
        output << LID << "\t" << nll << endl;
    }

    return 0;
}
