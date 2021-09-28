#include <string>
#include <vector>

class Distribution_Data {
public:
    std::vector<double> HIST_BINS;
    double CS_WITHOUT_DECAY;
    int NBINS;

    Distribution_Data(char *root_file);
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
    ~AZH_Parameter(){};
};

struct Point {
    double x;
    double y;
    Point();
};

class Triangle {
private:
    Point P1, P2, P3;

public:
    Triangle(){};
    Triangle(Point x1, Point x2, Point x3);
    ~Triangle(){};

    void Get_Barycentric_Coordinate(const Point &P, double &l1, double &l2, double &l3);
};

class AZH_Grid {
private:
    int NUM_POINTS;
    // std::vector<AZH_Parameter> Grid;
    AZH_Parameter *Grid[7][7][7][7];
    Distribution_Data *BKG;
    std::string WR_CHR[7];
    double WR[7];

public:
    AZH_Grid(char const *data_dir, char const *param_id);
    ~AZH_Grid();

    int Get_Width_Index(double wr);
    void Get_Mass_Index(double mha, double mhh, int &id_a, int &id_h);

    void Dump_Grid(char const *file_prefix);

    bool Get_Mass_Triangle(double mha, double mhh, Triangle &t);
    bool Get_Width_Triangle(double wra, double wrh, Triangle &t);
    bool Get_Signal_Distribution(double mha, double mhh, double wha, double whh, double tb, double alpha,
                                 Distribution_Data &dd);
    double Calculate_NLL(double mha, double mhh, double wha, double whh, double tb, double alpha);
};
