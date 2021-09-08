#include <fstream>
#include <iostream>
#include <string>

#include "AZHPreAnalysis.h"
#include "NLL_Calculator.h"
#include "Utilities.h"

using namespace std;

int main(int argc, char const *argv[]) {
    //* 0: exe name
    //* 1: output file
    //* 2: sig filename
    //* 3: bkg filename

    ofstream output(argv[1]);
    NLL_Calculator NLLC(argv[2], argv[3]);
    double nll = NLLC.Get_NLL();
    double mu = NLLC.Get_mu_at_95CL();

    output << "NLL  " << nll << endl;
    output << "MU   " << mu << endl;

    return 0;
}
