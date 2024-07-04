


#include "TCutG.h"
#include "TFile.h"
#include "include/MBS_BRSv.C"

void tree_pidcut(TString str_ri="93Ru")
{
	//TString str_ri = "93Ru";

	TFile *file_tcut = new TFile("./Cuts.root","read");
	TCutG *tcutg = (TCutG*) file_tcut->Get("cut_"+str_ri);
	if (tcutg==NULL) return;
	fprintf(stdout,"found %s\n",("cut_"+str_ri).Data());
	
	TChain *ch = new TChain("merged");
	ch->Add("ROOT/merged_mbs49/merged_mbs49_brsaccpt_*.root");
	MBS_BRSv *pmerg = new MBS_BRSv(ch);
	const Long64_t nent = pmerg->fChain->GetEntries();

	TFile *fileout = new TFile("ROOT/merged_mbs49_pidcut/merged_"+str_ri+".root","recreate");
	TTree *treeout = pmerg->fChain->CloneTree(0);
	treeout->SetMaxTreeSize(2*1000*1000*1000);
	TH2I *h2_aoq_zet = new TH2I("h2_aoq_zet","h2_aoq_zet; aoq; zet",
			400, 1.98, 2.18,
		   	600, 40, 52);

	Long64_t ient=0;
	Long64_t loop_couneter=0;
	for (ient=0; ient<nent; ient++, loop_couneter++)
	{
		pmerg->GetEntry(ient);
		if ((loop_couneter & 0xfff)==0)
		{
			fprintf(stdout,"\rient %lld of %lld (%.1f%%)",ient,nent, 100.*ient/nent);
		}

		double aoq = pmerg->aoq; double zet=pmerg->zet;
		h2_aoq_zet->Fill(aoq,zet);
		if (!(tcutg->IsInside(aoq, zet))) continue;
		//if (!(aoq>2.085 && aoq<2.091 && zet>46 && zet<46.7)) continue;
		treeout->Fill();

	}
	fprintf(stdout,"\rient %lld of %lld (%.1f%%)",ient,nent, 100.*ient/nent);
	fprintf(stdout,"\n");


	treeout->Write();
	h2_aoq_zet->Draw("colz");
	tcutg->Draw("same");
	//fileout->Close();



}
