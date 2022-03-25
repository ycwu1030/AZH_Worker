#include "AZHSystem.h"

#include <algorithm>
#include <iostream>
#include <vector>

#include "TLorentzVector.h"

using namespace std;

bool FVComparePT(const TLorentzVector& a, const TLorentzVector& b) { return a.Pt() > b.Pt(); }

bool AZHSystem::Setup_Lepton_System(Delphes* f) {
    if (f->Electron_size < 2 && f->Muon_size < 2) {
        return false;
    }
    vector<TLorentzVector> LVLEP;
    vector<TLorentzVector> LVELE_P;
    vector<TLorentzVector> LVELE_M;
    vector<TLorentzVector> LVMUON_P;
    vector<TLorentzVector> LVMUON_M;
    TLorentzVector tmp;
    for (int i = 0; i < f->Electron_size; i++) {
        if (f->Electron_PT[i] < PTLEPMIN) continue;
        tmp.SetPtEtaPhiM(f->Electron_PT[i], f->Electron_Eta[i], f->Electron_Phi[i], 0.000511);
        if (f->Electron_Charge[i] == 1) {
            LVELE_P.push_back(tmp);
        } else {
            LVELE_M.push_back(tmp);
        }
    }

    for (int i = 0; i < f->Muon_size; i++) {
        if (f->Muon_PT[i] < PTLEPMIN) continue;
        tmp.SetPtEtaPhiM(f->Muon_PT[i], f->Muon_Eta[i], f->Muon_Phi[i], 0.105);
        if (f->Muon_Charge[i] == 1) {
            LVMUON_P.push_back(tmp);
        } else {
            LVMUON_M.push_back(tmp);
        }
    }

    int NE_P = LVELE_P.size();
    int NE_M = LVELE_M.size();
    int NM_P = LVMUON_P.size();
    int NM_M = LVMUON_M.size();

    NELE_TOTAL = NE_P + NE_M;
    NMUON_TOTAL = NM_P + NM_M;
    if ((NE_P < 1 || NE_M < 1) && (NM_P < 1 || NM_M < 1)) {
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
            double mtmp = (LVMUON_P[i] + LVMUON_M[j]).M();
            if (fabs(mtmp - 91.1776) < fabs(MLL_M - 91.1776)) {
                MLL_M = mtmp;
                id_M_p = i;
                id_M_m = j;
            }
        }
    }

    LEP_OSSF.clear();
    double delta_m_e = fabs(MLL_E - 91.1776);
    double delta_m_m = fabs(MLL_M - 91.1776);
    if (delta_m_e < delta_m_m) {
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
    vector<TLorentzVector>::iterator iter = LEP_ISO.begin();
    LEP_ISO.insert(iter, LVELE_P.begin(), LVELE_P.end());
    iter = LEP_ISO.begin();
    LEP_ISO.insert(iter, LVELE_M.begin(), LVELE_M.end());
    iter = LEP_ISO.begin();
    LEP_ISO.insert(iter, LVMUON_P.begin(), LVMUON_P.end());
    iter = LEP_ISO.begin();
    LEP_ISO.insert(iter, LVMUON_M.begin(), LVMUON_M.end());
    if (LEP_ISO.size() < 1) return false;
    sort(LEP_ISO.begin(), LEP_ISO.end(), FVComparePT);
    NLEP_ISO = LEP_ISO.size();
    return true;
}

