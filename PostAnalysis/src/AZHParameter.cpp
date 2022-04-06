#include "AZHParameter.h"

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

using namespace std;

AZHParameter_t::AZHParameter_t(double MHA_IN, double MHH_IN, double WHA_IN, double WHH_IN, double tb_IN, double cba_IN,
                               const char* root_file_name)
    : MHA(MHA_IN), MHH(MHH_IN), WHA(WHA_IN), WHH(WHH_IN), tb(tb_IN), cba(cba_IN), dist(root_file_name) {}

void AZHParameter_t::Dump_Distribution(const char* output_file_name) {
    ofstream dist_file(output_file_name);
    dist_file << "MHA\tMHH\tWHA\tWHH\ttb\tcba\tcs";
    for (int i = 0; i < dist.NBINS; i++) {
        dist_file << "\tBIN" << i;
    }
    dist_file << endl;
    dist_file << MHA << "\t" << MHH << "\t" << WHA << "\t" << WHH << "\t" << tb << "\t" << cba << "\t" << dist.cs;
    for (int i = 0; i < dist.NBINS; i++) {
        dist_file << "\t" << dist.data_cs[i];
    }
    dist_file << endl;
    dist_file << MHA << "\t" << MHH << "\t" << WHA << "\t" << WHH << "\t" << tb << "\t" << cba << "\t" << dist.n_mc;
    for (int i = 0; i < dist.NBINS; i++) {
        dist_file << "\t" << dist.data_mc_count[i];
    }
    dist_file << endl;
}
