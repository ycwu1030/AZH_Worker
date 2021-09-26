#include "AZH_Parameter.h"

#include <fstream>
#include <iostream>
#include <string>

#include "AZHPreAnalysis.h"
#include "TFile.h"
#include "TH2F.h"

using namespace std;

AZH_Parameter::AZH_Parameter(double MHA_IN, double MHH_IN, double WHA_IN, double WHH_IN, char *tri_file_name,
                             char *box_file_name, char *inter_file_name)
    : MHA(MHA_IN), MHH(MHH_IN), WHA(WHA_IN), WHH(WHH_IN) {
    TFile *f_tri = new TFile(tri_file_name);
    TFile *f_box = new TFile(box_file_name);
    TFile *f_inter = new TFile(tri_file_name);

    TTree *t_tri = (TTree *)f_tri->Get("AZHPreAnalysis");
    TTree *t_box = (TTree *)f_box->Get("AZHPreAnalysis");
    TTree *t_inter = (TTree *)f_inter->Get("AZHPreAnalysis");
    AZHPreAnalysis *del_tri = new AZHPreAnalysis(t_tri);
    AZHPreAnalysis *del_box = new AZHPreAnalysis(t_box);
    AZHPreAnalysis *del_inter = new AZHPreAnalysis(t_inter);

    int NBINX = 20;
    int NBINY = 20;
    double X_LOW = 350;
    double X_HIGH = 1000;
    double Y_LOW = 400;
    double Y_HIGH = 1000;
    TH2F *hist_tri = new TH2F("hist_tri", "", NBINX, X_LOW, X_HIGH, NBINY, Y_LOW, Y_HIGH);
    TH2F *hist_box = new TH2F("hist_box", "", NBINX, X_LOW, X_HIGH, NBINY, Y_LOW, Y_HIGH);
    TH2F *hist_inter = new TH2F("hist_inter", "", NBINX, X_LOW, X_HIGH, NBINY, Y_LOW, Y_HIGH);

    NBINS = NBINX * NBINY;
    for (int i = 0; i < t_tri->GetEntries(); i++) {
        del_tri->GetEntry(i);
        hist_tri->Fill(del_tri->mtt, del_tri->mztt, del_tri->Weight * 1000);
    }
    for (int i = 0; i < t_box->GetEntries(); i++) {
        del_box->GetEntry(i);
        hist_box->Fill(del_box->mtt, del_box->mztt, del_box->Weight * 1000);
    }
    for (int i = 0; i < t_inter->GetEntries(); i++) {
        del_inter->GetEntry(i);
        hist_inter->Fill(del_inter->mtt, del_inter->mztt, del_inter->Weight * 1000);
    }
    int nbinall;
    for (int i = 0; i < NBINX; i++) {
        for (int j = 0; j < NBINY; j++) {
            nbinall = hist_tri->GetBin(i + 1, j + 1);
            HIST_TRI.push_back(hist_tri->GetBinContent(nbinall));  // * lumi * mu_S;
            nbinall = hist_box->GetBin(i + 1, j + 1);
            HIST_BOX.push_back(hist_box->GetBinContent(nbinall));  // * lumi * mu_S;
            nbinall = hist_inter->GetBin(i + 1, j + 1);
            HIST_INTER.push_back(hist_inter->GetBinContent(nbinall));  // * lumi * mu_S;
        }
    }
    delete hist_tri;
    delete hist_box;
    delete hist_inter;
    delete del_tri;
    delete del_box;
    delete del_inter;
}

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
}

void AZH_Grid::Dump_Grid(char const *file_prefix) {
    char tmp[500];
    sprintf(tmp, "%s_tri_distribution.txt", file_prefix);
    ofstream tri_file(tmp);
    sprintf(tmp, "%s_box_distribution.txt", file_prefix);
    ofstream box_file(tmp);
    sprintf(tmp, "%s_inter_distribution.txt", file_prefix);
    ofstream inter_file(tmp);

    tri_file << "MHA\tMHH\tWHA\tWHH\ttb\tcba";
    box_file << "MHA\tMHH\tWHA\tWHH\ttb\tcba";
    inter_file << "MHA\tMHH\tWHA\tWHH\ttb\tcba";
    for (int i = 0; i < Grid[0].NBINS; i++) {
        tri_file << "\tBIN" << i;
        box_file << "\tBIN" << i;
        inter_file << "\tBIN" << i;
    }
    tri_file << endl;
    box_file << endl;
    inter_file << endl;
    for (int i = 0; i < NUM_POINTS; i++) {
        tri_file << "\t" << Grid[i].MHA << "\t" << Grid[i].MHH << "\t" << Grid[i].WHA << "\t" << Grid[i].WHH
                 << "\t1\t0";
        box_file << "\t" << Grid[i].MHA << "\t" << Grid[i].MHH << "\t" << Grid[i].WHA << "\t" << Grid[i].WHH
                 << "\t1\t0";
        inter_file << "\t" << Grid[i].MHA << "\t" << Grid[i].MHH << "\t" << Grid[i].WHA << "\t" << Grid[i].WHH
                   << "\t1\t0";
        for (int j = 0; j < Grid[i].NBINS; j++) {
            tri_file << "\t" << Grid[i].HIST_TRI[j];
            box_file << "\t" << Grid[i].HIST_BOX[j];
            inter_file << "\t" << Grid[i].HIST_INTER[j];
        }
        tri_file << endl;
        box_file << endl;
        inter_file << endl;
    }
}
