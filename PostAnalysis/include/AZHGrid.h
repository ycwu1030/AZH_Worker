#ifndef AZH_GRID_H_
#define AZH_GRID_H_

#include <functional>
#include <map>
#include <string>
#include <vector>

#include "AZHBarycenter.h"
#include "AZHDistribution.h"

class AZHGrid {
public:
    AZHGrid();
    ~AZHGrid(){};

    Triangle get_mass_triangle(double mha, double mhh);
    Triangle get_width_ratio_triangle(double wra, double wrh);

    int get_mass_parameter_id(double mha, double mhh);
    std::string get_width_string(double wr);

private:
    int MASS_POINT_SIZE;
    std::map<std::pair<int, int>, int> MASS_POINTS;
    int WIDTH_RATIO_SIZE;
    std::vector<double> WIDTH_RATIO;

    int get_mass_grid_point_lower_HA(double mass);
    int get_mass_grid_point_lower_HH(double mass);
};

class AZHPoint {
public:
    double MHA;
    double MHH;
    double WHA;
    double WHH;
    double WRA;
    double WRH;
    double tb;
    double cba;

    AZHPoint(double MHA, double MHH, double WHA, double WHH, double tb, double cba);

    bool within_region() const { return within_region_; }
    Distribution_t &get_distribution() { return dist; }

private:
    Triangle mass_triangle;
    int mass_param_ids[3];
    double mass_lams[3];
    Triangle width_triangle;
    std::string width_strings[3];
    double width_lams[3];

    AZHGrid grid;
    bool within_region_;

    Distribution_t dist;

    void build_up_signal_distribution();

    Distribution_t get_signal_distribution_at_grid_point_typeI(int param_id, std::string width_string, double tb,
                                                               double cba);
    Distribution_t get_signal_distribution_at_grid_point_typeI_ggF_TRI(int param_id, std::string width_string,
                                                                       double tb, double cba);
    Distribution_t get_signal_distribution_at_grid_point_typeI_ggF_BOX(int param_id, std::string width_string,
                                                                       double tb, double cba);
    Distribution_t get_signal_distribution_at_grid_point_typeI_ggF_Z(int param_id, std::string width_string, double tb,
                                                                     double cba);
    Distribution_t get_signal_distribution_at_grid_point_typeI_ggF_TRIxBOX(int param_id, std::string width_string,
                                                                           double tb, double cba);
    Distribution_t get_signal_distribution_at_grid_point_typeI_ggF_TRIxZ(int param_id, std::string width_string,
                                                                         double tb, double cba);
    Distribution_t get_signal_distribution_at_grid_point_typeI_ggF_BOXxZ(int param_id, std::string width_string,
                                                                         double tb, double cba);
    Distribution_t get_signal_distribution_at_grid_point_typeI_qq_DY(int param_id, std::string width_string, double tb,
                                                                     double cba);
    Distribution_t get_signal_distribution_at_grid_point_typeI_bb_Z(int param_id, std::string width_string, double tb,
                                                                    double cba);
    Distribution_t get_signal_distribution_at_grid_point_typeI_bb_S(int param_id, std::string width_string, double tb,
                                                                    double cba);
    Distribution_t get_signal_distribution_at_grid_point_typeI_bb_T(int param_id, std::string width_string, double tb,
                                                                    double cba);
    Distribution_t get_signal_distribution_at_grid_point_typeI_bb_ZxS(int param_id, std::string width_string, double tb,
                                                                      double cba);
    Distribution_t get_signal_distribution_at_grid_point_typeI_bb_ZxT(int param_id, std::string width_string, double tb,
                                                                      double cba);
    Distribution_t get_signal_distribution_at_grid_point_typeI_bb_SxT(int param_id, std::string width_string, double tb,
                                                                      double cba);

    std::string get_folder_name(int param_id, std::string width_string);
    std::string get_file_name_template(int param_id, std::string width_string);

    double kappa_f_A_typeI(double tb, double cba) { return 1.0 / tb; }
    double kappa_u_A_typeII(double tb, double cba) { return 1.0 / tb; }
    double kappa_d_A_typeII(double tb, double cba) { return tb; }
    double kappa_u_H_typeII(double tb, double cba) {
        double sba = sqrt(1.0 - cba * cba);
        return cba - sba / tb;
    }
    double kappa_d_H_typeII(double tb, double cba) {
        double sba = sqrt(1.0 - cba * cba);
        return cba + sba * tb;
    }
    double kappa_f_H_typeI(double tb, double cba) {
        double sba = sqrt(1.0 - cba * cba);
        return cba - sba / tb;
    }
    double kappa_VVS(double tb, double cba) { return cba; }
    double kappa_VSS(double tb, double cba) { return sqrt(1.0 - cba * cba); }
};

#endif  // AZH_GRID_H_
