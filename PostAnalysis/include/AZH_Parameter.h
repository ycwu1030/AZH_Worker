#include <vector>

class AZH_Parameter {
public:
    double MHA;
    double MHH;
    double WHA;
    double WHH;
    std::vector<double> HIST_TRI;
    std::vector<double> HIST_BOX;
    std::vector<double> HIST_INTER;
    int NBINS;

    AZH_Parameter(double MHA, double MHH, double WHA, double WHH, char *tri_file_name, char *box_file_name,
                  char *inter_file_name);
    ~AZH_Parameter(){};
};

class AZH_Grid {
private:
    int NUM_POINTS;
    std::vector<AZH_Parameter> Grid;

public:
    AZH_Grid(char const *data_dir, char const *param_id);
    ~AZH_Grid(){};

    void Dump_Grid(char const *file_prefix);
};
