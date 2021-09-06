#include "TError.h"
#include "TStyle.h"
#include "Utilities.h"
using namespace std;

void SetPlotStyle() {
    gErrorIgnoreLevel = kWarning;

    // General
    gStyle->SetOptStat(kFALSE);
    gStyle->SetCanvasBorderMode(0);

    // Frame
    // gStyle->SetFrameBorderMode(0);
    // gStyle->SetFrameFillColor(0);
    // gStyle->SetFrameLineColor(1);
    // gStyle->SetFrameLineWidth(2);

    // Lines
    // gStyle->SetLineWidth(1);
    // gStyle->SetFillColor(0);
    // gStyle->SetCanvasColor(0);

    // Margins
    gStyle->SetPadLeftMargin(0.15);
    gStyle->SetPadBottomMargin(0.15);
    gStyle->SetPadRightMargin(0.05);
    gStyle->SetPadTopMargin(0.05);

    // text
    gStyle->SetTextFont(132);
    gStyle->SetTextSize(0.06);

    // Axis Title
    gStyle->SetTitleFont(62, "XYZ");
    gStyle->SetTitleSize(0.055, "X");
    gStyle->SetTitleSize(0.055, "Y");
    gStyle->SetTitleSize(0.055, "Z");
    gStyle->SetTitleOffset(1.0, "X");
    gStyle->SetTitleOffset(1.3, "Y");
    gStyle->SetTitleOffset(1.0, "Z");
    // Axis Lable
    gStyle->SetLabelFont(42, "XYZ");
    gStyle->SetLabelSize(0.045, "X");
    gStyle->SetLabelSize(0.045, "Y");
    gStyle->SetLabelSize(0.045, "Z");
    gStyle->SetLabelOffset(0.012, "X");
    gStyle->SetLabelOffset(0.012, "Y");
}
