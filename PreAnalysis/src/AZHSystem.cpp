#include "AZHSystem.h"

#include <algorithm>
#include <vector>

#include "TLorentzVector.h"

using namespace std;
bool FVComparePT(const TLorentzVector& a, const TLorentzVector& b) { return a.Pt() > b.Pt(); }

bool AZHSystem::Setup_Lepton_System(Delphes* f) {
    if (f->Electron_size < 2 && f->Muon_size < 2) {
        // The system does not have OSSF system
        return false;
    }
    vector<TLorentzVector> LVLEP;
    vector<TLorentzVector> LVELE_P;
    vector<TLorentzVector> LVELE_M;
    vector<TLorentzVector> LVMUON_P;
    vector<TLorentzVector> LVMUON_M;
    TLorentzVector tmp;
    for (int i = 0; i < f->Electron_size; i++) {
        if (f->Electron_PT[i] >= PTLEPTRIGGER) {
            tmp.SetPtEtaPhiM(f->Electron_PT[i], f->Electron_Eta[i], f->Electron_Phi[i], 0.000511);
            if (f->Electron_Charge[i] == 1) {
                LVELE_P.push_back(tmp);
            } else {
                LVELE_M.push_back(tmp);
            }
        }
    }
    for (int i = 0; i < f->Muon_size; i++) {
        if (f->Muon_PT[i] >= PTLEPTRIGGER) {
            tmp.SetPtEtaPhiM(f->Muon_PT[i], f->Muon_Eta[i], f->Muon_Phi[i], 0.105);
            if (f->Muon_Charge[i] == 1) {
                LVMUON_P.push_back(tmp);
            } else {
                LVMUON_M.push_back(tmp);
            }
        }
    }
    int NE_P = LVELE_P.size();
    int NE_M = LVELE_M.size();
    int NM_P = LVMUON_P.size();
    int NM_M = LVMUON_M.size();
    NELE_TOTAL = NE_P + NE_M;
    NMUON_TOTAL = NM_P + NM_M;
    if (NE_P < 1 && NE_M < 1 && NM_P < 1 && NM_M < 1) {
        // not possible to provide OSSF lepton pair
        return false;
    }
    double MLL_E = -14000;
    int id_E_p = -1;
    int id_E_m = -1;
    for (int i = 0; i < NE_P; i++) {
        for (int j = 0; j < NE_M; j++) {
            double mtmp = (LVELE_P[i] + LVELE_M[j]).M();
            if (fabs(mtmp - 91.1776) < fabs(MLL_E - 91.1776)) {
                MLL_E = mtmp;
                id_E_p = i;
                id_E_m = j;
            }
        }
    }
    double MLL_M = -14000;
    int id_M_p = -1;
    int id_M_m = -1;
    for (int i = 0; i < NM_P; i++) {
        for (int j = 0; j < NM_M; j++) {
            double mtmp = (LVMUON_P[i], LVMUON_M[j]).M();
            if (fabs(mtmp - 91.1776) < fabs(MLL_M - 91.1776)) {
                MLL_M = mtmp;
                id_M_p = i;
                id_M_m = j;
            }
        }
    }

    LEP_OSSF.clear();
    if (fabs(MLL_E - 91.1776) < fabs(MLL_M - 91.1776)) {
        MOSSF_BEST = MLL_E;
        LEP_OSSF.push_back(LVELE_P[id_E_p]);
        LEP_OSSF.push_back(LVELE_M[id_E_m]);
        LVELE_P.erase(LVELE_P.begin() + id_E_p);
        LVELE_M.erase(LVELE_M.begin() + id_E_m);
    } else {
        MOSSF_BEST = MLL_M;
        LEP_OSSF.push_back(LVMUON_P[id_M_p]);
        LEP_OSSF.push_back(LVMUON_M[id_M_m]);
        LVMUON_P.erase(LVMUON_P.begin() + id_M_p);
        LVMUON_M.erase(LVMUON_M.begin() + id_M_m);
    }
    NELE_ISO = LVELE_P.size() + LVELE_M.size();
    NMUON_ISO = LVMUON_P.size() + LVMUON_M.size();

    LEP_ISO.clear();
    auto iter = LEP_ISO.begin();
    iter = LEP_ISO.insert(iter, LVELE_P.begin(), LVELE_P.end());
    iter = LEP_ISO.insert(iter, LVELE_M.begin(), LVELE_M.end());
    iter = LEP_ISO.insert(iter, LVMUON_P.begin(), LVMUON_P.end());
    iter = LEP_ISO.insert(iter, LVMUON_M.begin(), LVMUON_M.end());
    if (LEP_ISO.size() < 1) return false;
    sort(LEP_ISO.begin(), LEP_ISO.end(), FVComparePT);
    return true;
}

