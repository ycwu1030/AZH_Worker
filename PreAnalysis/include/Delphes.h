//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Mar 23 21:06:57 2022 by ROOT version 6.22/06
// from TTree Delphes/Analysis tree
// found on file: delphes_gg_ztt_Type-I_ggF_box_202202201812_HA0x005_HH0x005_0_tb10_3l_20220318_095649.root
//////////////////////////////////////////////////////////

#ifndef Delphes_h
#define Delphes_h

#include <TChain.h>
#include <TFile.h>
#include <TROOT.h>

// Header file for the classes stored in the TTree if any.
#include "TClonesArray.h"
#include "TLorentzVector.h"
#include "TObject.h"
#include "TRef.h"
#include "TRefArray.h"

class Delphes {
public:
    TTree *fChain;   //! pointer to the analyzed TTree or TChain
    Int_t fCurrent;  //! current Tree number in a TChain

    // Fixed size dimensions of array or collections stored in the TTree if any.
    static constexpr Int_t kMaxEvent = 1;
    static constexpr Int_t kMaxWeight = 2;
    static constexpr Int_t kMaxJet = 20;
    static constexpr Int_t kMaxElectron = 10;
    static constexpr Int_t kMaxPhoton = 10;
    static constexpr Int_t kMaxMuon = 10;
    static constexpr Int_t kMaxMissingET = 1;
    static constexpr Int_t kMaxScalarHT = 1;

