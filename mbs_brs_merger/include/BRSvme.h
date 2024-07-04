//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Jul  1 14:03:11 2024 by ROOT version 6.30/04
// from TTree tree/tree
// found on file: ../idaten0087.root
//////////////////////////////////////////////////////////

#ifndef BRSvme_h
#define BRSvme_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class BRSvme {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.

   // Declaration of leaf types
 //TreeData        *pid;
   Long64_t        ts;
   Long64_t        sts;
   Double_t        de;
   Double_t        tof;
   Double_t        zet;
   Double_t        aoq;
   Double_t        f5x;
   Double_t        f11x;
   Double_t        f11y;
   Double_t        f11dt;
   Double_t        beta;
   ULong64_t       EventId;
   ULong64_t       RunId;
   ULong64_t       f11t_mpv;
   ULong64_t       t_vetar;
   ULong64_t       tsb_mpv;

   // List of branches
   TBranch        *b_pid_ts;   //!
   TBranch        *b_pid_sts;   //!
   TBranch        *b_pid_de;   //!
   TBranch        *b_pid_tof;   //!
   TBranch        *b_pid_zet;   //!
   TBranch        *b_pid_aoq;   //!
   TBranch        *b_pid_f5x;   //!
   TBranch        *b_pid_f11x;   //!
   TBranch        *b_pid_f11y;   //!
   TBranch        *b_pid_f11dt;   //!
   TBranch        *b_pid_beta;   //!
   TBranch        *b_pid_EventId;   //!
   TBranch        *b_pid_RunId;   //!
   TBranch        *b_pid_f11t_mpv;   //!
   TBranch        *b_pid_t_vetar;   //!
   TBranch        *b_pid_tsb_mpv;   //!

   BRSvme(TTree *tree=0);
   virtual ~BRSvme();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef BRSvme_cxx
BRSvme::BRSvme(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("../idaten0087.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("../idaten0087.root");
      }
      f->GetObject("tree",tree);

   }
   Init(tree);
}

BRSvme::~BRSvme()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t BRSvme::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t BRSvme::LoadTree(Long64_t entry)
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

void BRSvme::Init(TTree *tree)
{
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

   fChain->SetBranchAddress("ts", &ts, &b_pid_ts);
   fChain->SetBranchAddress("sts", &sts, &b_pid_sts);
   fChain->SetBranchAddress("de", &de, &b_pid_de);
   fChain->SetBranchAddress("tof", &tof, &b_pid_tof);
   fChain->SetBranchAddress("zet", &zet, &b_pid_zet);
   fChain->SetBranchAddress("aoq", &aoq, &b_pid_aoq);
   fChain->SetBranchAddress("f5x", &f5x, &b_pid_f5x);
   fChain->SetBranchAddress("f11x", &f11x, &b_pid_f11x);
   fChain->SetBranchAddress("f11y", &f11y, &b_pid_f11y);
   fChain->SetBranchAddress("f11dt", &f11dt, &b_pid_f11dt);
   fChain->SetBranchAddress("beta", &beta, &b_pid_beta);
   fChain->SetBranchAddress("EventId", &EventId, &b_pid_EventId);
   fChain->SetBranchAddress("RunId", &RunId, &b_pid_RunId);
   fChain->SetBranchAddress("f11t_mpv", &f11t_mpv, &b_pid_f11t_mpv);
   fChain->SetBranchAddress("t_vetar", &t_vetar, &b_pid_t_vetar);
   fChain->SetBranchAddress("tsb_mpv", &tsb_mpv, &b_pid_tsb_mpv);
   Notify();
}

Bool_t BRSvme::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void BRSvme::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t BRSvme::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef BRSvme_cxx
