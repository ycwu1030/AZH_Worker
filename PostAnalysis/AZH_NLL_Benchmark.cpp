#include <fstream>
#include <iostream>
#include <string>

#include "AZHGrid.h"
#include "AZHNLL.h"

using namespace std;

int main(int argc, char const *argv[]) {
    Distribution_t dist_bkg("../DATADIR/Distribution/BKG/COMB/AZH_Distribution_bkg_total_BKG_COMB_DEFAULT_3l.txt", 0);
    double width_ratios[6] = {0.005, 0.01, 0.025, 0.05, 0.1, 0.25};
    string width_strings[6] = {"0x005", "0x010", "0x025", "0x050", "0x100", "0x250"};
    for (int iwa = 0; iwa < 6; iwa++) {
        for (int iwh = 0; iwh < 6; iwh++) {
            char filename[200];
            sprintf(filename, "AZH_NLL_Result_Benchmark_HA%s_HH%s.txt", width_strings[iwa].c_str(),
                    width_strings[iwh].c_str());
            ofstream output(filename);
            output << "MHA MHH WRA WRH tb cba cs nll mu95" << endl;
            for (double mha = 500; mha < 850; mha += 50) {
                for (double mhh = 400; mhh < mha - 50; mhh += 50) {
                    double wha = mha * width_ratios[iwa];
                    double whh = mhh * width_ratios[iwh];
                    AZHPoint p_sig(mha, mhh, wha, whh, 1.0, 0.0);
                    cout << mha << " " << mhh << " " << width_ratios[iwa] << " " << width_ratios[iwh] << " "
                         << p_sig.within_region() << endl;
                    if (p_sig.within_region()) {
                        Distribution_t &dist_sig = p_sig.get_distribution();
                        AZHNLL nll_calc(&dist_sig, &dist_bkg);
                        double nll = nll_calc.get_nll();
                        double mu95 = nll_calc.get_mu_at_95CL();
                        output << mha << " " << mhh << " " << width_ratios[iwa] << " " << width_ratios[iwh]
                               << " 1.0 0.0 " << dist_sig.cs << " " << nll << " " << mu95 << endl;
                    }
                }
            }
        }
    }

    return 0;
}
