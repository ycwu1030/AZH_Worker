#ifndef __AZHSystem__H__
#define __AZHSystem__H__

#include <vector>

#include "Delphes.h"
#include "TLorentzVector.h"

#define PTLEPTRIGGER 10.0
#define PTJETTRIGGER 20.0

class AZHSystem {
public:
    AZHSystem(){};
    ~AZHSystem(){};

    bool Setup(Delphes *f);
    void Setup_Branches(TTree *t);

    int NELE_TOTAL;
    int NMUON_TOTAL;
    int NELE_ISO;
    int NMUON_ISO;
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
    bool Setup_Lepton_System(Delphes *f);
    bool Setup_Neutrino(Delphes *f);
    bool Setup_Jet_System(Delphes *f);
    std::vector<TLorentzVector> LEP_OSSF;
    std::vector<TLorentzVector> LEP_ISO;
    std::vector<TLorentzVector> BJETS;
    std::vector<TLorentzVector> LIGHTJETS;
};

#endif  //__AZHSystem__H__
