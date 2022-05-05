#include "AZHGrid.h"

#include <algorithm>
#include <iostream>
#include <utility>

using namespace std;

AZHGrid::AZHGrid() : WIDTH_RATIO{0, 0.005, 0.01, 0.025, 0.05, 0.1, 0.25, 999} {
    WIDTH_RATIO_SIZE = WIDTH_RATIO.size();
    int mass_id = 0;
    for (int i_mha = 500; i_mha < 850; i_mha += 50) {
        for (int i_mhh = 400; i_mhh < i_mha - 50; i_mhh += 50) {
            MASS_POINTS[make_pair(i_mha, i_mhh)] = mass_id;
            ++mass_id;
        }
    }
    MASS_POINT_SIZE = MASS_POINTS.size();
}

int AZHGrid::get_mass_grid_point_lower_HEAVY(double mass) {
    int id = int(mass) / 50;
    return id * 50;
}

int AZHGrid::get_mass_grid_point_lower_LIGHT(double mass) {
    int id = int(mass) / 50;
    return id * 50;
}

Triangle AZHGrid::get_mass_triangle(double m_heavy, double m_light) {
    if (fabs(m_heavy - 800) < 1e-3 && fabs(m_light - 700) < 1e-3) {
        m_heavy = 799.5;
        m_light = 699;
    }
    if (fabs(m_heavy - 500) < 1e-3 && fabs(m_light - 400) < 1e-3) {
        m_heavy = 501;
        m_light = 400.5;
    }
    if (fabs(m_heavy - m_light - 100) < 1e-3) m_light -= 1e-3;
    if (fabs(m_heavy - 800) < 1e-3) m_heavy -= 1e-3;
    if (fabs(m_light - 400) < 1e-3) m_light += 1e-3;
    double m_heavy_low = get_mass_grid_point_lower_HEAVY(m_heavy);
    double m_light_low = get_mass_grid_point_lower_LIGHT(m_light);
    double dm_heavy = m_heavy - m_heavy_low;
    double dm_light = m_light - m_light_low;
    if (dm_heavy > dm_light) {
        return Triangle({m_heavy_low, m_light_low}, {m_heavy_low + 50, m_light_low},
                        {m_heavy_low + 50, m_light_low + 50});
    } else {
        return Triangle({m_heavy_low, m_light_low}, {m_heavy_low, m_light_low + 50},
                        {m_heavy_low + 50, m_light_low + 50});
    }
}

Triangle AZHGrid::get_width_ratio_triangle(double wr_heavy, double wr_light) {
    auto iter_heavy_up = upper_bound(WIDTH_RATIO.begin(), WIDTH_RATIO.end(), wr_heavy);
    auto iter_light_up = upper_bound(WIDTH_RATIO.begin(), WIDTH_RATIO.end(), wr_light);
    double wr_heavy_upper = *iter_heavy_up;
    double wr_light_upper = *iter_light_up;
    double wr_heavy_lower = *(--iter_heavy_up);
    double wr_light_lower = *(--iter_light_up);
    double dwr_heavy = wr_heavy - wr_heavy_lower;
    double dwr_light = wr_light - wr_light_lower;
    double slope = (wr_light_upper - wr_light_lower) / (wr_heavy_upper - wr_heavy_lower);
    double slope_point = (wr_light - wr_light_lower) / (wr_heavy - wr_heavy_lower);
    if (slope_point < slope) {
        return Triangle({wr_heavy_lower, wr_light_lower}, {wr_heavy_upper, wr_light_lower},
                        {wr_heavy_upper, wr_light_upper});
    } else {
        return Triangle({wr_heavy_lower, wr_light_lower}, {wr_heavy_lower, wr_light_upper},
                        {wr_heavy_upper, wr_light_upper});
    }
}

