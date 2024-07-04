//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Mon Jul  1 16:03:24 2024 by ROOT version 6.30/04
// from TTree AnalysisxTree/Go4FileStore
// found on file: ROOT/mbs/Run49_0009_highrate_brsaccpt_0001.root
//////////////////////////////////////////////////////////

#ifndef MBS_h
#define MBS_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

class MBS {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

// Fixed size dimensions of array or collections stored in the TTree if any.
   static constexpr Int_t kMaxOutputEvent = 1;

   // Declaration of leaf types
 //TTamex_FullEvent *OutputEvent_;
 //TTamex_FullEvent *OutputEvent_TGo4EventElement;
   ULong64_t       OutputEvent_WR_TS;
   vector<unsigned int> OutputEvent_hit_SSY;
   vector<unsigned int> OutputEvent_hit_SFP;
   vector<unsigned int> OutputEvent_hit_TAM;
   vector<int>     OutputEvent_hit_PCHA;
   vector<double>  OutputEvent_hit_STOT;
   vector<double>  OutputEvent_hit_STle;
   vector<double>  OutputEvent_hit_FTOT;
   vector<double>  OutputEvent_hit_FTle;
   vector<double>  OutputEvent_hit_CalE;
   vector<double>  OutputEvent_hit_TTS;

   // List of branches
   TBranch        *b_OutputEvent_WR_TS;   //!
   TBranch        *b_OutputEvent_hit_SSY;   //!
   TBranch        *b_OutputEvent_hit_SFP;   //!
   TBranch        *b_OutputEvent_hit_TAM;   //!
   TBranch        *b_OutputEvent_hit_PCHA;   //!
   TBranch        *b_OutputEvent_hit_STOT;   //!
   TBranch        *b_OutputEvent_hit_STle;   //!
   TBranch        *b_OutputEvent_hit_FTOT;   //!
   TBranch        *b_OutputEvent_hit_FTle;   //!
   TBranch        *b_OutputEvent_hit_CalE;   //!
   TBranch        *b_OutputEvent_hit_TTS;   //!

   MBS(TTree *tree=0);
   virtual ~MBS();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Loop();
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef MBS_cxx
MBS::MBS(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("ROOT/mbs/Run49_0009_highrate_brsaccpt_0001.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("ROOT/mbs/Run49_0009_highrate_brsaccpt_0001.root");
      }
      f->GetObject("AnalysisxTree",tree);

   }
   Init(tree);
}

MBS::~MBS()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t MBS::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t MBS::LoadTree(Long64_t entry)
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

void MBS::Init(TTree *tree)
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

   fChain->SetBranchAddress("OutputEvent.WR_TS", &OutputEvent_WR_TS, &b_OutputEvent_WR_TS);
   fChain->SetBranchAddress("OutputEvent.hit_SSY", &OutputEvent_hit_SSY, &b_OutputEvent_hit_SSY);
   fChain->SetBranchAddress("OutputEvent.hit_SFP", &OutputEvent_hit_SFP, &b_OutputEvent_hit_SFP);
   fChain->SetBranchAddress("OutputEvent.hit_TAM", &OutputEvent_hit_TAM, &b_OutputEvent_hit_TAM);
   fChain->SetBranchAddress("OutputEvent.hit_PCHA", &OutputEvent_hit_PCHA, &b_OutputEvent_hit_PCHA);
   fChain->SetBranchAddress("OutputEvent.hit_STOT", &OutputEvent_hit_STOT, &b_OutputEvent_hit_STOT);
   fChain->SetBranchAddress("OutputEvent.hit_STle", &OutputEvent_hit_STle, &b_OutputEvent_hit_STle);
   fChain->SetBranchAddress("OutputEvent.hit_FTOT", &OutputEvent_hit_FTOT, &b_OutputEvent_hit_FTOT);
   fChain->SetBranchAddress("OutputEvent.hit_FTle", &OutputEvent_hit_FTle, &b_OutputEvent_hit_FTle);
   fChain->SetBranchAddress("OutputEvent.hit_CalE", &OutputEvent_hit_CalE, &b_OutputEvent_hit_CalE);
   fChain->SetBranchAddress("OutputEvent.hit_TTS", &OutputEvent_hit_TTS, &b_OutputEvent_hit_TTS);
   Notify();
}

Bool_t MBS::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void MBS::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t MBS::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef MBS_cxx
