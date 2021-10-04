#include <fstream>
#include <iostream>
#include <string>

#include "AZHPreAnalysis.h"
#include "AZH_Parameter.h"
#include "TFile.h"
#include "TTree.h"
#include "Utilities.h"

using namespace std;

int main(int argc, char const *argv[]) {
    //* 0: exe name
    //* 1: data_dir
    //* 2: param id
    //* 3: output_prefix
    //* 4: A->ZH or H->ZA: 0 for A->ZH, 1 for H->ZA

    AZH_Grid grid(argv[1], argv[2], atoi(argv[4]));
    grid.Dump_Grid(argv[3]);

    return 0;
}