    // Declaration of leaf types
    Int_t Event_;
    UInt_t Event_fUniqueID[kMaxEvent];           //[Event_]
    UInt_t Event_fBits[kMaxEvent];               //[Event_]
    Long64_t Event_Number[kMaxEvent];            //[Event_]
    Float_t Event_ReadTime[kMaxEvent];           //[Event_]
    Float_t Event_ProcTime[kMaxEvent];           //[Event_]
    Int_t Event_ProcessID[kMaxEvent];            //[Event_]
    Int_t Event_MPI[kMaxEvent];                  //[Event_]
    Float_t Event_Weight[kMaxEvent];             //[Event_]
    Float_t Event_CrossSection[kMaxEvent];       //[Event_]
    Float_t Event_CrossSectionError[kMaxEvent];  //[Event_]
    Float_t Event_Scale[kMaxEvent];              //[Event_]
    Float_t Event_AlphaQED[kMaxEvent];           //[Event_]
    Float_t Event_AlphaQCD[kMaxEvent];           //[Event_]
    Int_t Event_ID1[kMaxEvent];                  //[Event_]
    Int_t Event_ID2[kMaxEvent];                  //[Event_]
    Float_t Event_X1[kMaxEvent];                 //[Event_]
    Float_t Event_X2[kMaxEvent];                 //[Event_]
    Float_t Event_ScalePDF[kMaxEvent];           //[Event_]
    Float_t Event_PDF1[kMaxEvent];               //[Event_]
    Float_t Event_PDF2[kMaxEvent];               //[Event_]
    Int_t Event_size;
    Int_t Weight_;
    UInt_t Weight_fUniqueID[kMaxWeight];  //[Weight_]
    UInt_t Weight_fBits[kMaxWeight];      //[Weight_]
    Float_t Weight_Weight[kMaxWeight];    //[Weight_]
    Int_t Weight_size;
    Int_t Jet_;
    UInt_t Jet_fUniqueID[kMaxJet];               //[Jet_]
    UInt_t Jet_fBits[kMaxJet];                   //[Jet_]
    Float_t Jet_PT[kMaxJet];                     //[Jet_]
    Float_t Jet_Eta[kMaxJet];                    //[Jet_]
    Float_t Jet_Phi[kMaxJet];                    //[Jet_]
    Float_t Jet_T[kMaxJet];                      //[Jet_]
    Float_t Jet_Mass[kMaxJet];                   //[Jet_]
    Float_t Jet_DeltaEta[kMaxJet];               //[Jet_]
    Float_t Jet_DeltaPhi[kMaxJet];               //[Jet_]
    UInt_t Jet_Flavor[kMaxJet];                  //[Jet_]
    UInt_t Jet_FlavorAlgo[kMaxJet];              //[Jet_]
    UInt_t Jet_FlavorPhys[kMaxJet];              //[Jet_]
    UInt_t Jet_BTag[kMaxJet];                    //[Jet_]
    UInt_t Jet_BTagAlgo[kMaxJet];                //[Jet_]
    UInt_t Jet_BTagPhys[kMaxJet];                //[Jet_]
    UInt_t Jet_TauTag[kMaxJet];                  //[Jet_]
    Float_t Jet_TauWeight[kMaxJet];              //[Jet_]
    Int_t Jet_Charge[kMaxJet];                   //[Jet_]
    Float_t Jet_EhadOverEem[kMaxJet];            //[Jet_]
    Int_t Jet_NCharged[kMaxJet];                 //[Jet_]
    Int_t Jet_NNeutrals[kMaxJet];                //[Jet_]
    Float_t Jet_NeutralEnergyFraction[kMaxJet];  //[Jet_]
    Float_t Jet_ChargedEnergyFraction[kMaxJet];  //[Jet_]
    Float_t Jet_Beta[kMaxJet];                   //[Jet_]
    Float_t Jet_BetaStar[kMaxJet];               //[Jet_]
    Float_t Jet_MeanSqDeltaR[kMaxJet];           //[Jet_]
    Float_t Jet_PTD[kMaxJet];                    //[Jet_]
    Float_t Jet_FracPt[kMaxJet][5];              //[Jet_]
    Float_t Jet_Tau[kMaxJet][5];                 //[Jet_]
    TLorentzVector Jet_SoftDroppedJet[kMaxJet];
    TLorentzVector Jet_SoftDroppedSubJet1[kMaxJet];
    TLorentzVector Jet_SoftDroppedSubJet2[kMaxJet];
    TLorentzVector Jet_TrimmedP4[5][kMaxJet];
    TLorentzVector Jet_PrunedP4[5][kMaxJet];
    TLorentzVector Jet_SoftDroppedP4[5][kMaxJet];
    Int_t Jet_NSubJetsTrimmed[kMaxJet];      //[Jet_]
    Int_t Jet_NSubJetsPruned[kMaxJet];       //[Jet_]
    Int_t Jet_NSubJetsSoftDropped[kMaxJet];  //[Jet_]
    Double_t Jet_ExclYmerge23[kMaxJet];      //[Jet_]
    Double_t Jet_ExclYmerge34[kMaxJet];      //[Jet_]
    Double_t Jet_ExclYmerge45[kMaxJet];      //[Jet_]
    Double_t Jet_ExclYmerge56[kMaxJet];      //[Jet_]
    TRefArray Jet_Constituents[kMaxJet];
    TRefArray Jet_Particles[kMaxJet];
    TLorentzVector Jet_Area[kMaxJet];
    Int_t Jet_size;
    Int_t Electron_;
    UInt_t Electron_fUniqueID[kMaxElectron];     //[Electron_]
    UInt_t Electron_fBits[kMaxElectron];         //[Electron_]
    Float_t Electron_PT[kMaxElectron];           //[Electron_]
    Float_t Electron_Eta[kMaxElectron];          //[Electron_]
    Float_t Electron_Phi[kMaxElectron];          //[Electron_]
    Float_t Electron_T[kMaxElectron];            //[Electron_]
    Int_t Electron_Charge[kMaxElectron];         //[Electron_]
    Float_t Electron_EhadOverEem[kMaxElectron];  //[Electron_]
    TRef Electron_Particle[kMaxElectron];
    Float_t Electron_IsolationVar[kMaxElectron];         //[Electron_]
    Float_t Electron_IsolationVarRhoCorr[kMaxElectron];  //[Electron_]
    Float_t Electron_SumPtCharged[kMaxElectron];         //[Electron_]
    Float_t Electron_SumPtNeutral[kMaxElectron];         //[Electron_]
    Float_t Electron_SumPtChargedPU[kMaxElectron];       //[Electron_]
    Float_t Electron_SumPt[kMaxElectron];                //[Electron_]
    Float_t Electron_D0[kMaxElectron];                   //[Electron_]
    Float_t Electron_DZ[kMaxElectron];                   //[Electron_]
    Float_t Electron_ErrorD0[kMaxElectron];              //[Electron_]
    Float_t Electron_ErrorDZ[kMaxElectron];              //[Electron_]
    Int_t Electron_size;
    Int_t Photon_;
    UInt_t Photon_fUniqueID[kMaxPhoton];     //[Photon_]
    UInt_t Photon_fBits[kMaxPhoton];         //[Photon_]
    Float_t Photon_PT[kMaxPhoton];           //[Photon_]
    Float_t Photon_Eta[kMaxPhoton];          //[Photon_]
    Float_t Photon_Phi[kMaxPhoton];          //[Photon_]
    Float_t Photon_E[kMaxPhoton];            //[Photon_]
    Float_t Photon_T[kMaxPhoton];            //[Photon_]
    Float_t Photon_EhadOverEem[kMaxPhoton];  //[Photon_]
    TRefArray Photon_Particles[kMaxPhoton];
    Float_t Photon_IsolationVar[kMaxPhoton];         //[Photon_]
    Float_t Photon_IsolationVarRhoCorr[kMaxPhoton];  //[Photon_]
    Float_t Photon_SumPtCharged[kMaxPhoton];         //[Photon_]
    Float_t Photon_SumPtNeutral[kMaxPhoton];         //[Photon_]
    Float_t Photon_SumPtChargedPU[kMaxPhoton];       //[Photon_]
    Float_t Photon_SumPt[kMaxPhoton];                //[Photon_]
    Int_t Photon_Status[kMaxPhoton];                 //[Photon_]
    Int_t Photon_size;
    Int_t Muon_;
    UInt_t Muon_fUniqueID[kMaxMuon];  //[Muon_]
    UInt_t Muon_fBits[kMaxMuon];      //[Muon_]
    Float_t Muon_PT[kMaxMuon];        //[Muon_]
    Float_t Muon_Eta[kMaxMuon];       //[Muon_]
    Float_t Muon_Phi[kMaxMuon];       //[Muon_]
    Float_t Muon_T[kMaxMuon];         //[Muon_]
    Int_t Muon_Charge[kMaxMuon];      //[Muon_]
    TRef Muon_Particle[kMaxMuon];
    Float_t Muon_IsolationVar[kMaxMuon];         //[Muon_]
    Float_t Muon_IsolationVarRhoCorr[kMaxMuon];  //[Muon_]
    Float_t Muon_SumPtCharged[kMaxMuon];         //[Muon_]
    Float_t Muon_SumPtNeutral[kMaxMuon];         //[Muon_]
    Float_t Muon_SumPtChargedPU[kMaxMuon];       //[Muon_]
    Float_t Muon_SumPt[kMaxMuon];                //[Muon_]
    Float_t Muon_D0[kMaxMuon];                   //[Muon_]
    Float_t Muon_DZ[kMaxMuon];                   //[Muon_]
    Float_t Muon_ErrorD0[kMaxMuon];              //[Muon_]
    Float_t Muon_ErrorDZ[kMaxMuon];              //[Muon_]
    Int_t Muon_size;
    Int_t MissingET_;
    UInt_t MissingET_fUniqueID[kMaxMissingET];  //[MissingET_]
    UInt_t MissingET_fBits[kMaxMissingET];      //[MissingET_]
    Float_t MissingET_MET[kMaxMissingET];       //[MissingET_]
    Float_t MissingET_Eta[kMaxMissingET];       //[MissingET_]
    Float_t MissingET_Phi[kMaxMissingET];       //[MissingET_]
    Int_t MissingET_size;
    Int_t ScalarHT_;
    UInt_t ScalarHT_fUniqueID[kMaxScalarHT];  //[ScalarHT_]
    UInt_t ScalarHT_fBits[kMaxScalarHT];      //[ScalarHT_]
    Float_t ScalarHT_HT[kMaxScalarHT];        //[ScalarHT_]
    Int_t ScalarHT_size;