int AZHGrid::get_mass_parameter_id(double m_heavy, double m_light) {
    int im_heavy = 50 * int((m_heavy) / 50);
    int im_light = 50 * int((m_light) / 50);
    auto iter = MASS_POINTS.find(make_pair(im_heavy, im_light));
    if (iter == MASS_POINTS.end()) return -1;
    return iter->second;
}

std::string AZHGrid::get_width_string(double wr) {
    static const std::string width_ratios_s[8] = {"0x005", "0x005", "0x010", "0x025",
                                                  "0x050", "0x100", "0x250", "0x250"};
    auto iter = upper_bound(WIDTH_RATIO.begin(), WIDTH_RATIO.end(), wr);
    int id = iter - WIDTH_RATIO.begin() - 1;
    return width_ratios_s[id];
}

AZHPoint::AZHPoint(double MHA_, double MHH_, double WHA_, double WHH_, double tb_, double cba_)
    : MHA(MHA_), MHH(MHH_), WHA(WHA_), WHH(WHH_), tb(tb_), cba(cba_), within_region_(true), dist(400) {
    WRA = WHA / MHA;
    WRH = WHH / MHH;
    mass_triangle = grid.get_mass_triangle(MHA, MHH);
    width_triangle = grid.get_width_ratio_triangle(WRA, WRH);
    if (WRA > 0.2501 || WRH > 0.2501) {
        cout << "WIDTH TOO LARGE" << endl;
        within_region_ = false;
    }
    for (int i = 0; i < 3; i++) {
        mass_param_ids[i] = grid.get_mass_parameter_id(mass_triangle.P(i).x, mass_triangle.P(i).y);
        if (mass_param_ids[i] < 0) {
            within_region_ = false;
            cout << "MASS NOT MATCH" << endl;
        }
        width_strings[i] =
            "HA" + grid.get_width_string(width_triangle.P(i).x) + "_HH" + grid.get_width_string(width_triangle.P(i).y);
    }
    mass_triangle.Get_Barycentric_Coordinate({MHA, MHH}, mass_lams);
    width_triangle.Get_Barycentric_Coordinate({WRA, WRH}, width_lams);

    if (within_region_) {
        build_up_signal_distribution();
    }
}

void AZHPoint::build_up_signal_distribution() {
    for (int i_mass = 0; i_mass < 3; i_mass++) {
        Distribution_t dist_mass(400);
        for (int i_width = 0; i_width < 3; i_width++) {
            Distribution_t dist_width =
                get_signal_distribution_at_grid_point_typeI(mass_param_ids[i_mass], width_strings[i_width], tb, cba);
            dist_mass += dist_width * width_lams[i_width];
        }
        dist += dist_mass * mass_lams[i_mass];
    }
}

