#ifndef __PLOT_POOL_H__
#define __PLOT_POOL_H__
#include <string>
#include <vector>

#include "TCanvas.h"
#include "TH1F.h"
#include "THStack.h"
#include "TLegend.h"

class Plot_Pool {
private:
    static const int MAX_VARS = 30;
    int NBINX[MAX_VARS];
    double X_LOW[MAX_VARS];
    double X_HIGH[MAX_VARS];
    std::string TITLE_X[MAX_VARS];
    std::string NAME_X[MAX_VARS];
    double *VARIABLE_X_D[MAX_VARS];
    int *VARIABLE_X_I[MAX_VARS];
    int NVAR;

    std::vector<TH1F *> HIST_SIG;
    std::vector<double> NORM_SIG;

    std::vector<TH1F *> HIST_BKG;
    std::vector<double> NORM_BKG;

    std::vector<THStack *> STK;
    // std::vector<THStack *> STK_NORM;

    std::vector<TLegend *> LG;

    std::vector<TCanvas *> CANVAS;
    // std::vector<TCanvas *> CANVAS_NORM;

    void Clean_Plots();
    void Clean_Pads();

public:
    Plot_Pool();
    ~Plot_Pool();

    void Add(const int nbinx, const double x_low, const double x_high, const char *var_name, const char *title,
             int *var);
    void Add(const int nbinx, const double x_low, const double x_high, const char *var_name, const char *title,
             double *var);

    int Get_N_VAR() const { return NVAR; }
    void Prepare_Plots();
    void Prepare_Pads();
    void Fill_Plots(int S_or_B, double weight = 1);  // 1 for signal, 0 for background
    void Dump_Plots(const char *dirname, const char *bkgname, const char *signame, const bool NORM_TO_ONE = true);
    double Get_Value_Var(int i);
};

#endif  //__PLOT_POOL_H__
