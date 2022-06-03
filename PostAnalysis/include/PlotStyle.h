#include "TStyle.h"

void SetPlotStyle(double rightmargin = 0.2) {
    // gErrorIgnoreLevel = kWarning;

    // ZAxis Color
    //  const Int_t NRGBs = 5;
    //  const Int_t NCont = 255;
    //  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
    //  Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
    //  Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
    //  Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
    //  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
    //  gStyle->SetNumberContours(NCont);

    // General
    gStyle->SetOptStat(kFALSE);
    gStyle->SetCanvasBorderMode(0);
    gStyle->SetLegendBorderSize(0);

    // Frame
    gStyle->SetFrameBorderMode(0);
    gStyle->SetFrameFillColor(0);
    gStyle->SetFrameLineColor(1);
    gStyle->SetFrameLineWidth(2);

    // Lines
    gStyle->SetLineWidth(1);
    gStyle->SetFillColor(0);
    gStyle->SetCanvasColor(0);

    // Margins
    gStyle->SetPadLeftMargin(0.14);
    gStyle->SetPadBottomMargin(0.14);
    gStyle->SetPadRightMargin(rightmargin);
    gStyle->SetPadTopMargin(0.08);

    // text
    gStyle->SetTextFont(132);
    gStyle->SetTextSize(0.06);

    // Axis Title
    gStyle->SetTitleFont(62, "XYZ");
    gStyle->SetTitleSize(0.055, "X");
    gStyle->SetTitleSize(0.055, "Y");
    gStyle->SetTitleSize(0.055, "Z");
    gStyle->SetTitleOffset(1.2, "X");
    gStyle->SetTitleOffset(1.2, "Y");
    gStyle->SetTitleOffset(1.4, "Z");
    // Axis Lable
    gStyle->SetLabelFont(42, "XYZ");
    gStyle->SetLabelSize(0.045, "X");
    gStyle->SetLabelSize(0.045, "Y");
    gStyle->SetLabelSize(0.045, "Z");
    gStyle->SetLabelOffset(0.012, "X");
    gStyle->SetLabelOffset(0.012, "Y");

    // Linestyle
    gStyle->SetLineStyleString(10, "");
    gStyle->SetLineStyleString(11, "30 30");
    gStyle->SetLineStyleString(12, "30 20 10 20");
    gStyle->SetLineStyleString(13, "10 10");
    gStyle->SetLineStyleString(14, "30 20 10 20 10 20 10 20");
}