bool AZHSystem::Setup_Jet_System(Delphes* f) {
    BJETS.clear();
    LIGHTJETS.clear();
    TLorentzVector tmp;
    for (int i = 0; i < f->Jet_size; i++) {
        if (f->Jet_PT[i] >= PTJETTRIGGER) {
            tmp.SetPtEtaPhiM(f->Jet_PT[i], f->Jet_Eta[i], f->Jet_Phi[i], f->Jet_Mass[i]);
            if (f->Jet_BTag[i] & (1 << 0)) {
                BJETS.push_back(tmp);
            } else {
                LIGHTJETS.push_back(tmp);
            }
        }
    }
    sort(BJETS.begin(), BJETS.end(), FVComparePT);
    sort(LIGHTJETS.begin(), LIGHTJETS.end(), FVComparePT);
    NBJET = BJETS.size();
    NLIGHTJET = LIGHTJETS.size();
    NJET_TOTAL = NBJET + NLIGHTJET;
    if (NBJET < 2 || NLIGHTJET < 2) return false;  // We want two b-jets from two tops and two light jets from W decay
    return true;
}

bool Calculate_Neutrino_Momentum(const TLorentzVector& pl, const double& pvx, const double& pvy, TLorentzVector& pnu_p,
                                 TLorentzVector& pnu_m) {
    static const double MW = 80.385;
    double plx = pl.Px();
    double ply = pl.Py();
    double plz = pl.Pz();
    double ml = pl.M();
    double El = pl.E();
    // The z-component of neutrino momentum satisfies:
    // (4*plz^2-2*El^2)*pz^2 +
    // 4*plz*(MW^2-ml^2+2*plx*pvx+2*ply*pvy)*pz+(MW^2-ml^2+2*plx*pvx+2*ply*pvy)^2-2*El^2(pvx^2+pvy^2) = 0;
    double a = 4 * plz * plz - 2 * El * El;
    double b = 4 * plz * (MW * MW - ml * ml + 2 * plx * pvx + 2 * ply * pvy);
    double c = pow(MW * MW - ml * ml + 2 * plx * pvx + 2 * ply * pvy, 2) - 2 * El * El * (pvx * pvx + pvy * pvy);
    double Del2 = b * b - 4 * a * c;
    if (Del2 < 0) {
        return false;
    }
    double pzp = (-b + sqrt(Del2)) / 2.0 / a;
    double pzm = (-b - sqrt(Del2)) / 2.0 / a;
    pnu_p.SetXYZM(pvx, pvy, pzp, 0);
    pnu_m.SetXYZM(pvx, pvy, pzm, 0);
    return true;
}

bool AZHSystem::Setup_Neutrino(Delphes* f) {
    static const double MT = 172.5;
    TLorentzVector pMET;
    MET = f->MissingET_MET[0];
    pMET.SetPtEtaPhiM(f->MissingET_MET[0], f->MissingET_Eta[0], f->MissingET_Phi[0], 0);
    double pvx = pMET.Px();
    double pvy = pMET.Py();
    // * Now we only consider the hardest Lepton and the first two B-jets
    TLorentzVector pnup;
    TLorentzVector pnum;
    bool goodnu = Calculate_Neutrino_Momentum(LEP_ISO[0], pvx, pvy, pnup, pnum);
    if (!goodnu) return false;
    double MT_tmp_p = -14000;
    double MT_tmp_m = -14000;
    for (int i = 0; i < NBJET; i++) {
        double tmp = (LEP_ISO[0] + pnup + BJETS[i]).M();
        if (fabs(tmp - MT) < fabs(MT_tmp_p - MT)) {
            MT_tmp_p = tmp;
        }
        tmp = (LEP_ISO[0] + pnum + BJETS[i]).M();
        if (fabs(tmp - MT) < fabs(MT_tmp_m - MT)) {
            MT_tmp_m = tmp;
        }
    }
    TLorentzVector pnu;
    if (fabs(MT_tmp_p - MT) < fabs(MT_tmp_m - MT)) {
        pnu = pnup;
    } else {
        pnu = pnum;
    }
    mtt = (pnu + LEP_ISO[0] + BJETS[0] + BJETS[1] + LIGHTJETS[0] + LIGHTJETS[1]).M();
    mztt = (pnu + LEP_ISO[0] + BJETS[0] + BJETS[1] + LIGHTJETS[0] + LIGHTJETS[1] + LEP_OSSF[0] + LEP_OSSF[1]).M();
    px_nu = pnu.Px();
    py_nu = pnu.Py();
    pz_nu = pnu.Pz();
    return true;
}

bool AZHSystem::Setup(Delphes* f) {
    bool good = true;
    good = Setup_Lepton_System(f);
    if (!good) return good;
    good = Setup_Jet_System(f);
    if (!good) return good;
    good = Setup_Neutrino(f);
    if (!good) return good;
    HT = f->ScalarHT_HT[0];
    return good;
}
