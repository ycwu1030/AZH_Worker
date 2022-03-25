#ifndef AZHSYSTEM_H_
#define AZHSYSTEM_H_

#include <vector>

#include "Delphes.h"
#include "TLorentzVector.h"

class AZHSystem {
public:
    AZHSystem(double PTLEP = 20, double PTJET = 30) {
        PTJETMIN = PTLEP;
        PTJETMIN = PTJET;
    };
    ~AZHSystem(){};

    bool Setup(Delphes *f);
    void Setup_Output_Branches(TTree *t_out);
    int NEVENT_NEGATIVE_WEIGHT;
    int NEVENT_POSITIVE_WEIGHT;

    int NELE_TOTAL;
    int NMUON_TOTAL;
    int NELE_ISO;
    int NMUON_ISO;
    int NLEP_ISO;
    double MOSSF_BEST;

    int NJET_TOTAL;
    int NBJET;
    int NLIGHTJET;
    double px_nu;
    double py_nu;
    double pz_nu;
    double mw_had;
    double mt_rec_lep;
    double mt_rec_had;
    double mtt;
    double mztt;
    double HT;
    double MET;

private:
    double PTLEPMIN;
    double PTJETMIN;
    bool Setup_Lepton_System(Delphes *f);
    bool Setup_Neutrino(Delphes *f);
    bool Setup_Jet_System(Delphes *f);
    std::vector<TLorentzVector> LEP_OSSF;
    std::vector<TLorentzVector> LEP_ISO;
    std::vector<TLorentzVector> BJETS;
    std::vector<TLorentzVector> LIGHTJETS;
};

#endif  // AZHSYSTEM_H_