Distribution_t AZHPoint::get_signal_distribution_at_grid_point_typeI(int param_id, string width_string, double tb,
                                                                     double cba) {
    Distribution_t dist_ggF_TRI = get_signal_distribution_at_grid_point_typeI_ggF_TRI(param_id, width_string, tb, cba);
    // cout << "dist_ggF_TRI.cs = " << dist_ggF_TRI.cs << endl;
    Distribution_t dist_ggF_BOX = get_signal_distribution_at_grid_point_typeI_ggF_BOX(param_id, width_string, tb, cba);
    // cout << "dist_ggF_BOX.cs = " << dist_ggF_BOX.cs << endl;
    Distribution_t dist_ggF_Z = get_signal_distribution_at_grid_point_typeI_ggF_Z(param_id, width_string, tb, cba);
    // cout << "dist_ggF_Z.cs = " << dist_ggF_Z.cs << endl;
    Distribution_t dist_ggF_TRIxBOX =
        get_signal_distribution_at_grid_point_typeI_ggF_TRIxBOX(param_id, width_string, tb, cba);
    // cout << "dist_ggF_TRIxBOX.cs = " << dist_ggF_TRIxBOX.cs << endl;
    Distribution_t dist_ggF_TRIxZ =
        get_signal_distribution_at_grid_point_typeI_ggF_TRIxZ(param_id, width_string, tb, cba);
    // cout << "dist_ggF_TRIxZ.cs = " << dist_ggF_TRIxZ.cs << endl;
    Distribution_t dist_ggF_BOXxZ =
        get_signal_distribution_at_grid_point_typeI_ggF_BOXxZ(param_id, width_string, tb, cba);
    // cout << "dist_ggF_BOXxZ.cs = " << dist_ggF_BOXxZ.cs << endl;
    Distribution_t dist_qq_DY = get_signal_distribution_at_grid_point_typeI_qq_DY(param_id, width_string, tb, cba);
    // cout << "dist_qq_DY.cs = " << dist_qq_DY.cs << endl;
    Distribution_t dist_bb_Z = get_signal_distribution_at_grid_point_typeI_bb_Z(param_id, width_string, tb, cba);
    // cout << "dist_bb_Z.cs = " << dist_bb_Z.cs << endl;
    Distribution_t dist_bb_S = get_signal_distribution_at_grid_point_typeI_bb_S(param_id, width_string, tb, cba);
    // cout << "dist_bb_S.cs = " << dist_bb_S.cs << endl;
    Distribution_t dist_bb_T = get_signal_distribution_at_grid_point_typeI_bb_T(param_id, width_string, tb, cba);
    // cout << "dist_bb_T.cs = " << dist_bb_T.cs << endl;
    Distribution_t dist_bb_ZxS = get_signal_distribution_at_grid_point_typeI_bb_ZxS(param_id, width_string, tb, cba);
    // cout << "dist_bb_ZxS.cs = " << dist_bb_ZxS.cs << endl;
    Distribution_t dist_bb_ZxT = get_signal_distribution_at_grid_point_typeI_bb_ZxT(param_id, width_string, tb, cba);
    // cout << "dist_bb_ZxT.cs = " << dist_bb_ZxT.cs << endl;
    Distribution_t dist_bb_SxT = get_signal_distribution_at_grid_point_typeI_bb_SxT(param_id, width_string, tb, cba);
    // cout << "dist_bb_SxT.cs = " << dist_bb_SxT.cs << endl;

    return dist_ggF_TRI + dist_ggF_BOX + dist_ggF_Z + dist_ggF_TRIxBOX + dist_ggF_TRIxZ + dist_ggF_BOXxZ + dist_qq_DY +
           dist_bb_Z + dist_bb_S + dist_bb_T + dist_bb_ZxS + dist_bb_ZxT + dist_bb_SxT;
}

std::string AZHPoint::get_folder_name(int param_id, string width_string) {
    static const string folder_prefix = "../DATADIR/Distribution/202202201812_";
    return folder_prefix + width_string + "/" + to_string(param_id);
}

std::string AZHPoint::get_file_name_template(int param_id, string width_string) {
    static const string file_name_prefix = "%s/AZH_Distribution_%s_202202201812_";
    return file_name_prefix + width_string + "_" + to_string(param_id) + "_%s_3l.txt";
}

Distribution_t AZHPoint::get_signal_distribution_at_grid_point_typeI_ggF_TRI(int param_id, string width_string,
                                                                             double tb, double cba) {
    string folder = get_folder_name(param_id, width_string);
    string file_tmp = get_file_name_template(param_id, width_string);
    char filename[500];
    sprintf(filename, file_tmp.c_str(), folder.c_str(), "ggF_TRI", "tb1");
    Distribution_t dist_tb1(filename, 0);
    double scale_base = pow(kappa_f_A_typeI(1.0, 0.1) * kappa_VSS(1.0, 0.1) * kappa_f_H_typeI(1.0, 0.1), 2);
    double scale = pow(kappa_f_A_typeI(tb, cba) * kappa_VSS(tb, cba) * kappa_f_H_typeI(tb, cba), 2) / scale_base;
    return dist_tb1 * scale;
}

