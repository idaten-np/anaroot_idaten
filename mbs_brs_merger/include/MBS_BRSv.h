//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Jul  3 14:54:27 2024 by ROOT version 6.30/04
// from TTree merged/merged
// found on file: ROOT/merged_mbs49/merged_mbs49_brsaccpt_0.root
//////////////////////////////////////////////////////////

#ifndef MBS_BRSv_h
#define MBS_BRSv_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.
#include "vector"
#include "vector"

class MBS_BRSv {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
   Int_t           fTreeNumber_mbs;
   Int_t           fTreeNumber_brs;
   Long64_t        ient_local_mbs;
   Long64_t        ient_local_brs;
   vector<int>     *KL;
   vector<double>  *STOT;
   vector<double>  *FTle;
   vector<double>  *CalE;
   Double_t        ppac_x;
   Double_t        ppac_y;
   Double_t        aoq;
   Double_t        zet;
   ULong64_t       ts_mbs;
   ULong64_t       ts_vet;
   ULong64_t       ts_mpv;
   ULong64_t       ts_brs;

   // List of branches
   TBranch        *b_fTreeNumber_mbs;   //!
   TBranch        *b_fTreeNumber_brs;   //!
   TBranch        *b_ient_local_mbs;   //!
   TBranch        *b_ient_local_brs;   //!
   TBranch        *b_KL;   //!
   TBranch        *b_STOT;   //!
   TBranch        *b_FTle;   //!
   TBranch        *b_CalE;   //!
   TBranch        *b_ppac_x;   //!
   TBranch        *b_ppac_y;   //!
   TBranch        *b_aoq;   //!
   TBranch        *b_zet;   //!
   TBranch        *b_ts_mbs;   //!
   TBranch        *b_ts_vet;   //!
   TBranch        *b_ts_mpv;   //!
   TBranch        *b_ts_brs;   //!

   MBS_BRSv(TTree *tree=0);
   virtual ~MBS_BRSv();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef MBS_BRSv_cxx
MBS_BRSv::MBS_BRSv(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("ROOT/merged_mbs49/merged_mbs49_brsaccpt_0.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("ROOT/merged_mbs49/merged_mbs49_brsaccpt_0.root");
      }
      f->GetObject("merged",tree);

   }
   Init(tree);
}

MBS_BRSv::~MBS_BRSv()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t MBS_BRSv::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t MBS_BRSv::LoadTree(Long64_t entry)
{
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

void MBS_BRSv::Init(TTree *tree)
{
   // The Init() function is called when the selector needs to initialize
   // a new tree or chain. Typically here the branch addresses and branch
   // pointers of the tree will be set.
   // It is normally not necessary to make changes to the generated
   // code, but the routine can be extended by the user if needed.
   // Init() will be called many times when running on PROOF
   // (once per file to be processed).

   // Set object pointer
   KL = 0;
   STOT = 0;
   FTle = 0;
   CalE = 0;
   // Set branch addresses and branch pointers
   if (!tree) return;
   fChain = tree;
   fCurrent = -1;
   fChain->SetMakeClass(1);

   fChain->SetBranchAddress("fTreeNumber_mbs", &fTreeNumber_mbs, &b_fTreeNumber_mbs);
   fChain->SetBranchAddress("fTreeNumber_brs", &fTreeNumber_brs, &b_fTreeNumber_brs);
   fChain->SetBranchAddress("ient_local_mbs", &ient_local_mbs, &b_ient_local_mbs);
   fChain->SetBranchAddress("ient_local_brs", &ient_local_brs, &b_ient_local_brs);
   fChain->SetBranchAddress("KL", &KL, &b_KL);
   fChain->SetBranchAddress("STOT", &STOT, &b_STOT);
   fChain->SetBranchAddress("FTle", &FTle, &b_FTle);
   fChain->SetBranchAddress("CalE", &CalE, &b_CalE);
   fChain->SetBranchAddress("ppac_x", &ppac_x, &b_ppac_x);
   fChain->SetBranchAddress("ppac_y", &ppac_y, &b_ppac_y);
   fChain->SetBranchAddress("aoq", &aoq, &b_aoq);
   fChain->SetBranchAddress("zet", &zet, &b_zet);
   fChain->SetBranchAddress("ts_mbs", &ts_mbs, &b_ts_mbs);
   fChain->SetBranchAddress("ts_vet", &ts_vet, &b_ts_vet);
   fChain->SetBranchAddress("ts_mpv", &ts_mpv, &b_ts_mpv);
   fChain->SetBranchAddress("ts_brs", &ts_brs, &b_ts_brs);
   Notify();
}

Bool_t MBS_BRSv::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void MBS_BRSv::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t MBS_BRSv::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef MBS_BRSv_cxx
