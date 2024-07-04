
#include "TCutG.h"
#include "TH3I.h"
#include "MBS_BRSv.C"

double curr_offset[48] ={2555.2,1666.8,777.2,1014.8,304.7,-1838.7,-2313.3,-689.6,1407.5,-237.9,261.5,-231.8,920.0,17.1,690.7,622.5,1275.8,1704.2,167.5,276.4,-972.5,-1615.3,-3549.5,-961.6,1692.2,-920.1,189.4,-797.4,245.1,-447.9,1342.7,366.7,1078.9,2337.9,68.7,943.0,-492.4,-1075.6,-3401.8,-2359.9,1251.1,-1262.1,231.6,-774.2,123.9,-721.1,310.5,818.7}; // new offset

void hist_Tdiff_0544_0146()
{
	//TFile *file_tcut = new TFile("TCuts.root","read");
	//TCutG *tcutg = (TCutG*) file_tcut->Get("Pd96");

	//ch->Draw("CalE:FTle>>(2000,0,2000, 2000,-10e6,10e6)","aoq>2.085 && aoq<2.091 && zet>46 && zet<46.7","colz")

	TH2I *h2_Eg_Eg = new TH2I("h2_Eg_Eg",
			Form("h2_Eg_Eg {%s}; Eg [keV]; Eg [keV]", "96Pd"),
			//Form("h2_Eg_Eg {%s}; Eg [keV]; Eg [keV]", tcutg->GetName()),
			1000,0,2000,
			1000,0,2000
			);
	TH2I *h2_aoq_zet = new TH2I("h2_aoq_zet","h2_aoq_zet; aoq; zet",
			1000,2,2.18, 600,40,52 );
	TH2I *h2_Eg_FTle = new TH2I("h2_Eg_FTle","h2_Eg_FTle; Eg [keV]; FTle [ps]",
			1000,0,2000,
			4000,-2000e3,2000e3);

	TH1I *h1_Tdiff = new TH1I("h1_Tdiff","h1_Tdiff; Tdiff [ps]",
			4000,-40e3,40e3);


	double m_Ed = 531.946;
	double s_Ed = 11.2378;
	double m_Ef = 141.088;
	double s_Ef = 6.23644;

	TChain *ch = new TChain("merged");
	ch->Add("merged_93Ru.root");
	//ch->Add("merged00*.root");
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
			if (iftle<-1800e3) continue;
			if (iftle>-1150e3) continue;
			if (icalE<m_Ed-3*s_Ed || icalE>m_Ed+3*s_Ed) continue;
			

			h2_Eg_FTle->Fill(icalE,iftle);
			iftle -= curr_offset[iKL];
			for(jhit=0; jhit<nhit; jhit++) if(ihit!=jhit)
			{
				int jKL = pmerg->KL->at(jhit);
				double jcalE = pmerg->CalE->at(jhit);
				double jftle = pmerg->FTle->at(jhit);
				if (jftle<-1800e3) continue;
				if (jftle>-1150e3) continue;
				if (jcalE<m_Ef-3*s_Ef || jcalE>m_Ef+3*s_Ef) continue;
			
				jftle -= curr_offset[jKL];
				h2_Eg_Eg->Fill(pmerg->CalE->at(ihit),pmerg->CalE->at(jhit));
				h1_Tdiff->Fill(iftle-jftle);



			}
		}
	}
	fprintf(stdout,"\rient %lld of %lld (%.1f%%)",ient,nent, 100.*ient/nent);
	fprintf(stdout,"\n");


	TCanvas *c1 = new TCanvas("c1","c1");
	h2_Eg_Eg->Draw("colz");
	TCanvas *c2 = new TCanvas("c2","c2");
	h2_Eg_FTle->Draw("colz");
	TCanvas *c3 = new TCanvas("c3","c3");
	h1_Tdiff->Draw();
	//h2_aoq_zet->Draw("colz");



}