    // List of branches
    TBranch *b_Event_;                        //!
    TBranch *b_Event_fUniqueID;               //!
    TBranch *b_Event_fBits;                   //!
    TBranch *b_Event_Number;                  //!
    TBranch *b_Event_ReadTime;                //!
    TBranch *b_Event_ProcTime;                //!
    TBranch *b_Event_ProcessID;               //!
    TBranch *b_Event_MPI;                     //!
    TBranch *b_Event_Weight;                  //!
    TBranch *b_Event_CrossSection;            //!
    TBranch *b_Event_CrossSectionError;       //!
    TBranch *b_Event_Scale;                   //!
    TBranch *b_Event_AlphaQED;                //!
    TBranch *b_Event_AlphaQCD;                //!
    TBranch *b_Event_ID1;                     //!
    TBranch *b_Event_ID2;                     //!
    TBranch *b_Event_X1;                      //!
    TBranch *b_Event_X2;                      //!
    TBranch *b_Event_ScalePDF;                //!
    TBranch *b_Event_PDF1;                    //!
    TBranch *b_Event_PDF2;                    //!
    TBranch *b_Event_size;                    //!
    TBranch *b_Weight_;                       //!
    TBranch *b_Weight_fUniqueID;              //!
    TBranch *b_Weight_fBits;                  //!
    TBranch *b_Weight_Weight;                 //!
    TBranch *b_Weight_size;                   //!
    TBranch *b_Jet_;                          //!
    TBranch *b_Jet_fUniqueID;                 //!
    TBranch *b_Jet_fBits;                     //!
    TBranch *b_Jet_PT;                        //!
    TBranch *b_Jet_Eta;                       //!
    TBranch *b_Jet_Phi;                       //!
    TBranch *b_Jet_T;                         //!
    TBranch *b_Jet_Mass;                      //!
    TBranch *b_Jet_DeltaEta;                  //!
    TBranch *b_Jet_DeltaPhi;                  //!
    TBranch *b_Jet_Flavor;                    //!
    TBranch *b_Jet_FlavorAlgo;                //!
    TBranch *b_Jet_FlavorPhys;                //!
    TBranch *b_Jet_BTag;                      //!
    TBranch *b_Jet_BTagAlgo;                  //!
    TBranch *b_Jet_BTagPhys;                  //!
    TBranch *b_Jet_TauTag;                    //!
    TBranch *b_Jet_TauWeight;                 //!
    TBranch *b_Jet_Charge;                    //!
    TBranch *b_Jet_EhadOverEem;               //!
    TBranch *b_Jet_NCharged;                  //!
    TBranch *b_Jet_NNeutrals;                 //!
    TBranch *b_Jet_NeutralEnergyFraction;     //!
    TBranch *b_Jet_ChargedEnergyFraction;     //!
    TBranch *b_Jet_Beta;                      //!
    TBranch *b_Jet_BetaStar;                  //!
    TBranch *b_Jet_MeanSqDeltaR;              //!
    TBranch *b_Jet_PTD;                       //!
    TBranch *b_Jet_FracPt;                    //!
    TBranch *b_Jet_Tau;                       //!
    TBranch *b_Jet_SoftDroppedJet;            //!
    TBranch *b_Jet_SoftDroppedSubJet1;        //!
    TBranch *b_Jet_SoftDroppedSubJet2;        //!
    TBranch *b_Jet_TrimmedP4;                 //!
    TBranch *b_Jet_PrunedP4;                  //!
    TBranch *b_Jet_SoftDroppedP4;             //!
    TBranch *b_Jet_NSubJetsTrimmed;           //!
    TBranch *b_Jet_NSubJetsPruned;            //!
    TBranch *b_Jet_NSubJetsSoftDropped;       //!
    TBranch *b_Jet_ExclYmerge23;              //!
    TBranch *b_Jet_ExclYmerge34;              //!
    TBranch *b_Jet_ExclYmerge45;              //!
    TBranch *b_Jet_ExclYmerge56;              //!
    TBranch *b_Jet_Constituents;              //!
    TBranch *b_Jet_Particles;                 //!
    TBranch *b_Jet_Area;                      //!
    TBranch *b_Jet_size;                      //!
    TBranch *b_Electron_;                     //!
    TBranch *b_Electron_fUniqueID;            //!
    TBranch *b_Electron_fBits;                //!
    TBranch *b_Electron_PT;                   //!
    TBranch *b_Electron_Eta;                  //!
    TBranch *b_Electron_Phi;                  //!
    TBranch *b_Electron_T;                    //!
    TBranch *b_Electron_Charge;               //!
    TBranch *b_Electron_EhadOverEem;          //!
    TBranch *b_Electron_Particle;             //!
    TBranch *b_Electron_IsolationVar;         //!
    TBranch *b_Electron_IsolationVarRhoCorr;  //!
    TBranch *b_Electron_SumPtCharged;         //!
    TBranch *b_Electron_SumPtNeutral;         //!
    TBranch *b_Electron_SumPtChargedPU;       //!
    TBranch *b_Electron_SumPt;                //!
    TBranch *b_Electron_D0;                   //!
    TBranch *b_Electron_DZ;                   //!
    TBranch *b_Electron_ErrorD0;              //!
    TBranch *b_Electron_ErrorDZ;              //!
    TBranch *b_Electron_size;                 //!
    TBranch *b_Photon_;                       //!
    TBranch *b_Photon_fUniqueID;              //!
    TBranch *b_Photon_fBits;                  //!
    TBranch *b_Photon_PT;                     //!
    TBranch *b_Photon_Eta;                    //!
    TBranch *b_Photon_Phi;                    //!
    TBranch *b_Photon_E;                      //!
    TBranch *b_Photon_T;                      //!
    TBranch *b_Photon_EhadOverEem;            //!
    TBranch *b_Photon_Particles;              //!
    TBranch *b_Photon_IsolationVar;           //!
    TBranch *b_Photon_IsolationVarRhoCorr;    //!
    TBranch *b_Photon_SumPtCharged;           //!
    TBranch *b_Photon_SumPtNeutral;           //!
    TBranch *b_Photon_SumPtChargedPU;         //!
    TBranch *b_Photon_SumPt;                  //!
    TBranch *b_Photon_Status;                 //!
    TBranch *b_Photon_size;                   //!
    TBranch *b_Muon_;                         //!
    TBranch *b_Muon_fUniqueID;                //!
    TBranch *b_Muon_fBits;                    //!
    TBranch *b_Muon_PT;                       //!
    TBranch *b_Muon_Eta;                      //!
    TBranch *b_Muon_Phi;                      //!
    TBranch *b_Muon_T;                        //!
    TBranch *b_Muon_Charge;                   //!
    TBranch *b_Muon_Particle;                 //!
    TBranch *b_Muon_IsolationVar;             //!
    TBranch *b_Muon_IsolationVarRhoCorr;      //!
    TBranch *b_Muon_SumPtCharged;             //!
    TBranch *b_Muon_SumPtNeutral;             //!
    TBranch *b_Muon_SumPtChargedPU;           //!
    TBranch *b_Muon_SumPt;                    //!
    TBranch *b_Muon_D0;                       //!
    TBranch *b_Muon_DZ;                       //!
    TBranch *b_Muon_ErrorD0;                  //!
    TBranch *b_Muon_ErrorDZ;                  //!
    TBranch *b_Muon_size;                     //!
    TBranch *b_MissingET_;                    //!
    TBranch *b_MissingET_fUniqueID;           //!
    TBranch *b_MissingET_fBits;               //!
    TBranch *b_MissingET_MET;                 //!
    TBranch *b_MissingET_Eta;                 //!
    TBranch *b_MissingET_Phi;                 //!
    TBranch *b_MissingET_size;                //!
    TBranch *b_ScalarHT_;                     //!
    TBranch *b_ScalarHT_fUniqueID;            //!
    TBranch *b_ScalarHT_fBits;                //!
    TBranch *b_ScalarHT_HT;                   //!
    TBranch *b_ScalarHT_size;                 //!

