
#include "TCutG.h"
#include "TH3I.h"
#include "include/MBS_BRSv.C"

#define ppac_pos_corr 1
#ifdef ppac_pos_corr
#include "include/gamma_flight_time.h"
#endif // ppac_pos_corr

double curr_offset[48] ={2555.2,1666.8,777.2,1014.8,304.7,-1838.7,-2313.3,-689.6,1407.5,-237.9,261.5,-231.8,920.0,17.1,690.7,622.5,1275.8,1704.2,167.5,276.4,-972.5,-1615.3,-3549.5,-961.6,1692.2,-920.1,189.4,-797.4,245.1,-447.9,1342.7,366.7,1078.9,2337.9,68.7,943.0,-492.4,-1075.6,-3401.8,-2359.9,1251.1,-1262.1,231.6,-774.2,123.9,-721.1,310.5,818.7}; // new offset

void hist_Tdiff_94Pd_0324_1092()
{
	TString str_ri = "94Pd";
	Double_t Ed = 324;
	Double_t Ef =1092;

	//TFile *file_tcut = new TFile("TCuts.root","read");
	//TCutG *tcutg = (TCutG*) file_tcut->Get("Pd96");

	TH1I *h1_Tdiff = new TH1I(
			Form("h1_Tdiff_%s_%.0f_%.0f",  str_ri.Data(), Ed, Ef),
			Form("T(%.0f keV)-T(%.0f keV) {%s}; Tdiff [ps]",  Ed, Ef, str_ri.Data()),
			4000,-40e3,40e3);


	double m_Ed = 314.205;
	double s_Ed = 10.1881;
	double m_Ef =1071.121;
	double s_Ef = 20.5832;

	TChain *ch = new TChain("merged");
	if(ch->Add("ROOT/merged_mbs49_pidcut/merged_"+str_ri+".root")==0) return;
	MBS_BRSv *pmerg = new MBS_BRSv(ch);
	const Long64_t nent = pmerg->fChain->GetEntries();

	size_t ihit, jhit;
#ifdef ppac_pos_corr
	gamma_flight_time gft = gamma_flight_time();
#endif // ppac_pos_corr


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





		size_t nhit = pmerg->KL->size();
#ifdef ppac_pos_corr
		//ppac_x=-999e20;	//ppac_y=-999e20;
		if (pmerg->ppac_x < -899e20 || pmerg->ppac_y < -899e20) continue;
#endif // ppac_pos_corr

		for (ihit=0; ihit<nhit; ihit++)
		{
			int iKL = pmerg->KL->at(ihit);
			double icalE = pmerg->CalE->at(ihit);
			double iftle = pmerg->FTle->at(ihit);
			iftle -= curr_offset[iKL];
#ifdef ppac_pos_corr
			iftle -= gft.flight_time(iKL, pmerg->ppac_x, pmerg->ppac_y,0);
#endif // ppac_pos_corr
			//if (iftle<-1800e3) continue;
			if (icalE<m_Ed-3*s_Ed || icalE>m_Ed+3*s_Ed) continue;
			

			for(jhit=0; jhit<nhit; jhit++) if(ihit!=jhit)
			{
				int jKL = pmerg->KL->at(jhit);
				double jcalE = pmerg->CalE->at(jhit);
				double jftle = pmerg->FTle->at(jhit);
				jftle -= curr_offset[jKL];
#ifdef ppac_pos_corr
				jftle -= gft.flight_time(jKL, pmerg->ppac_x, pmerg->ppac_y,0);
#endif // ppac_pos_corr
				if (jftle<-1800e3) continue;
				if (jcalE<m_Ef-3*s_Ef || jcalE>m_Ef+3*s_Ef) continue;
			
				h1_Tdiff->Fill(iftle-jftle);



			}
		}
	}
	fprintf(stdout,"\rient %lld of %lld (%.1f%%)",ient,nent, 100.*ient/nent);
	fprintf(stdout,"\n");


	TFile *fileout = new TFile(Form("Hist/h1_Tdiff_%s_%04.f_%04.f.root",str_ri.Data(),Ed,Ef),"recreate");
	TCanvas *c3 = new TCanvas("c3","c3");
	h1_Tdiff->Draw();
	h1_Tdiff->Write();



}