Distribution_t AZHPoint::get_signal_distribution_at_grid_point_typeI_ggF_BOX(int param_id, string width_string,
                                                                             double tb, double cba) {
    string folder = get_folder_name(param_id, width_string);
    string file_tmp = get_file_name_template(param_id, width_string);
    char filename[500];
    sprintf(filename, file_tmp.c_str(), folder.c_str(), "ggF_BOX", "tb1");
    Distribution_t dist_tb1(filename, 0);
    double scale_base = pow(kappa_f_H_typeI(1.0, 0.1), 4);
    double scale = pow(kappa_f_H_typeI(tb, cba), 4) / scale_base;
    return dist_tb1 * scale;
}

Distribution_t AZHPoint::get_signal_distribution_at_grid_point_typeI_ggF_Z(int param_id, string width_string, double tb,
                                                                           double cba) {
    string folder = get_folder_name(param_id, width_string);
    string file_tmp = get_file_name_template(param_id, width_string);
    char filename[500];
    sprintf(filename, file_tmp.c_str(), folder.c_str(), "ggF_Z", "tb1");
    Distribution_t dist_tb1(filename, 0);
    double scale_base = pow(kappa_VVS(1.0, 0.1) * kappa_f_H_typeI(1.0, 0.1), 2);
    double scale = pow(kappa_VVS(tb, cba) * kappa_f_H_typeI(tb, cba), 2) / scale_base;
    return dist_tb1 * scale;
}

Distribution_t AZHPoint::get_signal_distribution_at_grid_point_typeI_ggF_TRIxBOX(int param_id, string width_string,
                                                                                 double tb, double cba) {
    string folder = get_folder_name(param_id, width_string);
    string file_tmp = get_file_name_template(param_id, width_string);
    char filename[500];
    sprintf(filename, file_tmp.c_str(), folder.c_str(), "ggF_TRIxBOX", "tb1");
    Distribution_t dist_tb1(filename, 0);
    double scale_base = kappa_f_A_typeI(1.0, 0.1) * kappa_VVS(1.0, 0.1) * pow(kappa_f_H_typeI(1.0, 0.1), 3);
    double scale = kappa_f_A_typeI(tb, cba) * kappa_VVS(tb, cba) * pow(kappa_f_H_typeI(tb, cba), 3) / scale_base;
    return dist_tb1 * scale;
}

Distribution_t AZHPoint::get_signal_distribution_at_grid_point_typeI_ggF_TRIxZ(int param_id, string width_string,
                                                                               double tb, double cba) {
    string folder = get_folder_name(param_id, width_string);
    string file_tmp = get_file_name_template(param_id, width_string);
    char filename[500];
    sprintf(filename, file_tmp.c_str(), folder.c_str(), "ggF_TRIxZ", "tb1");
    Distribution_t dist_tb1(filename, 0);
    double scale_base =
        kappa_f_A_typeI(1.0, 0.1) * kappa_VVS(1.0, 0.1) * kappa_VSS(1.0, 0.1) * pow(kappa_f_H_typeI(1.0, 0.1), 2);
    double scale = kappa_f_A_typeI(tb, cba) * kappa_VVS(tb, cba) * kappa_VSS(tb, cba) *
                   pow(kappa_f_H_typeI(tb, cba), 2) / scale_base;
    return dist_tb1 * scale;
}

Distribution_t AZHPoint::get_signal_distribution_at_grid_point_typeI_ggF_BOXxZ(int param_id, string width_string,
                                                                               double tb, double cba) {
    string folder = get_folder_name(param_id, width_string);
    string file_tmp = get_file_name_template(param_id, width_string);
    char filename[500];
    sprintf(filename, file_tmp.c_str(), folder.c_str(), "ggF_BOXxZ", "tb1");
    Distribution_t dist_tb1(filename, 0);
    double scale_base = kappa_VVS(1.0, 0.1) * pow(kappa_f_H_typeI(1.0, 0.1), 3);
    double scale = kappa_VVS(tb, cba) * pow(kappa_f_H_typeI(tb, cba), 3) / scale_base;
    return dist_tb1 * scale;
}

