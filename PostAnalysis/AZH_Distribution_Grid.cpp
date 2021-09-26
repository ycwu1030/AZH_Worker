#include <fstream>
#include <iostream>
#include <string>

#include "AZHPreAnalysis.h"
#include "AZH_Parameter.h"
#include "Utilities.h"

using namespace std;

int main(int argc, char const *argv[]) {
    //* 0: exe name
    //* 1: data_dir
    //* 2: param id
    //* 3: output_prefix

    // char data_dir[200];
    // char param_id[200];
    // char output_prefix[200];
    // sprintf(data_dir,"%s",argv[1]);
    // sprintf(data_d)
    AZH_Grid grid(argv[1], argv[2]);
    grid.Dump_Grid(argv[3]);

    return 0;
}