bool AZHSystem::Setup_Jet_System(Delphes* f) {
    BJETS.clear();
    LIGHTJETS.clear();
    TLorentzVector tmp;
    for (int i = 0; i < f->Jet_size; i++) {
        if (f->Jet_PT[i] >= PTJETMIN) {
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
    // (4*plz^2-4*El^2)*pz^2 +
    // 4*plz*(MW^2-ml^2+2*plx*pvx+2*ply*pvy)*pz+(MW^2-ml^2+2*plx*pvx+2*ply*pvy)^2-4*El^2(pvx^2+pvy^2) = 0;
    double a = 4 * plz * plz - 4 * El * El;
    double b = 4 * plz * (MW * MW - ml * ml + 2 * plx * pvx + 2 * ply * pvy);
    double c = pow(MW * MW - ml * ml + 2 * plx * pvx + 2 * ply * pvy, 2) - 4 * El * El * (pvx * pvx + pvy * pvy);
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

double AZHSystem_To_Be_Minimize(const TLorentzVector& pb_had, const TLorentzVector& pj1, const TLorentzVector& pj2,
                                const TLorentzVector& pb_lep, const TLorentzVector& pl, const TLorentzVector& pv) {
    static const double MT = 172.5;
    static const double MW = 80.385;
    double res = 0;

    // * First the hadronic W
    double mw_tmp = (pj1 + pj2).M();
    res += pow(mw_tmp - MW, 2);

    // * Then the hadronic top
    double mt_had = (pj1 + pj2 + pb_had).M();
    res += pow(mt_had - MT, 2);

    // * Then the leptonic top
    double mt_lep = (pl + pv + pb_lep).M();
    res += pow(mt_lep - MT, 2);

    return res;
}

bool AZHSystem::Setup_Neutrino(Delphes* f) {
    static const double MT = 172.5;
    static const double MW = 80.385;
    TLorentzVector pMET;
    MET = f->MissingET_MET[0];
    pMET.SetPtEtaPhiM(f->MissingET_MET[0], f->MissingET_Eta[0], f->MissingET_Phi[0], 0);
    double pvx = pMET.Px();
    double pvy = pMET.Py();

    double best_delta_mass2 = -1;
    int id_b_had;
    int id_b_lep;
    int id_w_j1;
    int id_w_j2;
    int id_lep;
    TLorentzVector pnu_p;
    TLorentzVector pnu_m;
    TLorentzVector pnu;

    for (int i = 0; i < NBJET; i++) {
        // * used for hadronic top
        for (int j = 0; j < NBJET; j++) {
            // * used for leptonic top
            if (j == i) continue;
            for (int k = 0; k < NLIGHTJET; k++) {
                for (int l = k + 1; l < NLIGHTJET; l++) {
                    // * two jet from hadronic w
                    for (int ll = 0; ll < NLEP_ISO; ll++) {
                        bool goodnu = Calculate_Neutrino_Momentum(LEP_ISO[ll], pvx, pvy, pnu_p, pnu_m);
                        if (!goodnu) continue;
                        double tmp_p = AZHSystem_To_Be_Minimize(BJETS[i], LIGHTJETS[k], LIGHTJETS[l], BJETS[j],
                                                                LEP_ISO[ll], pnu_p);
                        double tmp_m = AZHSystem_To_Be_Minimize(BJETS[i], LIGHTJETS[k], LIGHTJETS[l], BJETS[j],
                                                                LEP_ISO[ll], pnu_m);
                        if (best_delta_mass2 < 0 || tmp_p < best_delta_mass2) {
                            best_delta_mass2 = tmp_p;
                            id_b_had = i;
                            id_b_lep = j;
                            id_w_j1 = k;
                            id_w_j2 = l;
                            id_lep = ll;
                            pnu = pnu_p;
                        }
                        if (best_delta_mass2 < 0 || tmp_m < best_delta_mass2) {
                            best_delta_mass2 = tmp_m;
                            id_b_had = i;
                            id_b_lep = j;
                            id_w_j1 = k;
                            id_w_j2 = l;
                            id_lep = ll;
                            pnu = pnu_m;
                        }
                    }
                }
            }
        }
    }

    if (best_delta_mass2 < 0) return false;

    mw_had = (LIGHTJETS[id_w_j1] + LIGHTJETS[id_w_j2]).M();
    mt_rec_had = (BJETS[id_b_had] + LIGHTJETS[id_w_j1] + LIGHTJETS[id_w_j2]).M();
    mt_rec_lep = (BJETS[id_b_lep] + LEP_ISO[id_lep] + pnu).M();
    px_nu = pnu.Px();
    py_nu = pnu.Py();
    pz_nu = pnu.Pz();

    mtt = (pnu + BJETS[id_b_lep] + LEP_ISO[id_lep] + BJETS[id_b_had] + LIGHTJETS[id_w_j1] + LIGHTJETS[id_w_j2]).M();
    mztt = (pnu + BJETS[id_b_lep] + LEP_ISO[id_lep] + BJETS[id_b_had] + LIGHTJETS[id_w_j1] + LIGHTJETS[id_w_j2] +
            LEP_OSSF[0] + LEP_OSSF[1])
               .M();
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

void AZHSystem::Setup_Output_Branches(TTree* t) {
    t->Branch("NELE_TOTAL", &NELE_TOTAL, "NELE_TOTAL/I");
    t->Branch("NMUON_TOTAL", &NMUON_TOTAL, "NMUON_TOTAL/I");
    t->Branch("NELE_ISO", &NELE_ISO, "NELE_ISO/I");
    t->Branch("NMUON_ISO", &NMUON_ISO, "NMUON_ISO/I");
    t->Branch("MOSSF_BEST", &MOSSF_BEST, "MOSSF_BEST/D");
    t->Branch("NJET_TOTAL", &NJET_TOTAL, "NJET_TOTAL/I");
    t->Branch("NBJET", &NBJET, "NBJET/I");
    t->Branch("NLIGHTJET", &NLIGHTJET, "NLIGHTJET/I");
    t->Branch("Px_nu", &px_nu, "Px_nu/D");
    t->Branch("Py_nu", &py_nu, "Py_nu/D");
    t->Branch("Pz_nu", &pz_nu, "Pz_nu/D");
    t->Branch("mw_had", &mw_had, "mw_had/D");
    t->Branch("mt_rec_lep", &mt_rec_lep, "mt_rec_lep/D");
    t->Branch("mt_rec_had", &mt_rec_had, "mt_rec_had/D");
    t->Branch("mtt", &mtt, "mtt/D");
    t->Branch("mztt", &mztt, "mztt/D");
    t->Branch("HT", &HT, "HT/D");
    t->Branch("MET", &MET, "MET/D");
}
