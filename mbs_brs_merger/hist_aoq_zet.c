


#include "glob.h"

#include "TH1I.h"



void hist_aoq_zet(TString runname)
{
	int ret;
	TChain *ch = new TChain("tree");
	ret=ch->Add("/home/idatendaq/idaten24/rootfiles/analysis_LJ/ROOT/vme/"+runname+".root");
	if (ret<=0) return;
	BRSvme *pbrs = new BRSvme(ch);
	const Long64_t nent_brs = pbrs->fChain->GetEntries();

	TH2I *h2_aoq_zet = new TH2I(
		Form("h2_aoq_zet"),
		Form("h2_aoq_zet; aoq; zet"),
		1000, 1.95, 2.3, 
		1000, 39,53
		);


	Long64_t loop_counter=0;
	Long64_t ient_brs=0;
	for ( ient_brs=0;
			ient_brs<nent_brs;
			ient_brs++, loop_counter++)
	{
		if ((loop_counter&0xfff)==0)
		{
			fprintf(stdout,"\rient %lld of %lld (%0.1f%%)", ient_brs,nent_brs,100.*ient_brs/nent_brs);
		}
		ret = pbrs->GetEntry(ient_brs);
		if (ret==0) return;

		h2_aoq_zet->Fill(pbrs->aoq, pbrs->zet);
	}
	fprintf(stdout,"\rient %lld of %lld (%0.1f%%)", ient_brs,nent_brs,100.*ient_brs/nent_brs);
	fprintf(stdout,"\n");

	//h2_aoq_zet->Draw();

	TFile *fileout = new TFile("Hist/h2_aoq_zet_"+runname+".root","recreate");
	h2_aoq_zet->Write();
	fileout->Close();


}


