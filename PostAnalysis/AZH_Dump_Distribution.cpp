#include <iostream>

#include "AZHDistribution.h"
#include "AZHParameter.h"

using namespace std;

int main(int argc, char const *argv[]) {
    // * 0: exe name
    // * 1: MHA
    // * 2: MHH
    // * 3: WHA
    // * 4: WHH
    // * 5: tb
    // * 6: cba
    // * 7: input root file name
    // * 8: output file name
    if (argc != 9) {
        cout << "Usage: " << argv[0] << " MHA MHH WHA WHH tb cba cs <root_file_name> <output_file_name>" << endl;
        return -1;
    }
    double MHA = atof(argv[1]);
    double MHH = atof(argv[2]);
    double WHA = atof(argv[3]);
    double WHH = atof(argv[4]);
    double tb = atof(argv[5]);
    double cba = atof(argv[6]);
    AZHParameter_t param(MHA, MHH, WHA, WHH, tb, cba, argv[7]);
    param.Dump_Distribution(argv[8]);

    return 0;
}
