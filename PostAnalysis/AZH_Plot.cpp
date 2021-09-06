#include <iostream>
#include <string>

#include "AZHPreAnalysis.h"
#include "Plot_Pool.h"
#include "Utilities.h"

using namespace std;

int main(int argc, char const *argv[]) {
    SetPlotStyle();

    //* 0: exe name
    //* 1: output dir
    //* 2-n: input root files

    string output_dir(argv[1]);
    TChain *ch = new TChain("AZHPreAnalysis");
    for (int i = 2; i < argc; i++) {
        ch->Add(argv[i]);
    }
    AZHPreAnalysis *AZHch = new AZHPreAnalysis(ch);
    Plot_Pool PP;
    PP.Add(2, 0, 2, "CATEGORY", "S or B", &(AZHch->Proc_Cate_ID));
    PP.Add(50, 350, 1000, "MTT", "m_{t#bar t} [GeV]", &(AZHch->mtt));
    PP.Add(50, 450, 1000, "MZTT", "m_{zt#bar t} [GeV]", &(AZHch->mztt));
    PP.Prepare_Plots();
    cout << "#Plots: " << PP.Get_N_VAR() << endl;
    for (int ne = 0; ne < ch->GetEntries(); ne++) {
        if ((ne + 1) % 1000 == 0) {
            cout << "Events: " << ne + 1 << "\r";
            cout.flush();
        }
        AZHch->GetEntry(ne);
        PP.Fill_Plots(AZHch->Proc_Cate_ID, AZHch->Weight * 1000);
    }
    PP.Dump_Plots(output_dir.c_str(), "ztt", "sig", false);
    PP.Dump_Plots(output_dir.c_str(), "ztt", "sig", true);

    return 0;
}