Distribution_t AZHPoint::get_signal_distribution_at_grid_point_typeI_qq_DY(int param_id, string width_string, double tb,
                                                                           double cba) {
    string folder = get_folder_name(param_id, width_string);
    string file_tmp = get_file_name_template(param_id, width_string);
    char filename[500];
    sprintf(filename, file_tmp.c_str(), folder.c_str(), "qq_DY", "tb1");
    Distribution_t dist_tb1(filename, 0);
    double scale_base = pow(kappa_VVS(1.0, 0.1) * kappa_f_H_typeI(1.0, 0.1), 2);
    double scale = pow(kappa_VVS(tb, cba) * kappa_f_H_typeI(tb, cba), 2) / scale_base;
    return dist_tb1 * scale;
}

Distribution_t AZHPoint::get_signal_distribution_at_grid_point_typeI_bb_Z(int param_id, string width_string, double tb,
                                                                          double cba) {
    string folder = get_folder_name(param_id, width_string);
    string file_tmp = get_file_name_template(param_id, width_string);
    char filename[500];
    sprintf(filename, file_tmp.c_str(), folder.c_str(), "bb_Z", "tb1");
    Distribution_t dist_tb1(filename, 0);
    double scale_base = pow(kappa_VVS(1.0, 0.1) * kappa_f_H_typeI(1.0, 0.1), 2);
    double scale = pow(kappa_VVS(tb, cba) * kappa_f_H_typeI(tb, cba), 2) / scale_base;
    return dist_tb1 * scale;
}

Distribution_t AZHPoint::get_signal_distribution_at_grid_point_typeI_bb_S(int param_id, string width_string, double tb,
                                                                          double cba) {
    string folder = get_folder_name(param_id, width_string);
    string file_tmp = get_file_name_template(param_id, width_string);
    char filename[500];
    sprintf(filename, file_tmp.c_str(), folder.c_str(), "bb_S", "tb1");
    Distribution_t dist_tb1(filename, 0);
    double scale_base = pow(kappa_f_A_typeI(1.0, 0.1) * kappa_VSS(1.0, 0.1) * kappa_f_H_typeI(1.0, 0.1), 2);
    double scale = pow(kappa_f_A_typeI(tb, cba) * kappa_VSS(tb, cba) * kappa_f_H_typeI(tb, cba), 2) / scale_base;
    return dist_tb1 * scale;
}

Distribution_t AZHPoint::get_signal_distribution_at_grid_point_typeI_bb_T(int param_id, string width_string, double tb,
                                                                          double cba) {
    string folder = get_folder_name(param_id, width_string);
    string file_tmp = get_file_name_template(param_id, width_string);
    char filename[500];
    sprintf(filename, file_tmp.c_str(), folder.c_str(), "bb_T", "tb1");
    Distribution_t dist_tb1(filename, 0);
    double scale_base = pow(kappa_f_H_typeI(1.0, 0.1), 4);
    double scale = pow(kappa_f_H_typeI(tb, cba), 4) / scale_base;
    return dist_tb1 * scale;
}

Distribution_t AZHPoint::get_signal_distribution_at_grid_point_typeI_bb_ZxS(int param_id, string width_string,
                                                                            double tb, double cba) {
    string folder = get_folder_name(param_id, width_string);
    string file_tmp = get_file_name_template(param_id, width_string);
    char filename[500];
    sprintf(filename, file_tmp.c_str(), folder.c_str(), "bb_ZxS", "tb1");
    Distribution_t dist_tb1(filename, 0);
    double scale_base =
        kappa_f_A_typeI(1.0, 0.1) * kappa_VVS(1.0, 0.1) * kappa_VSS(1.0, 0.1) * pow(kappa_f_H_typeI(1.0, 0.1), 2);
    double scale = kappa_f_A_typeI(tb, cba) * kappa_VVS(tb, cba) * kappa_VSS(tb, cba) *
                   pow(kappa_f_H_typeI(tb, cba), 2) / scale_base;
    return dist_tb1 * scale;
}

