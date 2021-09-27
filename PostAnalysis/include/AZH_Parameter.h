#include <vector>

class Distribution_Data {
public:
    std::vector<double> HIST_BINS;
    double CS_WITHOUT_DECAY;
    int NBINS;

    Distribution_Data(char *root_file);
    ~Distribution_Data(){};
};

class AZH_Parameter {
public:
    double MHA;
    double MHH;
    double WHA;
    double WHH;
    Distribution_Data TRI_Data;
    Distribution_Data BOX_Data;
    Distribution_Data INTER_Data;

    AZH_Parameter(double MHA, double MHH, double WHA, double WHH, char *tri_file_name, char *box_file_name,
                  char *inter_file_name);
    ~AZH_Parameter(){};
};

class AZH_Grid {
private:
    int NUM_POINTS;
    std::vector<AZH_Parameter> Grid;
    Distribution_Data *BKG;

public:
    AZH_Grid(char const *data_dir, char const *param_id);
    ~AZH_Grid(){};

    void Dump_Grid(char const *file_prefix);
};
