#include "AZH_Parameter.h"

#include <fstream>
#include <iostream>
#include <string>

#include "AZHPreAnalysis.h"
#include "TFile.h"
#include "TH2F.h"

using namespace std;

Distribution_Data::Distribution_Data(char *root_file) {
    TFile *f = new TFile(root_file);
    TTree *t = (TTree *)f->Get("AZHPreAnalysis");
    AZHPreAnalysis *del = new AZHPreAnalysis(t);
    int NBINX = 20;
    int NBINY = 20;
    double X_LOW = 350;
    double X_HIGH = 1000;
    double Y_LOW = 400;
    double Y_HIGH = 1000;
    TH2F *hist = new TH2F("hist", "", NBINX, X_LOW, X_HIGH, NBINY, Y_LOW, Y_HIGH);

    NBINS = NBINX * NBINY;
    for (int i = 0; i < t->GetEntries(); i++) {
        del->GetEntry(i);
        CS_WITHOUT_DECAY = del->CS_without_Decay;
        hist->Fill(del->mtt, del->mztt, del->Weight * 1000);
    }

    int nbinall;
    for (int i = 0; i < NBINX; i++) {
        for (int j = 0; j < NBINY; j++) {
            nbinall = hist->GetBin(i + 1, j + 1);
            HIST_BINS.push_back(hist->GetBinContent(nbinall));  // * lumi * mu_S;
        }
    }
    delete hist;
    delete del;
}

AZH_Parameter::AZH_Parameter(double MHA_IN, double MHH_IN, double WHA_IN, double WHH_IN, char *tri_file_name,
                             char *box_file_name, char *inter_file_name)
    : MHA(MHA_IN),
      MHH(MHH_IN),
      WHA(WHA_IN),
      WHH(WHH_IN),
      TRI_Data(tri_file_name),
      BOX_Data(box_file_name),
      INTER_Data(inter_file_name) {}

AZH_Grid::AZH_Grid(char const *data_dir, char const *param_id) {
    string WR_CHR[7] = {"0x005", "0x005", "0x010", "0x025", "0x050", "0x100", "0x250"};
    double WR[7] = {0, 0.005, 0.01, 0.025, 0.05, 0.1, 0.25};

    for (int i_wr_a = 0; i_wr_a < 7; i_wr_a++) {
        for (int i_wr_h = 0; i_wr_h < 7; i_wr_h++) {
            int pid = 0;
            for (double MHA = 500; MHA < 850; MHA += 50) {
                for (double MHH = 400; MHH < MHA - 100 + 50; MHH += 50) {
                    double WHA = MHA * WR[i_wr_a];
                    double WHH = MHH * WR[i_wr_h];
                    char tri_name[500];
                    char box_name[500];
                    char inter_name[500];
                    sprintf(tri_name, "%s/AZH_PreAna_gg_ztt_Type-I_tri_%s_HA%s_HH%s_%d_3l.root", data_dir, param_id,
                            WR_CHR[i_wr_a].c_str(), WR_CHR[i_wr_h].c_str(), pid);
                    sprintf(box_name, "%s/AZH_PreAna_gg_ztt_Type-I_box_%s_HA%s_HH%s_%d_3l.root", data_dir, param_id,
                            WR_CHR[i_wr_a].c_str(), WR_CHR[i_wr_h].c_str(), pid);
                    sprintf(inter_name, "%s/AZH_PreAna_gg_ztt_Type-I_inter_%s_HA%s_HH%s_%d_3l.root", data_dir, param_id,
                            WR_CHR[i_wr_a].c_str(), WR_CHR[i_wr_h].c_str(), pid);
                    Grid.emplace_back(MHA, MHH, WHA, WHH, tri_name, box_name, inter_name);
                    ++pid;
                }
            }
        }
    }
    NUM_POINTS = Grid.size();

    char tmp[500];
    sprintf(tmp, "%s/AZH_PreAna_gg_ztt_bkg_bkg_3l.root", data_dir);
    BKG = new Distribution_Data(tmp);
}

void AZH_Grid::Dump_Grid(char const *file_prefix) {
    char tmp[500];
    sprintf(tmp, "%s_tri_distribution.txt", file_prefix);
    ofstream tri_file(tmp);
    sprintf(tmp, "%s_box_distribution.txt", file_prefix);
    ofstream box_file(tmp);
    sprintf(tmp, "%s_inter_distribution.txt", file_prefix);
    ofstream inter_file(tmp);
    sprintf(tmp, "%s_bkg_distribution.txt", file_prefix);
    ofstream bkg_file(tmp);

    tri_file << "MHA\tMHH\tWHA\tWHH\ttb\tcba\tCS";
    box_file << "MHA\tMHH\tWHA\tWHH\ttb\tcba\tCS";
    inter_file << "MHA\tMHH\tWHA\tWHH\ttb\tcba\tCS";
    bkg_file << "CS";
    for (int i = 0; i < Grid[0].TRI_Data.NBINS; i++) {
        tri_file << "\tBIN" << i;
        box_file << "\tBIN" << i;
        inter_file << "\tBIN" << i;
        bkg_file << "\tBIN" << i;
    }
    tri_file << endl;
    box_file << endl;
    inter_file << endl;
    bkg_file << endl;
    for (int i = 0; i < NUM_POINTS; i++) {
        tri_file << "\t" << Grid[i].MHA << "\t" << Grid[i].MHH << "\t" << Grid[i].WHA << "\t" << Grid[i].WHH
                 << "\t1\t0\t" << Grid[i].TRI_Data.CS_WITHOUT_DECAY;
        box_file << "\t" << Grid[i].MHA << "\t" << Grid[i].MHH << "\t" << Grid[i].WHA << "\t" << Grid[i].WHH
                 << "\t1\t0\t" << Grid[i].BOX_Data.CS_WITHOUT_DECAY;
        inter_file << "\t" << Grid[i].MHA << "\t" << Grid[i].MHH << "\t" << Grid[i].WHA << "\t" << Grid[i].WHH
                   << "\t1\t0\t" << Grid[i].INTER_Data.CS_WITHOUT_DECAY;
        for (int j = 0; j < Grid[i].TRI_Data.NBINS; j++) {
            tri_file << "\t" << Grid[i].TRI_Data.HIST_BINS[j];
            box_file << "\t" << Grid[i].BOX_Data.HIST_BINS[j];
            inter_file << "\t" << Grid[i].INTER_Data.HIST_BINS[j];
        }
        tri_file << endl;
        box_file << endl;
        inter_file << endl;
    }

    bkg_file << BKG->CS_WITHOUT_DECAY;
    for (int i = 0; i < BKG->NBINS; i++) {
        bkg_file << "\t" << BKG->HIST_BINS[i];
    }
    bkg_file << endl;
}