Distribution_t AZHPoint::get_signal_distribution_at_grid_point_typeI_bb_ZxT(int param_id, string width_string,
                                                                            double tb, double cba) {
    string folder = get_folder_name(param_id, width_string);
    string file_tmp = get_file_name_template(param_id, width_string);
    char filename[500];
    sprintf(filename, file_tmp.c_str(), folder.c_str(), "bb_ZxT", "tb1");
    Distribution_t dist_tb1(filename, 0);
    double scale_base = kappa_VVS(1.0, 0.1) * pow(kappa_f_H_typeI(1.0, 0.1), 3);
    double scale = kappa_VVS(tb, cba) * pow(kappa_f_H_typeI(tb, cba), 3) / scale_base;
    return dist_tb1 * scale;
}

Distribution_t AZHPoint::get_signal_distribution_at_grid_point_typeI_bb_SxT(int param_id, string width_string,
                                                                            double tb, double cba) {
    string folder = get_folder_name(param_id, width_string);
    string file_tmp = get_file_name_template(param_id, width_string);
    char filename[500];
    sprintf(filename, file_tmp.c_str(), folder.c_str(), "bb_SxT", "tb1");
    Distribution_t dist_tb1(filename, 0);
    double scale_base = kappa_f_A_typeI(1.0, 0.1) * kappa_VSS(1.0, 0.1) * pow(kappa_f_H_typeI(1.0, 0.1), 3);
    double scale = kappa_f_A_typeI(tb, cba) * kappa_VSS(tb, cba) * pow(kappa_f_H_typeI(tb, cba), 3) / scale_base;
    return dist_tb1 * scale;
}

HZAPoint::HZAPoint(double MHA_, double MHH_, double WHA_, double WHH_, double tb_, double cba_)
    : MHA(MHA_), MHH(MHH_), WHA(WHA_), WHH(WHH_), tb(tb_), cba(cba_), within_region_(true), dist(400) {
    WRA = WHA / MHA;
    WRH = WHH / MHH;
    mass_triangle = grid.get_mass_triangle(MHH, MHA);
    width_triangle = grid.get_width_ratio_triangle(WRH, WRA);
    cout << "Triangle Obtained" << endl;
    if (WRA > 0.2501 || WRH > 0.2501) {
        cout << "WIDTH TOO LARGE" << endl;
        within_region_ = false;
    }
    for (int i = 0; i < 3; i++) {
        mass_param_ids[i] = grid.get_mass_parameter_id(mass_triangle.P(i).x, mass_triangle.P(i).y);
        if (mass_param_ids[i] < 0) {
            within_region_ = false;
            cout << "MASS NOT MATCH" << endl;
        }
        width_strings[i] =
            "HH" + grid.get_width_string(width_triangle.P(i).x) + "_HA" + grid.get_width_string(width_triangle.P(i).y);
    }
    mass_triangle.Get_Barycentric_Coordinate({MHH, MHA}, mass_lams);
    width_triangle.Get_Barycentric_Coordinate({WRH, WRA}, width_lams);
    cout << "Barycentric coordinate obtained" << endl;
    cout << "mass lam: (" << mass_lams[0] << "," << mass_lams[1] << "," << mass_lams[2] << ")" << endl;
    cout << "width lam: (" << width_lams[0] << "," << width_lams[1] << "," << width_lams[2] << ")" << endl;
    if (within_region_) {
        build_up_signal_distribution();
        cout << "distribution built" << endl;
    }
}

