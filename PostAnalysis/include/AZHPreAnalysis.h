//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Sun Sep  5 16:48:05 2021 by ROOT version 6.22/08
// from TTree AZHPreAnalysis/Pre-Analysis for AZH System
// found on file: AZH_PreAna_gg_ztt_bkg_bkg_3l.root
//////////////////////////////////////////////////////////

#ifndef AZHPreAnalysis_h
#define AZHPreAnalysis_h

#include <TChain.h>
#include <TFile.h>
#include <TROOT.h>

// Header file for the classes stored in the TTree if any.

class AZHPreAnalysis {
public:
    TTree *fChain;   //! pointer to the analyzed TTree or TChain
    Int_t fCurrent;  //! current Tree number in a TChain

    // Fixed size dimensions of array or collections stored in the TTree if any.

    // Declaration of leaf types
    Char_t Proc_Full_Name[50];
    Char_t Proc_Abbr_Name[50];
    Int_t Proc_Cate_ID;
    Char_t Proc_Cate_Name[10];
    Int_t Decay_ID;
    Char_t Decay_Name[10];
    Double_t CS_without_Decay;
    Double_t CS_with_Decay;
    Char_t Proc_Param_Name[50];
    Int_t N_EVENTS;
    Double_t Weight;
    Int_t NELE_TOTAL;
    Int_t NMUON_TOTAL;
    Int_t NELE_ISO;
    Int_t NMUON_ISO;
    Double_t MOSSF_BEST;
    Int_t NJET_TOTAL;
    Int_t NBJET;
    Int_t NLIGHTJET;
    Double_t Px_nu;
    Double_t Py_nu;
    Double_t Pz_nu;
    Double_t mw_had;
    Double_t mt_rec_lep;
    Double_t mt_rec_had;
    Double_t mtt;
    Double_t mztt;
    Double_t HT;
    Double_t MET;

    // List of branches
    TBranch *b_Proc_Full_Name;    //!
    TBranch *b_Proc_Abbr_Name;    //!
    TBranch *b_Proc_Cate_ID;      //!
    TBranch *b_Proc_Cate_Name;    //!
    TBranch *b_Decay_ID;          //!
    TBranch *b_Decay_Name;        //!
    TBranch *b_CS_without_Decay;  //!
    TBranch *b_CS_with_Decay;     //!
    TBranch *b_Proc_Param_Name;   //!
    TBranch *b_N_EVENTS;          //!
    TBranch *b_Weight;            //!
    TBranch *b_NELE_TOTAL;        //!
    TBranch *b_NMUON_TOTAL;       //!
    TBranch *b_NELE_ISO;          //!
    TBranch *b_NMUON_ISO;         //!
    TBranch *b_MOSSF_BEST;        //!
    TBranch *b_NJET_TOTAL;        //!
    TBranch *b_NBJET;             //!
    TBranch *b_NLIGHTJET;         //!
    TBranch *b_Px_nu;             //!
    TBranch *b_Py_nu;             //!
    TBranch *b_Pz_nu;             //!
    TBranch *b_mw_had;            //!
    TBranch *b_mt_rec_lep;        //!
    TBranch *b_mt_rec_had;        //!
    TBranch *b_mtt;               //!
    TBranch *b_mztt;              //!
    TBranch *b_HT;                //!
    TBranch *b_MET;               //!

    AZHPreAnalysis(TTree *tree = 0);
    virtual ~AZHPreAnalysis();
    virtual Int_t Cut(Long64_t entry);
    virtual Int_t GetEntry(Long64_t entry);
    virtual Long64_t LoadTree(Long64_t entry);
    virtual void Init(TTree *tree);
    virtual void Loop();
    virtual Bool_t Notify();
    virtual void Show(Long64_t entry = -1);
};

#endif

#ifdef AZHPreAnalysis_cxx
AZHPreAnalysis::AZHPreAnalysis(TTree *tree) : fChain(0) {
    // if parameter tree is not specified (or zero), connect the file
    // used to generate this class and read the Tree.
    if (tree == 0) {
        TFile *f = (TFile *)gROOT->GetListOfFiles()->FindObject("AZH_PreAna_gg_ztt_bkg_bkg_3l.root");
        if (!f || !f->IsOpen()) {
            f = new TFile("AZH_PreAna_gg_ztt_bkg_bkg_3l.root");
        }
        f->GetObject("AZHPreAnalysis", tree);
    }
    Init(tree);
}

AZHPreAnalysis::~AZHPreAnalysis() {
    if (!fChain) return;
    delete fChain->GetCurrentFile();
}

