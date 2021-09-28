#include "AZH_Parameter.h"

#include <fstream>
#include <iostream>
#include <string>

#include "AZHPreAnalysis.h"
#include "TFile.h"
#include "TH2F.h"
#include "Utilities.h"

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

Distribution_Data::Distribution_Data() : CS_WITHOUT_DECAY(0) {}

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
    // string WR_CHR[7] = {"0x005", "0x005", "0x010", "0x025", "0x050", "0x100", "0x250"};
    // double WR[7] = {0, 0.005, 0.01, 0.025, 0.05, 0.1, 0.25};
    WR_CHR[0] = "0x005";  // Use 0x005 distribution for 0x000 case, extrapolation
    WR[0] = 0;

    WR_CHR[1] = "0x005";
    WR[1] = 0.005;

    WR_CHR[2] = "0x010";
    WR[2] = 0.01;

    WR_CHR[3] = "0x025";
    WR[3] = 0.025;

    WR_CHR[4] = "0x050";
    WR[4] = 0.05;

    WR_CHR[5] = "0x100";
    WR[5] = 0.1;

    WR_CHR[6] = "0x250";
    WR[6] = 0.25;

    for (int i_wr_a = 0; i_wr_a < 7; i_wr_a++) {
        for (int i_wr_h = 0; i_wr_h < 7; i_wr_h++) {
            int pid = 0;
            for (int i_MHA = 0; i_MHA < 7; i_MHA++) {
                double MHA = 500 + i_MHA * 50;
                double WHA = MHA * WR[i_wr_a];
                for (int i_MHH = 0; i_MHH < i_MHA + 1; i_MHH++) {
                    double MHH = 400 + i_MHH * 50;
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
                    cout << "Construct node for " << MHA << " " << MHH << " " << WHA << " " << WHH << endl;
                    Grid[i_wr_a][i_wr_h][i_MHA][i_MHH] =
                        new AZH_Parameter(MHA, MHH, WHA, WHH, tri_name, box_name, inter_name);
                    ++pid;
                }
            }
        }
    }
    // NUM_POINTS = Grid.size();

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
    for (int i = 0; i < (Grid[0][0][0][0]->TRI_Data).NBINS; i++) {
        tri_file << "\tBIN" << i;
        box_file << "\tBIN" << i;
        inter_file << "\tBIN" << i;
        bkg_file << "\tBIN" << i;
    }
    tri_file << "\tNLL" << endl;
    box_file << "\tNLL" << endl;
    inter_file << "\tNLL" << endl;
    bkg_file << endl;
    for (int i_wr_a = 0; i_wr_a < 7; i_wr_a++) {
        for (int i_wr_h = 0; i_wr_h < 7; i_wr_h++) {
            for (int i_MHA = 0; i_MHA < 7; i_MHA++) {
                for (int i_MHH = 0; i_MHH < i_MHA + 1; i_MHH++) {
                    AZH_Parameter *ptr = Grid[i_wr_a][i_wr_h][i_MHA][i_MHH];
                    tri_file << ptr->MHA << "\t" << ptr->MHH << "\t" << ptr->WHA << "\t" << ptr->WHH << "\t1\t0\t"
                             << (ptr->TRI_Data).CS_WITHOUT_DECAY;
                    box_file << ptr->MHA << "\t" << ptr->MHH << "\t" << ptr->WHA << "\t" << ptr->WHH << "\t1\t0\t"
                             << (ptr->BOX_Data).CS_WITHOUT_DECAY;
                    inter_file << ptr->MHA << "\t" << ptr->MHH << "\t" << ptr->WHA << "\t" << ptr->WHH << "\t1\t0\t"
                               << (ptr->INTER_Data).CS_WITHOUT_DECAY;
                    double nll_total = 0.0;
                    for (int j = 0; j < (ptr->TRI_Data).NBINS; j++) {
                        double c_tri = (ptr->TRI_Data).HIST_BINS[j];
                        double c_box = (ptr->BOX_Data).HIST_BINS[j];
                        double c_inter = (ptr->INTER_Data).HIST_BINS[j];
                        tri_file << "\t" << c_tri;
                        box_file << "\t" << c_box;
                        inter_file << "\t" << c_inter;
                        nll_total += 3000 * NLL(c_tri + c_box + c_inter + BKG->HIST_BINS[j], BKG->HIST_BINS[j]);
                    }
                    tri_file << "\t" << nll_total << endl;
                    box_file << "\t" << nll_total << endl;
                    inter_file << "\t" << nll_total << endl;
                }
            }
        }
    }

    bkg_file << BKG->CS_WITHOUT_DECAY;
    for (int i = 0; i < BKG->NBINS; i++) {
        bkg_file << "\t" << BKG->HIST_BINS[i];
    }
    bkg_file << endl;
}

AZH_Grid::~AZH_Grid() {
    for (int i_wr_a = 0; i_wr_a < 7; i_wr_a++) {
        for (int i_wr_h = 0; i_wr_h < 7; i_wr_h++) {
            for (int i_MHA = 0; i_MHA < 7; i_MHA++) {
                for (int i_MHH = 0; i_MHH < i_MHA + 1; i_MHH++) {
                    delete Grid[i_wr_a][i_wr_h][i_MHA][i_MHH];
                    Grid[i_wr_a][i_wr_h][i_MHA][i_MHH] = nullptr;
                }
            }
        }
    }
}

int AZH_Grid::Get_Width_Index(double wr) {
    if (wr < WR[0] || wr >= WR[6]) {
        return -1;
    }
    int id_low = 0;
    int id_high = 6;
    int id_mid;
    while (id_high - id_low > 1) {
        id_mid = (id_high + id_low) >> 1;
        if (wr >= WR[id_mid]) {
            id_low = id_mid;
        } else {
            id_high = id_mid;
        }
    }
    if (wr == WR[0]) {
        return 0;
    } else {
        return id_low;
    }
}

void AZH_Grid::Get_Mass_Index(double mha, double mhh, int &id_a, int &id_h) {
    if (mha < 500 || mha > 800 || mhh < 400 || mhh > 700 || mhh > mha - 100) {
        id_a = -1;
        id_h = -1;
        return;
    }
    id_a = static_cast<int>((mha - 499) / 50);
    id_h = static_cast<int>((mhh - 399) / 50);
}

Point::Point() : x(0), y(0) {}

Triangle::Triangle(Point x1, Point x2, Point x3) : P1(x1), P2(x2), P3(x3) {}

void Triangle::Get_Barycentric_Coordinate(const Point &P, double &l1, double &l2, double &l3) {
    double Area_2 = P1.x * (P2.y - P3.y) + P2.x * (P3.y - P1.y) + P3.x * (P1.y - P2.y);
    l1 = ((P2.y - P3.y) * (P.x - P3.x) + (P3.x - P2.x) * (P.y - P3.y)) / Area_2;
    l2 = ((P3.y - P1.y) * (P.x - P3.x) + (P1.x - P3.x) * (P.y - P3.y)) / Area_2;
    l3 = 1.0 - l1 - l2;
}