void HZAPoint::build_up_signal_distribution() {
    for (int i_mass = 0; i_mass < 3; i_mass++) {
        Distribution_t dist_mass(400);
        for (int i_width = 0; i_width < 3; i_width++) {
            cout << "For im=" << i_mass << " iw=" << i_width << endl;
            cout << " mass id=" << mass_param_ids[i_mass] << " width string=" << width_strings[i_width] << endl;
            Distribution_t dist_width =
                get_signal_distribution_at_grid_point_typeI(mass_param_ids[i_mass], width_strings[i_width], tb, cba);
            cout << "  Distribution for the point obtained" << endl;
            cout << "  dist_width.NBINS = " << dist_width.NBINS << endl;
            dist_mass += dist_width * width_lams[i_width];
            cout << "  Added to mass dist" << endl;
        }
        dist += dist_mass * mass_lams[i_mass];
        cout << "  Added to total dist" << endl;
    }
}

Distribution_t HZAPoint::get_signal_distribution_at_grid_point_typeI(int param_id, string width_string, double tb,
                                                                     double cba) {
    Distribution_t dist_ggF_TRI = get_signal_distribution_at_grid_point_typeI_ggF_TRI(param_id, width_string, tb, cba);
    cout << "TRI obtained, NBINS=" << dist_ggF_TRI.NBINS << endl;
    Distribution_t dist_ggF_BOX = get_signal_distribution_at_grid_point_typeI_ggF_BOX(param_id, width_string, tb, cba);
    cout << "BOX obtained" << endl;
    Distribution_t dist_ggF_TRIxBOX =
        get_signal_distribution_at_grid_point_typeI_ggF_TRIxBOX(param_id, width_string, tb, cba);
    cout << "TRIxBOX obtained" << endl;
    Distribution_t dist_bb_S = get_signal_distribution_at_grid_point_typeI_bb_S(param_id, width_string, tb, cba);
    cout << "S obtained" << endl;
    Distribution_t dist_bb_T = get_signal_distribution_at_grid_point_typeI_bb_T(param_id, width_string, tb, cba);
    cout << "T obtained" << endl;
    Distribution_t dist_bb_SxT = get_signal_distribution_at_grid_point_typeI_bb_SxT(param_id, width_string, tb, cba);
    cout << "SxT obtained" << endl;

    return dist_ggF_TRI + dist_ggF_BOX + dist_ggF_TRIxBOX + dist_bb_S + dist_bb_T + dist_bb_SxT;
}

Distribution_t HZAPoint::get_signal_distribution_at_grid_point_typeI_ggF_TRI(int param_id, string width_string,
                                                                             double tb, double cba) {
    string folder = get_folder_name(param_id, width_string);
    string file_tmp = get_file_name_template(param_id, width_string);
    char filename[500];
    sprintf(filename, file_tmp.c_str(), folder.c_str(), "ggF_TRI", "tb1");
    Distribution_t dist_tb1(filename, 0);
    double scale_base = pow(kappa_f_A_typeI(1.0, 0.1) * kappa_VSS(1.0, 0.1) * kappa_f_H_typeI(1.0, 0.1), 2);
    double scale = pow(kappa_f_A_typeI(tb, cba) * kappa_VSS(tb, cba) * kappa_f_H_typeI(tb, cba), 2) / scale_base;
    return dist_tb1 * scale;
}

Distribution_t HZAPoint::get_signal_distribution_at_grid_point_typeI_ggF_BOX(int param_id, string width_string,
                                                                             double tb, double cba) {
    string folder = get_folder_name(param_id, width_string);
    string file_tmp = get_file_name_template(param_id, width_string);
    char filename[500];
    sprintf(filename, file_tmp.c_str(), folder.c_str(), "ggF_BOX", "tb1");
    Distribution_t dist_tb1(filename, 0);
    double scale_base = pow(kappa_f_A_typeI(1.0, 0.1), 4);
    double scale = pow(kappa_f_A_typeI(tb, cba), 4) / scale_base;
    return dist_tb1 * scale;
}

