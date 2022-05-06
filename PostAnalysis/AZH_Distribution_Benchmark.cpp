
#include <fstream>
#include <iostream>
#include <string>

#include "AZHGrid.h"
#include "AZHNLL.h"
#include "PlotStyle.h"
#include "TCanvas.h"
#include "TH2F.h"
#include "TLatex.h"

using namespace std;

int main(int argc, char const *argv[]) {
    // Distribution_t dist_bkg("../DATADIR/Distribution/BKG/COMB/AZH_Distribution_bkg_total_BKG_COMB_DEFAULT_3l.txt",
    // 0);
    SetPlotStyle(0.2);
    double MHA = 750;
    double MHH = 600;
    double width_benchmark[2] = {0.01, 0.1};
    string width_strings[2] = {"0x010", "0x100"};
    char tmp[200];
    TH2F *h2[2];
    for (int iw = 0; iw < 2; iw++) {
        double rw = width_benchmark[iw];
        double wha = rw * MHA;
        double whh = rw * MHH;
        AZHPoint p_sig(MHA, MHH, wha, whh, 1.0, 0.1);
        cout << "Point obtained" << endl;
        Distribution_t &dist_sig = p_sig.get_distribution();
        cout << "Distribution obtained" << endl;
        sprintf(tmp, "hist_%s", width_strings[iw].c_str());
        h2[iw] = new TH2F(tmp, "", 20, 350, 1000, 20, 400, 1000);
        int ib = 0;
        double max = 0;
        for (int ix = 0; ix < 20; ix++) {
            for (int iy = 0; iy < 20; iy++) {
                int nbinall = h2[iw]->GetBin(ix + 1, iy + 1);
                h2[iw]->SetBinContent(nbinall, dist_sig.data_cs[ib]);
                if (dist_sig.data_cs[ib] > max) max = dist_sig.data_cs[ib];
                ib++;
            }
        }
        // dist_sig.set_histogram(tmp, h2[iw]);
        cout << "Histogram obtained" << endl;
        cout << h2[iw]->GetNbinsX() << " x " << h2[iw]->GetNbinsY();
        sprintf(tmp, "c_%s", width_strings[iw].c_str());
        TCanvas *c1 = new TCanvas(tmp, "", 800, 600);
        cout << "Setup canvas" << endl;
        c1->cd();
        h2[iw]->SetXTitle("m_{tt} [GeV]");
        h2[iw]->SetYTitle("m_{Ztt} [GeV]");
        h2[iw]->SetZTitle("#sigma/bin [fb]");
        h2[iw]->GetZaxis()->SetRangeUser(0, 1.2 * max);
        h2[iw]->Draw("COLZ");
        cout << "Histogram draw" << endl;
        sprintf(tmp, "#Gamma_{A,H}/m_{A,H} = %d%%", int(width_benchmark[iw] * 100));
        TLatex t1(700, 450, tmp);
        t1.Draw();
        sprintf(tmp, "distribution2D_%s.pdf", width_strings[iw].c_str());
        c1->SaveAs(tmp);
        delete c1;
    }

    return 0;
}