Int_t AZHPreAnalysis::GetEntry(Long64_t entry) {
    // Read contents of entry.
    if (!fChain) return 0;
    return fChain->GetEntry(entry);
}
Long64_t AZHPreAnalysis::LoadTree(Long64_t entry) {
    // Set the environment to read one entry
    if (!fChain) return -5;
    Long64_t centry = fChain->LoadTree(entry);
    if (centry < 0) return centry;
    if (fChain->GetTreeNumber() != fCurrent) {
        fCurrent = fChain->GetTreeNumber();
        Notify();
    }
    return centry;
}

void AZHPreAnalysis::Init(TTree *tree) {
    // The Init() function is called when the selector needs to initialize
    // a new tree or chain. Typically here the branch addresses and branch
    // pointers of the tree will be set.
    // It is normally not necessary to make changes to the generated
    // code, but the routine can be extended by the user if needed.
    // Init() will be called many times when running on PROOF
    // (once per file to be processed).

    // Set branch addresses and branch pointers
    if (!tree) return;
    fChain = tree;
    fCurrent = -1;
    fChain->SetMakeClass(1);

    fChain->SetBranchAddress("Proc_Full_Name", Proc_Full_Name, &b_Proc_Full_Name);
    fChain->SetBranchAddress("Proc_Abbr_Name", Proc_Abbr_Name, &b_Proc_Abbr_Name);
    fChain->SetBranchAddress("Proc_Cate_ID", &Proc_Cate_ID, &b_Proc_Cate_ID);
    fChain->SetBranchAddress("Proc_Cate_Name", Proc_Cate_Name, &b_Proc_Cate_Name);
    fChain->SetBranchAddress("Decay_ID", &Decay_ID, &b_Decay_ID);
    fChain->SetBranchAddress("Decay_Name", Decay_Name, &b_Decay_Name);
    fChain->SetBranchAddress("CS_without_Decay", &CS_without_Decay, &b_CS_without_Decay);
    fChain->SetBranchAddress("CS_with_Decay", &CS_with_Decay, &b_CS_with_Decay);
    fChain->SetBranchAddress("Proc_Param_Name", Proc_Param_Name, &b_Proc_Param_Name);
    fChain->SetBranchAddress("N_EVENTS", &N_EVENTS, &b_N_EVENTS);
    fChain->SetBranchAddress("Weight", &Weight, &b_Weight);
    fChain->SetBranchAddress("NELE_TOTAL", &NELE_TOTAL, &b_NELE_TOTAL);
    fChain->SetBranchAddress("NMUON_TOTAL", &NMUON_TOTAL, &b_NMUON_TOTAL);
    fChain->SetBranchAddress("NELE_ISO", &NELE_ISO, &b_NELE_ISO);
    fChain->SetBranchAddress("NMUON_ISO", &NMUON_ISO, &b_NMUON_ISO);
    fChain->SetBranchAddress("MOSSF_BEST", &MOSSF_BEST, &b_MOSSF_BEST);
    fChain->SetBranchAddress("NJET_TOTAL", &NJET_TOTAL, &b_NJET_TOTAL);
    fChain->SetBranchAddress("NBJET", &NBJET, &b_NBJET);
    fChain->SetBranchAddress("NLIGHTJET", &NLIGHTJET, &b_NLIGHTJET);
    fChain->SetBranchAddress("Px_nu", &Px_nu, &b_Px_nu);
    fChain->SetBranchAddress("Py_nu", &Py_nu, &b_Py_nu);
    fChain->SetBranchAddress("Pz_nu", &Pz_nu, &b_Pz_nu);
    fChain->SetBranchAddress("mw_had", &mw_had, &b_mw_had);
    fChain->SetBranchAddress("mt_rec_lep", &mt_rec_lep, &b_mt_rec_lep);
    fChain->SetBranchAddress("mt_rec_had", &mt_rec_had, &b_mt_rec_had);
    fChain->SetBranchAddress("mtt", &mtt, &b_mtt);
    fChain->SetBranchAddress("mztt", &mztt, &b_mztt);
    fChain->SetBranchAddress("HT", &HT, &b_HT);
    fChain->SetBranchAddress("MET", &MET, &b_MET);
    Notify();
}

Bool_t AZHPreAnalysis::Notify() {
    // The Notify() function is called when a new file is opened. This
    // can be either for a new TTree in a TChain or when when a new TTree
    // is started when using PROOF. It is normally not necessary to make changes
    // to the generated code, but the routine can be extended by the
    // user if needed. The return value is currently not used.

    return kTRUE;
}

void AZHPreAnalysis::Show(Long64_t entry) {
    // Print contents of entry.
    // If entry is not specified, print current entry
    if (!fChain) return;
    fChain->Show(entry);
}
Int_t AZHPreAnalysis::Cut(Long64_t entry) {
    // This function may be called from Loop.
    // returns  1 if entry is accepted.
    // returns -1 otherwise.
    return 1;
}
#endif  // #ifdef AZHPreAnalysis_cxx