    Delphes(TTree *tree = 0);
    virtual ~Delphes();
    virtual Int_t Cut(Long64_t entry);
    virtual Int_t GetEntry(Long64_t entry);
    virtual Long64_t LoadTree(Long64_t entry);
    virtual void Init(TTree *tree);
    virtual void Loop();
    virtual Bool_t Notify();
    virtual void Show(Long64_t entry = -1);
};

#endif

#ifdef Delphes_cxx
Delphes::Delphes(TTree *tree) : fChain(0) {
    // if parameter tree is not specified (or zero), connect the file
    // used to generate this class and read the Tree.
    if (tree == 0) {
        TFile *f = (TFile *)gROOT->GetListOfFiles()->FindObject(
            "delphes_gg_ztt_Type-I_ggF_box_202202201812_HA0x005_HH0x005_0_tb10_3l_20220318_095649.root");
        if (!f || !f->IsOpen()) {
            f = new TFile("delphes_gg_ztt_Type-I_ggF_box_202202201812_HA0x005_HH0x005_0_tb10_3l_20220318_095649.root");
        }
        f->GetObject("Delphes", tree);
    }
    Init(tree);
}

Delphes::~Delphes() {
    if (!fChain) return;
    delete fChain->GetCurrentFile();
}

Int_t Delphes::GetEntry(Long64_t entry) {
    // Read contents of entry.
    if (!fChain) return 0;
    return fChain->GetEntry(entry);
}
Long64_t Delphes::LoadTree(Long64_t entry) {
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

void Delphes::Init(TTree *tree) {
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

    fChain->SetBranchAddress("Event", &Event_, &b_Event_);
    fChain->SetBranchAddress("Event.fUniqueID", Event_fUniqueID, &b_Event_fUniqueID);
    fChain->SetBranchAddress("Event.fBits", Event_fBits, &b_Event_fBits);
    fChain->SetBranchAddress("Event.Number", Event_Number, &b_Event_Number);
    fChain->SetBranchAddress("Event.ReadTime", Event_ReadTime, &b_Event_ReadTime);
    fChain->SetBranchAddress("Event.ProcTime", Event_ProcTime, &b_Event_ProcTime);
    fChain->SetBranchAddress("Event.ProcessID", Event_ProcessID, &b_Event_ProcessID);
    fChain->SetBranchAddress("Event.MPI", Event_MPI, &b_Event_MPI);
    fChain->SetBranchAddress("Event.Weight", Event_Weight, &b_Event_Weight);
    fChain->SetBranchAddress("Event.CrossSection", Event_CrossSection, &b_Event_CrossSection);
    fChain->SetBranchAddress("Event.CrossSectionError", Event_CrossSectionError, &b_Event_CrossSectionError);
    fChain->SetBranchAddress("Event.Scale", Event_Scale, &b_Event_Scale);
    fChain->SetBranchAddress("Event.AlphaQED", Event_AlphaQED, &b_Event_AlphaQED);
    fChain->SetBranchAddress("Event.AlphaQCD", Event_AlphaQCD, &b_Event_AlphaQCD);
    fChain->SetBranchAddress("Event.ID1", Event_ID1, &b_Event_ID1);
    fChain->SetBranchAddress("Event.ID2", Event_ID2, &b_Event_ID2);
    fChain->SetBranchAddress("Event.X1", Event_X1, &b_Event_X1);
    fChain->SetBranchAddress("Event.X2", Event_X2, &b_Event_X2);
    fChain->SetBranchAddress("Event.ScalePDF", Event_ScalePDF, &b_Event_ScalePDF);
    fChain->SetBranchAddress("Event.PDF1", Event_PDF1, &b_Event_PDF1);
    fChain->SetBranchAddress("Event.PDF2", Event_PDF2, &b_Event_PDF2);
    fChain->SetBranchAddress("Event_size", &Event_size, &b_Event_size);
    fChain->SetBranchAddress("Weight", &Weight_, &b_Weight_);
    fChain->SetBranchAddress("Weight.fUniqueID", Weight_fUniqueID, &b_Weight_fUniqueID);
    fChain->SetBranchAddress("Weight.fBits", Weight_fBits, &b_Weight_fBits);
    fChain->SetBranchAddress("Weight.Weight", Weight_Weight, &b_Weight_Weight);
    fChain->SetBranchAddress("Weight_size", &Weight_size, &b_Weight_size);
    fChain->SetBranchAddress("Jet", &Jet_, &b_Jet_);
    fChain->SetBranchAddress("Jet.fUniqueID", Jet_fUniqueID, &b_Jet_fUniqueID);
    fChain->SetBranchAddress("Jet.fBits", Jet_fBits, &b_Jet_fBits);
    fChain->SetBranchAddress("Jet.PT", Jet_PT, &b_Jet_PT);
    fChain->SetBranchAddress("Jet.Eta", Jet_Eta, &b_Jet_Eta);
    fChain->SetBranchAddress("Jet.Phi", Jet_Phi, &b_Jet_Phi);
    fChain->SetBranchAddress("Jet.T", Jet_T, &b_Jet_T);
    fChain->SetBranchAddress("Jet.Mass", Jet_Mass, &b_Jet_Mass);
    fChain->SetBranchAddress("Jet.DeltaEta", Jet_DeltaEta, &b_Jet_DeltaEta);
    fChain->SetBranchAddress("Jet.DeltaPhi", Jet_DeltaPhi, &b_Jet_DeltaPhi);
    fChain->SetBranchAddress("Jet.Flavor", Jet_Flavor, &b_Jet_Flavor);
    fChain->SetBranchAddress("Jet.FlavorAlgo", Jet_FlavorAlgo, &b_Jet_FlavorAlgo);
    fChain->SetBranchAddress("Jet.FlavorPhys", Jet_FlavorPhys, &b_Jet_FlavorPhys);
    fChain->SetBranchAddress("Jet.BTag", Jet_BTag, &b_Jet_BTag);
    fChain->SetBranchAddress("Jet.BTagAlgo", Jet_BTagAlgo, &b_Jet_BTagAlgo);
    fChain->SetBranchAddress("Jet.BTagPhys", Jet_BTagPhys, &b_Jet_BTagPhys);
    fChain->SetBranchAddress("Jet.TauTag", Jet_TauTag, &b_Jet_TauTag);
    fChain->SetBranchAddress("Jet.TauWeight", Jet_TauWeight, &b_Jet_TauWeight);
    fChain->SetBranchAddress("Jet.Charge", Jet_Charge, &b_Jet_Charge);
    fChain->SetBranchAddress("Jet.EhadOverEem", Jet_EhadOverEem, &b_Jet_EhadOverEem);
    fChain->SetBranchAddress("Jet.NCharged", Jet_NCharged, &b_Jet_NCharged);
    fChain->SetBranchAddress("Jet.NNeutrals", Jet_NNeutrals, &b_Jet_NNeutrals);
    fChain->SetBranchAddress("Jet.NeutralEnergyFraction", Jet_NeutralEnergyFraction, &b_Jet_NeutralEnergyFraction);
    fChain->SetBranchAddress("Jet.ChargedEnergyFraction", Jet_ChargedEnergyFraction, &b_Jet_ChargedEnergyFraction);
    fChain->SetBranchAddress("Jet.Beta", Jet_Beta, &b_Jet_Beta);
    fChain->SetBranchAddress("Jet.BetaStar", Jet_BetaStar, &b_Jet_BetaStar);
    fChain->SetBranchAddress("Jet.MeanSqDeltaR", Jet_MeanSqDeltaR, &b_Jet_MeanSqDeltaR);
    fChain->SetBranchAddress("Jet.PTD", Jet_PTD, &b_Jet_PTD);
    fChain->SetBranchAddress("Jet.FracPt[5]", Jet_FracPt, &b_Jet_FracPt);
    fChain->SetBranchAddress("Jet.Tau[5]", Jet_Tau, &b_Jet_Tau);
    fChain->SetBranchAddress("Jet.SoftDroppedJet", Jet_SoftDroppedJet, &b_Jet_SoftDroppedJet);
    fChain->SetBranchAddress("Jet.SoftDroppedSubJet1", Jet_SoftDroppedSubJet1, &b_Jet_SoftDroppedSubJet1);
    fChain->SetBranchAddress("Jet.SoftDroppedSubJet2", Jet_SoftDroppedSubJet2, &b_Jet_SoftDroppedSubJet2);
    fChain->SetBranchAddress("Jet.TrimmedP4[5]", Jet_TrimmedP4, &b_Jet_TrimmedP4);
    fChain->SetBranchAddress("Jet.PrunedP4[5]", Jet_PrunedP4, &b_Jet_PrunedP4);
    fChain->SetBranchAddress("Jet.SoftDroppedP4[5]", Jet_SoftDroppedP4, &b_Jet_SoftDroppedP4);
    fChain->SetBranchAddress("Jet.NSubJetsTrimmed", Jet_NSubJetsTrimmed, &b_Jet_NSubJetsTrimmed);
    fChain->SetBranchAddress("Jet.NSubJetsPruned", Jet_NSubJetsPruned, &b_Jet_NSubJetsPruned);
    fChain->SetBranchAddress("Jet.NSubJetsSoftDropped", Jet_NSubJetsSoftDropped, &b_Jet_NSubJetsSoftDropped);
    fChain->SetBranchAddress("Jet.ExclYmerge23", Jet_ExclYmerge23, &b_Jet_ExclYmerge23);
    fChain->SetBranchAddress("Jet.ExclYmerge34", Jet_ExclYmerge34, &b_Jet_ExclYmerge34);
    fChain->SetBranchAddress("Jet.ExclYmerge45", Jet_ExclYmerge45, &b_Jet_ExclYmerge45);
    fChain->SetBranchAddress("Jet.ExclYmerge56", Jet_ExclYmerge56, &b_Jet_ExclYmerge56);
    fChain->SetBranchAddress("Jet.Constituents", Jet_Constituents, &b_Jet_Constituents);
    fChain->SetBranchAddress("Jet.Particles", Jet_Particles, &b_Jet_Particles);
    fChain->SetBranchAddress("Jet.Area", Jet_Area, &b_Jet_Area);
    fChain->SetBranchAddress("Jet_size", &Jet_size, &b_Jet_size);
    fChain->SetBranchAddress("Electron", &Electron_, &b_Electron_);
    fChain->SetBranchAddress("Electron.fUniqueID", Electron_fUniqueID, &b_Electron_fUniqueID);
    fChain->SetBranchAddress("Electron.fBits", Electron_fBits, &b_Electron_fBits);
    fChain->SetBranchAddress("Electron.PT", Electron_PT, &b_Electron_PT);
    fChain->SetBranchAddress("Electron.Eta", Electron_Eta, &b_Electron_Eta);
    fChain->SetBranchAddress("Electron.Phi", Electron_Phi, &b_Electron_Phi);
    fChain->SetBranchAddress("Electron.T", Electron_T, &b_Electron_T);
    fChain->SetBranchAddress("Electron.Charge", Electron_Charge, &b_Electron_Charge);
    fChain->SetBranchAddress("Electron.EhadOverEem", Electron_EhadOverEem, &b_Electron_EhadOverEem);
    fChain->SetBranchAddress("Electron.Particle", Electron_Particle, &b_Electron_Particle);
    fChain->SetBranchAddress("Electron.IsolationVar", Electron_IsolationVar, &b_Electron_IsolationVar);
    fChain->SetBranchAddress("Electron.IsolationVarRhoCorr", Electron_IsolationVarRhoCorr,
                             &b_Electron_IsolationVarRhoCorr);
    fChain->SetBranchAddress("Electron.SumPtCharged", Electron_SumPtCharged, &b_Electron_SumPtCharged);
    fChain->SetBranchAddress("Electron.SumPtNeutral", Electron_SumPtNeutral, &b_Electron_SumPtNeutral);
    fChain->SetBranchAddress("Electron.SumPtChargedPU", Electron_SumPtChargedPU, &b_Electron_SumPtChargedPU);
    fChain->SetBranchAddress("Electron.SumPt", Electron_SumPt, &b_Electron_SumPt);
    fChain->SetBranchAddress("Electron.D0", Electron_D0, &b_Electron_D0);
    fChain->SetBranchAddress("Electron.DZ", Electron_DZ, &b_Electron_DZ);
    fChain->SetBranchAddress("Electron.ErrorD0", Electron_ErrorD0, &b_Electron_ErrorD0);
    fChain->SetBranchAddress("Electron.ErrorDZ", Electron_ErrorDZ, &b_Electron_ErrorDZ);
    fChain->SetBranchAddress("Electron_size", &Electron_size, &b_Electron_size);
    fChain->SetBranchAddress("Photon", &Photon_, &b_Photon_);
    fChain->SetBranchAddress("Photon.fUniqueID", Photon_fUniqueID, &b_Photon_fUniqueID);
    fChain->SetBranchAddress("Photon.fBits", Photon_fBits, &b_Photon_fBits);
    fChain->SetBranchAddress("Photon.PT", Photon_PT, &b_Photon_PT);
    fChain->SetBranchAddress("Photon.Eta", Photon_Eta, &b_Photon_Eta);
    fChain->SetBranchAddress("Photon.Phi", Photon_Phi, &b_Photon_Phi);
    fChain->SetBranchAddress("Photon.E", Photon_E, &b_Photon_E);
    fChain->SetBranchAddress("Photon.T", Photon_T, &b_Photon_T);
    fChain->SetBranchAddress("Photon.EhadOverEem", Photon_EhadOverEem, &b_Photon_EhadOverEem);
    fChain->SetBranchAddress("Photon.Particles", Photon_Particles, &b_Photon_Particles);
    fChain->SetBranchAddress("Photon.IsolationVar", Photon_IsolationVar, &b_Photon_IsolationVar);
    fChain->SetBranchAddress("Photon.IsolationVarRhoCorr", Photon_IsolationVarRhoCorr, &b_Photon_IsolationVarRhoCorr);
    fChain->SetBranchAddress("Photon.SumPtCharged", Photon_SumPtCharged, &b_Photon_SumPtCharged);
    fChain->SetBranchAddress("Photon.SumPtNeutral", Photon_SumPtNeutral, &b_Photon_SumPtNeutral);
    fChain->SetBranchAddress("Photon.SumPtChargedPU", Photon_SumPtChargedPU, &b_Photon_SumPtChargedPU);
    fChain->SetBranchAddress("Photon.SumPt", Photon_SumPt, &b_Photon_SumPt);
    fChain->SetBranchAddress("Photon.Status", Photon_Status, &b_Photon_Status);
    fChain->SetBranchAddress("Photon_size", &Photon_size, &b_Photon_size);
    fChain->SetBranchAddress("Muon", &Muon_, &b_Muon_);
    fChain->SetBranchAddress("Muon.fUniqueID", Muon_fUniqueID, &b_Muon_fUniqueID);
    fChain->SetBranchAddress("Muon.fBits", Muon_fBits, &b_Muon_fBits);
    fChain->SetBranchAddress("Muon.PT", Muon_PT, &b_Muon_PT);
    fChain->SetBranchAddress("Muon.Eta", Muon_Eta, &b_Muon_Eta);
    fChain->SetBranchAddress("Muon.Phi", Muon_Phi, &b_Muon_Phi);
    fChain->SetBranchAddress("Muon.T", Muon_T, &b_Muon_T);
    fChain->SetBranchAddress("Muon.Charge", Muon_Charge, &b_Muon_Charge);
    fChain->SetBranchAddress("Muon.Particle", Muon_Particle, &b_Muon_Particle);
    fChain->SetBranchAddress("Muon.IsolationVar", Muon_IsolationVar, &b_Muon_IsolationVar);
    fChain->SetBranchAddress("Muon.IsolationVarRhoCorr", Muon_IsolationVarRhoCorr, &b_Muon_IsolationVarRhoCorr);
    fChain->SetBranchAddress("Muon.SumPtCharged", Muon_SumPtCharged, &b_Muon_SumPtCharged);
    fChain->SetBranchAddress("Muon.SumPtNeutral", Muon_SumPtNeutral, &b_Muon_SumPtNeutral);
    fChain->SetBranchAddress("Muon.SumPtChargedPU", Muon_SumPtChargedPU, &b_Muon_SumPtChargedPU);
    fChain->SetBranchAddress("Muon.SumPt", Muon_SumPt, &b_Muon_SumPt);
    fChain->SetBranchAddress("Muon.D0", Muon_D0, &b_Muon_D0);
    fChain->SetBranchAddress("Muon.DZ", Muon_DZ, &b_Muon_DZ);
    fChain->SetBranchAddress("Muon.ErrorD0", Muon_ErrorD0, &b_Muon_ErrorD0);
    fChain->SetBranchAddress("Muon.ErrorDZ", Muon_ErrorDZ, &b_Muon_ErrorDZ);
    fChain->SetBranchAddress("Muon_size", &Muon_size, &b_Muon_size);
    fChain->SetBranchAddress("MissingET", &MissingET_, &b_MissingET_);
    fChain->SetBranchAddress("MissingET.fUniqueID", MissingET_fUniqueID, &b_MissingET_fUniqueID);
    fChain->SetBranchAddress("MissingET.fBits", MissingET_fBits, &b_MissingET_fBits);
    fChain->SetBranchAddress("MissingET.MET", MissingET_MET, &b_MissingET_MET);
    fChain->SetBranchAddress("MissingET.Eta", MissingET_Eta, &b_MissingET_Eta);
    fChain->SetBranchAddress("MissingET.Phi", MissingET_Phi, &b_MissingET_Phi);
    fChain->SetBranchAddress("MissingET_size", &MissingET_size, &b_MissingET_size);
    fChain->SetBranchAddress("ScalarHT", &ScalarHT_, &b_ScalarHT_);
    fChain->SetBranchAddress("ScalarHT.fUniqueID", ScalarHT_fUniqueID, &b_ScalarHT_fUniqueID);
    fChain->SetBranchAddress("ScalarHT.fBits", ScalarHT_fBits, &b_ScalarHT_fBits);
    fChain->SetBranchAddress("ScalarHT.HT", ScalarHT_HT, &b_ScalarHT_HT);
    fChain->SetBranchAddress("ScalarHT_size", &ScalarHT_size, &b_ScalarHT_size);
    Notify();
}

Bool_t Delphes::Notify() {
    // The Notify() function is called when a new file is opened. This
    // can be either for a new TTree in a TChain or when when a new TTree
    // is started when using PROOF. It is normally not necessary to make changes
    // to the generated code, but the routine can be extended by the
    // user if needed. The return value is currently not used.

    return kTRUE;
}

void Delphes::Show(Long64_t entry) {
    // Print contents of entry.
    // If entry is not specified, print current entry
    if (!fChain) return;
    fChain->Show(entry);
}
Int_t Delphes::Cut(Long64_t entry) {
    // This function may be called from Loop.
    // returns  1 if entry is accepted.
    // returns -1 otherwise.
    return 1;
}
#endif  // #ifdef Delphes_cxx