Distribution_t HZAPoint::get_signal_distribution_at_grid_point_typeI_ggF_TRIxBOX(int param_id, string width_string,
                                                                                 double tb, double cba) {
    string folder = get_folder_name(param_id, width_string);
    string file_tmp = get_file_name_template(param_id, width_string);
    char filename[500];
    sprintf(filename, file_tmp.c_str(), folder.c_str(), "ggF_TRIxBOX", "tb1");
    Distribution_t dist_tb1(filename, 0);
    double scale_base = kappa_f_H_typeI(1.0, 0.1) * kappa_VVS(1.0, 0.1) * pow(kappa_f_A_typeI(1.0, 0.1), 3);
    double scale = kappa_f_H_typeI(tb, cba) * kappa_VVS(tb, cba) * pow(kappa_f_A_typeI(tb, cba), 3) / scale_base;
    return dist_tb1 * scale;
}

Distribution_t HZAPoint::get_signal_distribution_at_grid_point_typeI_bb_S(int param_id, string width_string, double tb,
                                                                          double cba) {
    string folder = get_folder_name(param_id, width_string);
    string file_tmp = get_file_name_template(param_id, width_string);
    char filename[500];
    sprintf(filename, file_tmp.c_str(), folder.c_str(), "bb_S", "tb1");
    Distribution_t dist_tb1(filename, 0);
    double scale_base = pow(kappa_f_A_typeI(1.0, 0.1) * kappa_VSS(1.0, 0.1) * kappa_f_H_typeI(1.0, 0.1), 2);
    double scale = pow(kappa_f_A_typeI(tb, cba) * kappa_VSS(tb, cba) * kappa_f_H_typeI(tb, cba), 2) / scale_base;
    return dist_tb1 * scale;
}

Distribution_t HZAPoint::get_signal_distribution_at_grid_point_typeI_bb_T(int param_id, string width_string, double tb,
                                                                          double cba) {
    string folder = get_folder_name(param_id, width_string);
    string file_tmp = get_file_name_template(param_id, width_string);
    char filename[500];
    sprintf(filename, file_tmp.c_str(), folder.c_str(), "bb_T", "tb1");
    Distribution_t dist_tb1(filename, 0);
    double scale_base = pow(kappa_f_A_typeI(1.0, 0.1), 4);
    double scale = pow(kappa_f_A_typeI(tb, cba), 4) / scale_base;
    return dist_tb1 * scale;
}

Distribution_t HZAPoint::get_signal_distribution_at_grid_point_typeI_bb_SxT(int param_id, string width_string,
                                                                            double tb, double cba) {
    string folder = get_folder_name(param_id, width_string);
    string file_tmp = get_file_name_template(param_id, width_string);
    char filename[500];
    sprintf(filename, file_tmp.c_str(), folder.c_str(), "bb_SxT", "tb1");
    Distribution_t dist_tb1(filename, 0);
    double scale_base = kappa_f_H_typeI(1.0, 0.1) * kappa_VSS(1.0, 0.1) * pow(kappa_f_A_typeI(1.0, 0.1), 3);
    double scale = kappa_f_H_typeI(tb, cba) * kappa_VSS(tb, cba) * pow(kappa_f_A_typeI(tb, cba), 3) / scale_base;
    return dist_tb1 * scale;
}

std::string HZAPoint::get_folder_name(int param_id, string width_string) {
    static const string folder_prefix = "../DATADIR/Distribution/202204191220_";
    return folder_prefix + width_string + "/" + to_string(param_id);
}

std::string HZAPoint::get_file_name_template(int param_id, string width_string) {
    static const string file_name_prefix = "%s/AZH_Distribution_%s_202204191220_";
    return file_name_prefix + width_string + "_" + to_string(param_id) + "_%s_3l.txt";
}
