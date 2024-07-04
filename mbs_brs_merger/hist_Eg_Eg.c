
#include "TCutG.h"
#include "TH3I.h"
#include "include/MBS_BRSv.C"

double curr_offset[48] ={2555.2,1666.8,777.2,1014.8,304.7,-1838.7,-2313.3,-689.6,1407.5,-237.9,261.5,-231.8,920.0,17.1,690.7,622.5,1275.8,1704.2,167.5,276.4,-972.5,-1615.3,-3549.5,-961.6,1692.2,-920.1,189.4,-797.4,245.1,-447.9,1342.7,366.7,1078.9,2337.9,68.7,943.0,-492.4,-1075.6,-3401.8,-2359.9,1251.1,-1262.1,231.6,-774.2,123.9,-721.1,310.5,818.7}; // new offset

void hist_Eg_Eg()
{
	TString str_ri = "94Ru";

	//TFile *file_tcut = new TFile("TCuts.root","read");
	//TCutG *tcutg = (TCutG*) file_tcut->Get("Pd96");

	//ch->Draw("CalE:FTle>>(2000,0,2000, 2000,-10e6,10e6)","aoq>2.085 && aoq<2.091 && zet>46 && zet<46.7","colz")

	TH2I *h2_Eg_FTle = new TH2I("h2_Eg_FTle","h2_Eg_FTle; Eg [keV]; FTle [ps]",
			1000,0,2000,
			1200,-2e6,10e6);



	TH2I *h2_Eg_Eg = new TH2I("h2_Eg_Eg",
			Form("h2_Eg_Eg {%s}; Eg [keV]; Eg [keV]", str_ri.Data()),
			//Form("h2_Eg_Eg {%s}; Eg [keV]; Eg [keV]", tcutg->GetName()),
			1000,0,2000,
			1000,0,2000
			);
	TH2I *h2_aoq_zet = new TH2I("h2_aoq_zet","h2_aoq_zet; aoq; zet",
			1000,2,2.18, 600,40,52 );





	TChain *ch = new TChain("merged");
	if(ch->Add("ROOT/merged_mbs49_pidcut/merged_"+str_ri+".root")==0) return;
	//if(ch->Add("ROOT/merged_mbs49/merged00*.root")==0) return;;
	MBS_BRSv *pmerg = new MBS_BRSv(ch);
	const Long64_t nent = pmerg->fChain->GetEntries();

	size_t ihit, jhit;


	Long64_t ient=0;
	Long64_t loop_couneter=0;
	for (ient=0; ient<nent; ient++, loop_couneter++)
	{
		pmerg->GetEntry(ient);
		if ((loop_couneter & 0xff)==0)
		{
			fprintf(stdout,"\rient %lld of %lld (%.1f%%)",ient,nent, 100.*ient/nent);
		}

		//if (!(tcutg->IsInside(pmerg->aoq, pmerg->zet))) continue;
		double aoq = pmerg->aoq; double zet=pmerg->zet;
		//if (!(aoq>2.085 && aoq<2.091 && zet>46 && zet<46.7)) continue;
		h2_aoq_zet->Fill(aoq,zet);





		size_t nhit = pmerg->KL->size();

		for (ihit=0; ihit<nhit; ihit++)
		{
			int iKL = pmerg->KL->at(ihit);
			double icalE = pmerg->CalE->at(ihit);
			double iftle = pmerg->FTle->at(ihit);
			iftle -= curr_offset[iKL];
			if (iftle<-1800e3) continue;
			//if (iftle>-1150e3) continue; // for 94Pd

			h2_Eg_FTle->Fill(icalE,iftle);
			for(jhit=0; jhit<nhit; jhit++) if(ihit!=jhit)
			{
				int jKL = pmerg->KL->at(jhit);
				double jcalE = pmerg->CalE->at(jhit);
				double jftle = pmerg->FTle->at(jhit);
				jftle -= curr_offset[jKL];
				if (jftle<-1800e3) continue;
				//if (jftle>-1150e3) continue; // for 94Pd
				h2_Eg_Eg->Fill(pmerg->CalE->at(ihit),pmerg->CalE->at(jhit));



			}
		}
	}
	fprintf(stdout,"\rient %lld of %lld (%.1f%%)",ient,nent, 100.*ient/nent);
	fprintf(stdout,"\n");


	TFile *fileout = new TFile("Hist/h2_Eg_Eg_"+str_ri+".root","recreate");

	TCanvas *c1 = new TCanvas("c1","c1");
	h2_Eg_Eg->Draw("colz");
	TCanvas *c2 = new TCanvas("c2","c2");
	h2_Eg_FTle->Draw("colz");
	TCanvas *c3 = new TCanvas("c3","c3");
	h2_aoq_zet->Draw("colz");

	h2_Eg_Eg->Write();
	h2_Eg_FTle->Write();
	h2_aoq_zet->Write();




}



