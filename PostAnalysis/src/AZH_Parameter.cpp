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

AZH_Grid::AZH_Grid()
    : MHA_MIN(500), MHA_MAX(800), MHA_STEP(50), MHH_MIN(400), MHH_MAX(700), MHH_STEP(50), NEED_TO_DELETE(false) {
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
}

AZH_Grid::AZH_Grid(char const *data_dir, char const *param_id)
    : MHA_MIN(500), MHA_MAX(800), MHA_STEP(50), MHH_MIN(400), MHH_MAX(700), MHH_STEP(50), NEED_TO_DELETE(false) {
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

    Read_Data(data_dir, param_id);
}

void AZH_Grid::Read_Data(char const *data_dir, char const *param_id) {
    for (int i_wr_a = 0; i_wr_a < 7; i_wr_a++) {
        for (int i_wr_h = 0; i_wr_h < 7; i_wr_h++) {
            int pid = 0;
            for (int i_MHA = 0; i_MHA < 7; i_MHA++) {
                double MHA = MHA_MIN + i_MHA * MHA_STEP;
                double WHA = MHA * WR[i_wr_a];
                for (int i_MHH = 0; i_MHH < i_MHA + 1; i_MHH++) {
                    double MHH = MHH_MIN + i_MHH * MHH_STEP;
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
    NEED_TO_DELETE = true;
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
    if (NEED_TO_DELETE) {
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
        delete BKG;
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
    if (mha < MHA_MIN || mha >= MHA_MAX || mhh < MHH_MIN || mhh >= MHH_MAX || mhh > mha - 100) {
        id_a = -1;
        id_h = -1;
        return;
    }
    id_a = static_cast<int>((mha - MHA_MIN + 1) / MHA_STEP);
    id_h = static_cast<int>((mhh - MHH_MIN + 1) / MHH_STEP);
}

bool AZH_Grid::Get_Mass_Triangle(double mha, double mhh, Triangle &t) {
    int id_mha_low;
    int id_mhh_low;
    Get_Mass_Index(mha, mhh, id_mha_low, id_mhh_low);
    if (id_mha_low < 0 || id_mhh_low < 0) return false;
    /*
     *  The naming convention is
     *  D----C
     *  |    |
     *  A----B
     */
    Point A = {Get_MHA(id_mha_low), Get_MHH(id_mhh_low), id_mha_low, id_mhh_low};
    Point C = {Get_MHA(id_mha_low + 1), Get_MHH(id_mhh_low + 1), id_mha_low + 1, id_mhh_low + 1};
    double slope = (C.y - A.y) / (C.x - A.x);
    double slope_test = (mhh - A.y) / (mha - A.x);
    if (slope_test > slope) {
        t.P1 = A;
        t.P2 = C;
        t.P3.x = A.x;
        t.P3.y = C.y;
        t.P3.id_x = A.id_x;
        t.P3.id_y = C.id_y;
        return true;
    } else {
        t.P1 = A;
        t.P2 = C;
        t.P3.x = C.x;
        t.P3.y = A.y;
        t.P3.id_x = C.id_x;
        t.P3.id_y = A.id_y;
        return true;
    }
}

bool AZH_Grid::Get_Width_Triangle(double wra, double wrh, Triangle &t) {
    int id_wra_low = Get_Width_Index(wra);
    if (id_wra_low < 0) return false;
    int id_wrh_low = Get_Width_Index(wrh);
    if (id_wrh_low < 0) return false;
    /*
     *  The naming convention is
     *  D----C
     *  |    |
     *  A----B
     */
    Point A = {WR[id_wra_low], WR[id_wrh_low], id_wra_low, id_wrh_low};
    Point C = {WR[id_wra_low + 1], WR[id_wrh_low + 1], id_wra_low + 1, id_wrh_low + 1};
    double slope = (C.y - A.y) / (C.x - A.x);
    double slope_test = (wrh - A.y) / (wra - A.x);
    if (slope_test > slope) {
        t.P1 = A;
        t.P2 = C;
        t.P3.x = A.x;
        t.P3.y = C.y;
        t.P3.id_x = A.id_x;
        t.P3.id_y = C.id_y;
        return true;
    } else {
        t.P1 = A;
        t.P2 = C;
        t.P3.x = C.x;
        t.P3.y = A.y;
        t.P3.id_x = C.id_x;
        t.P3.id_y = A.id_y;
        return true;
    }
}

struct __INDEX__ {
    int id_MHA;
    int id_MHH;
    int id_WRA;
    int id_WRH;
};

bool AZH_Grid::Get_Signal_Distribution(double mha, double mhh, double wha, double whh, double tb, double alpha,
                                       Distribution_Data &dd) {
    double wra = wha / mha;
    double wrh = whh / mhh;
    Triangle t_MASS;
    Triangle t_WIDTH;
    bool good;
    good = Get_Mass_Triangle(mha, mhh, t_MASS);
    if (!good) return false;
    good = Get_Width_Triangle(wra, wrh, t_WIDTH);
    if (!good) return false;
    double lam_m[3];
    t_MASS.Get_Barycentric_Coordinate({mha, mhh, 0, 0}, lam_m);
    double lam_w[3];
    t_WIDTH.Get_Barycentric_Coordinate({wra, wrh, 0, 0}, lam_w);

    double beta = atan(tb);
    double kV = sin(beta - alpha);
    double ktA = 1 / tb;
    double ktH = sin(alpha) / sin(beta);

    double scale_tri = pow(ktA * ktH * kV, 2);
    double scale_box = pow(ktH, 4);
    double scale_int = pow(ktH, 3) * ktA * kV;

    __INDEX__ vertices[3][3];
    for (int id_t_width = 0; id_t_width < 3; id_t_width++) {
        for (int id_t_mass = 0; id_t_mass < 3; id_t_mass++) {
            vertices[id_t_width][id_t_mass].id_MHA = t_MASS.P(id_t_mass).id_x;
            vertices[id_t_width][id_t_mass].id_MHH = t_MASS.P(id_t_mass).id_y;
            vertices[id_t_width][id_t_mass].id_WRA = t_WIDTH.P(id_t_width).id_x;
            vertices[id_t_width][id_t_mass].id_WRH = t_WIDTH.P(id_t_width).id_y;
        }
    }
    dd.HIST_BINS = vector<double>(400);
    for (int ibin = 0; ibin < 400; ibin++) {
        dd.HIST_BINS[ibin] = 0;
        for (int id_t_width = 0; id_t_width < 3; id_t_width++) {
            for (int id_t_mass = 0; id_t_mass < 3; id_t_mass++) {
                __INDEX__ vtx = vertices[id_t_width][id_t_mass];
                AZH_Parameter *ptr = Grid[vtx.id_WRA][vtx.id_WRH][vtx.id_MHA][vtx.id_MHH];
                dd.HIST_BINS[ibin] +=
                    scale_tri * lam_w[id_t_width] * lam_m[id_t_mass] * ((ptr->TRI_Data).HIST_BINS[ibin]);
                dd.HIST_BINS[ibin] +=
                    scale_box * lam_w[id_t_width] * lam_m[id_t_mass] * ((ptr->BOX_Data).HIST_BINS[ibin]);
                dd.HIST_BINS[ibin] +=
                    scale_int * lam_w[id_t_width] * lam_m[id_t_mass] * ((ptr->INTER_Data).HIST_BINS[ibin]);
            }
        }
    }
    return true;
}

double AZH_Grid::Calculate_NLL(double mha, double mhh, double wha, double whh, double tb, double alpha) {
    Distribution_Data signal;
    bool good = Get_Signal_Distribution(mha, mhh, wha, whh, tb, alpha, signal);
    if (!good) return 0;
    double nll = 0;
    for (int i = 0; i < BKG->NBINS; i++) {
        nll += NLL(signal.HIST_BINS[i] + BKG->HIST_BINS[i], BKG->HIST_BINS[i]);
    }
    return nll;
}

Triangle::Triangle(Point x1, Point x2, Point x3) : P1(x1), P2(x2), P3(x3) {}

Point &Triangle::P(int i) {
    if (i == 0) return P1;
    if (i == 1) return P2;
    if (i == 2) return P3;
}

void Triangle::Get_Barycentric_Coordinate(const Point &P, double *lams) {
    double Area_2 = P1.x * (P2.y - P3.y) + P2.x * (P3.y - P1.y) + P3.x * (P1.y - P2.y);
    lams[0] = ((P2.y - P3.y) * (P.x - P3.x) + (P3.x - P2.x) * (P.y - P3.y)) / Area_2;
    lams[1] = ((P3.y - P1.y) * (P.x - P3.x) + (P1.x - P3.x) * (P.y - P3.y)) / Area_2;
    lams[2] = 1.0 - lams[0] - lams[1];
}
