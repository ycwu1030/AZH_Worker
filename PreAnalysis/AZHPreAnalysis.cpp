#include <iostream>
#include <string>

#include "AZHSystem.h"
#include "Delphes.h"

using namespace std;

int main(int argc, char const *argv[]) {
    // * Input arguments:
    // * 0: exe name
    // * 1: process name;
    // * 2: process name abbr;
    // * 3: SorB: 0 for Bkg, 1 for Signal Total, 2 for Signal Triangle, 3 for Signal Box, 4 for Signal Interferences
    // * 4: Decay ID: 0 for 3l, 1 for 4l;
    // * 5: parameter point tag: parameter tag for signal, 'bkg' for Bkg;
    // * 6: Output file name;
    // * 7: CS: the cross section for current process, without decay of Z/t
    // * 8 - n: input root files for current process
    if (argc < 9) {
        cout << "Usage: " << argv[0]
             << " <proc_full_name> <proc_abbr_name> <proc_cate_id> <proc_decay_id> <proc_param_name> "
                "<output_file_name> <proc_cs> <proc_root_files...>"
             << endl;
        return -1;
    }

    char proc_full_name[200];
    sprintf(proc_full_name, "%s", argv[1]);
    char proc_abbr_name[20];
    sprintf(proc_abbr_name, "%s", argv[2]);
    int proc_cate_id = atoi(argv[3]);
    char proc_cate_name[20];
    switch (proc_cate_id) {
        case 0:
            sprintf(proc_cate_name, "%s", "bkg");
            break;
        case 1:
            sprintf(proc_cate_name, "%s", "sig_total");
            break;
        case 2:
            sprintf(proc_cate_name, "%s", "sig_triangle");
            break;
        case 3:
            sprintf(proc_cate_name, "%s", "sig_box");
            break;
        case 4:
            sprintf(proc_cate_name, "%s", "sig_interference");
            break;
        default:
            cout << "Wrong Process Category ID, must be 0 to 4, but " << proc_cate_id << " is provided." << endl;
            return -1;
    }
    if (proc_cate_id == 0) {
        sprintf(proc_cate_name, "%s", "bkg");
    } else {
        sprintf(proc_cate_name, "%s", "sig");
    }
    int proc_decay_id = atoi(argv[4]);
    char proc_decay_name[10];
    if (proc_decay_id == 0) {
        sprintf(proc_decay_name, "%s", "3l");
    } else {
        sprintf(proc_decay_name, "%s", "4l");
    }
    char proc_param_name[100];
    sprintf(proc_param_name, "%s", argv[5]);
    char proc_output_name[500];
    sprintf(proc_output_name, "%s", argv[6]);
    double proc_cs = atof(argv[7]);
    const double BRZll = 0.067294;
    const double BRt[2] = {2 * 0.2134 * 0.6741, 0.2134 * 0.2134};
    double proc_cs_with_decay = proc_cs * BRZll * BRt[proc_decay_id];
    TChain *ch = new TChain("Delphes");
    for (int i = 8; i < argc; i++) {
        ch->Add(argv[i]);
    }
    Delphes del(ch);
    int NEVENTS = ch->GetEntries();
    double weight = proc_cs_with_decay / (double(NEVENTS));

    TFile *fout = new TFile(proc_output_name, "RECREATE");
    TTree *tout = new TTree("AZHPreAnalysis", "Pre-Analysis for AZH System");

    // * Process Information
    tout->Branch("Proc_Full_Name", proc_full_name, "Proc_Full_Name/C");
    tout->Branch("Proc_Abbr_Name", proc_abbr_name, "Proc_Abbr_Name/C");
    tout->Branch("Proc_Cate_ID", &proc_cate_id, "Proc_Cate_ID/I");
    tout->Branch("Proc_Cate_Name", proc_cate_name, "Proc_Cate_Name/C");
    tout->Branch("Decay_ID", &proc_decay_id, "Decay_ID/I");
    tout->Branch("Decay_Name", proc_decay_name, "Decay_Name/C");
    tout->Branch("CS_without_Decay", &proc_cs, "CS_without_Decay/D");
    tout->Branch("CS_with_Decay", &proc_cs_with_decay, "CS_with_Decay/D");
    tout->Branch("Proc_Param_Name", &proc_param_name, "Proc_Param_Name/C");
    tout->Branch("N_EVENTS", &NEVENTS, "N_EVENTS/I");
    // tout->Branch("Weight", &weight, "Weight/D");

    // * AZH System Information
    AZHSystem AZH;
    AZH.Setup_Branches(tout);

    for (int ne = 0; ne < NEVENTS; ne++) {
        if ((ne + 1) % 1000 == 0) {
            cout << "Events: " << ne + 1 << "\r";
            cout.flush();
        }
        del.GetEntry(ne);
        bool good = AZH.Setup(&del, weight);
        if (!good) continue;
        tout->Fill();
    }
    fout->cd();
    tout->Write();
    fout->Close();
    return 0;
}
