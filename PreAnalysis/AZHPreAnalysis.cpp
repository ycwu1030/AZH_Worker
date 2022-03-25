#include <iostream>
#include <string>

#include "AZHSystem.h"
#include "Delphes.h"

using namespace std;

string proc_names[14] = {"Bkg",  "ggF_TRI", "ggF_BOX", "ggF_Z",  "ggF_TRIxBOX", "ggF_TRIxZ", "ggF_BOXxZ",
                         "bb_Z", "bb_S",    "bb_T",    "bb_ZxS", "bb_ZxT",      "bb_SxT",    "qq_DY"};

int main(int argc, char const *argv[]) {
    // * 0: exe name
    // * 1: process id
    // * 2: parameter point tag
    // * 3: cross section without decay
    // * 4: output file name
    // * 5-n: input root files for current process with corresponding parameter
    if (argc < 6) {
        cout << "Usage: " << argv[0] << " <proc_id> <parameter_tag> <cs> <out_name> <input_root_files...>" << endl;
        return -1;
    }
    int proc_id = atoi(argv[1]);
    if (proc_id < 0 || proc_id > 13) return -1;
    string proc_name = proc_names[proc_id];
    char proc_abbr_name[200];
    sprintf(proc_abbr_name, "%s", proc_name.c_str());
    char proc_param_name[200];
    sprintf(proc_param_name, "%s", argv[2]);
    double proc_cs = atof(argv[3]);
    char proc_output_name[200];
    sprintf(proc_output_name, "%s", argv[4]);
    const double BRZll = 0.067294;
    const double BRt = 2 * 0.2134 * 0.6741;
    double proc_cs_with_decay = proc_cs * BRZll * BRt;
    TChain *ch = new TChain("Delphes");
    for (int i = 5; i < argc; i++) {
        ch->Add(argv[i]);
    }
    Delphes del(ch);
    int NEVENTS = ch->GetEntries();
    double weight = 0;
    int NEVENTS_NEGATIVE_WEIGHT = 0;
    int NEVENTS_POSITIVE_WEIGHT = 0;

    for (int ne = 0; ne < NEVENTS; ne++) {
        del.GetEntry(ne);
        weight = del.Event_Weight[0];
        if (weight < 0)
            NEVENTS_NEGATIVE_WEIGHT++;
        else
            NEVENTS_POSITIVE_WEIGHT++;
    }
    double abs_weight = abs(proc_cs_with_decay / double(NEVENTS_POSITIVE_WEIGHT - NEVENTS_NEGATIVE_WEIGHT));

    TFile *fout = new TFile(proc_output_name, "RECREATE");
    TTree *tout = new TTree("AZHPreAnalysis", "Pre-Analysis for AZH System");

    tout->Branch("proc_name", proc_abbr_name, "proc_name/C");
    tout->Branch("proc_id", &proc_id, "proc_id/I");
    tout->Branch("cs_wo_decay", &proc_cs, "cs_wo_decay/D");
    tout->Branch("cs_w_decay", &proc_cs_with_decay, "cs_w_decay/D");
    tout->Branch("param_tag", proc_param_name, "param_tag/C");
    tout->Branch("n_total", &NEVENTS, "n_total/I");
    tout->Branch("n_total_negative_weight", &NEVENTS_NEGATIVE_WEIGHT, "n_total_negative_weight/I");
    tout->Branch("n_total_positive_weight", &NEVENTS_POSITIVE_WEIGHT, "n_total_positive_weight/I");
    tout->Branch("weight", &weight, "weight/D");

    AZHSystem AZH;
    AZH.Setup_Output_Branches(tout);

    double cs_total = 0;
    for (int ne = 0; ne < NEVENTS; ne++) {
        if ((ne + 1) % 1000 == 0) {
            cout << "Events: " << ne + 1 << "\r";
            cout.flush();
        }
        del.GetEntry(ne);
        weight = del.Event_Weight[0];
        if (weight < 0)
            weight = -abs_weight;
        else
            weight = abs_weight;
        cs_total += weight;
        bool good = AZH.Setup(&del);
        if (!good) continue;
        tout->Fill();
    }
    cout << "Input CS: " << proc_cs_with_decay << " Used CS: " << cs_total << endl;
    fout->cd();
    tout->Write();
    fout->Close();
    return 0;
}
