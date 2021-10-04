#include <string>
#include <vector>

class Distribution_Data {
public:
    std::vector<double> HIST_BINS;
    double CS_WITHOUT_DECAY;
    int NBINS;

    Distribution_Data(char *root_file);
    Distribution_Data(std::stringstream &sstr);
    Distribution_Data();
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

    AZH_Parameter(double MHA_IN, double MHH_IN, double WHA_IN, double WHH_IN, char *tri_file_name, char *box_file_name,
                  char *inter_file_name);
    AZH_Parameter(double MHA_IN, double MHH_IN, double WHA_IN, double WHH_IN, std::stringstream &tri_line,
                  std::stringstream &box_line, std::stringstream &inter_line);
    ~AZH_Parameter(){};
};

struct Point {
    double x;
    double y;
    int id_x;
    int id_y;
};

class Triangle {
public:
    Triangle(){};
    Triangle(Point x1, Point x2, Point x3);
    ~Triangle(){};

    void Get_Barycentric_Coordinate(const Point &P, double *lams);
    Point &P(int i);
    Point P1, P2, P3;
};

class AZH_Grid {
private:
    int NUM_POINTS;
    bool NEED_TO_DELETE;
    bool FLIPPED;  // False: A->ZH, True: H->ZA
    // std::vector<AZH_Parameter> Grid;
    AZH_Parameter *Grid[7][7][7][7];
    Distribution_Data *BKG;
    std::string WR_CHR[7];
    double WR[7];
    double MHA_MIN;
    double MHA_MAX;
    double MHA_STEP;
    double MHH_MIN;
    double MHH_MAX;
    double MHH_STEP;

public:
    AZH_Grid(bool FLIP = false);
    AZH_Grid(char const *data_dir, char const *param_id, bool FLIP = false);
    AZH_Grid(char const *dist_prefix, bool FLIP = false);
    ~AZH_Grid();

    void Read_Data(char const *data_dir, char const *param_id);
    void Read_Data(char const *dist_prefix);
    int Get_Width_Index(double wr);
    void Get_Mass_Index(double mha, double mhh, int &id_a, int &id_h);

    double Get_MHA(int id_a) { return MHA_MIN + id_a * MHA_STEP; }
    double Get_MHH(int id_h) { return MHH_MIN + id_h * MHH_STEP; }

    void Dump_Grid(char const *file_prefix);

    bool Get_Mass_Triangle(double mha, double mhh, Triangle &t);
    bool Get_Width_Triangle(double wra, double wrh, Triangle &t);
    bool Get_Signal_Distribution(double mha, double mhh, double wha, double whh, double tb, double alpha,
                                 Distribution_Data &dd);
    double Calculate_NLL(double mha, double mhh, double wha, double whh, double tb, double alpha, double lumi = 3000);
};
