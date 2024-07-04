
#include "glob.h"


#include <iostream>

void mbs_brs()
{
	int icha=0, jcha=0;
	int itam=0, jtam=0;
	double istot, jstot;
	double iftle, jftle;
	double icale, jcale;
	int iKL, jKL;

	TH2I *h2_CalE_FTle_LaBr = new TH2I(
				Form("h2_CalE_FTle_LaBr"),
				Form("h2_CalE_FTle_LaBr; CalE [keV]; FTle [ps]"),
				2000,0,2000,
				2*COARSE_CT_RANGE, -COARSE_CT_RANGE*CYCLE_TIME, COARSE_CT_RANGE*CYCLE_TIME
			);
	

	TH2I *h2_aoq_zet = new TH2I("h2_aoq_zet","h2_aoq_zet; aoq; zet",
			400, 1.98, 2.18,
		   	600, 40, 52);


	Bool_t b_veto;

	Int_t ppac_mask;
	Double_t ppac_tx1, ppac_tx2, ppac_ty1, ppac_ty2;
	Double_t ppac_x, ppac_y;



	int ret=0;

	TChain *ch_mbs = new TChain("AnalysisxTree");
	TChain *ch_brs = new TChain("tree");
	ret = ch_mbs->Add(Form("/home/idatendaq/idaten24/rootfiles/mbsrootfiles_unpack/Run49_0009_highrate_brsaccpt_0*.root"));
	if (ret==0) return;
	ret = ch_brs->Add(Form("/raid/r0901/idaten24/rootfiles/analysis_high_LJ/ROOT/vme_mbs49_brsaccpt/idaten00*.root"));
	if (ret==0) return;
	ch_mbs->GetListOfFiles()->Print();
	std::cout << "clr to continue" << std::endl;
	std::cin >> ret;
	ch_brs->GetListOfFiles()->Print();
	std::cout << "clr to continue" << std::endl;
	std::cin >> ret;

	MBS *pmbs = new MBS(ch_mbs);
	BRSvme *pbrs = new BRSvme(ch_brs);
	Long64_t nent_mbs = pmbs->fChain->GetEntries();
	Long64_t nent_brs = pbrs->fChain->GetEntries();

	fprintf(stdout, "nent_mbs %lld ",nent_mbs);
	fprintf(stdout, "nent_brs %lld ",nent_brs);
	fprintf(stdout, "\n");

	pmbs->fChain->SetBranchStatus("*",0);
	pmbs->fChain->SetBranchStatus("OutputEvent.WR_TS",1);
	pmbs->fChain->SetBranchStatus("OutputEvent.hit_TAM",1);
	pmbs->fChain->SetBranchStatus("OutputEvent.hit_PCHA",1);
	pmbs->fChain->SetBranchStatus("OutputEvent.hit_STOT",1);
	pmbs->fChain->SetBranchStatus("OutputEvent.hit_FTle",1);
	pmbs->fChain->SetBranchStatus("OutputEvent.hit_CalE",1);

	pbrs->fChain->SetBranchStatus("*",0); 
	pbrs->fChain->SetBranchStatus("t_vetar",1);
	pbrs->fChain->SetBranchStatus("tsb_mpv",1);
	pbrs->fChain->SetBranchStatus("f11t_mpv",1);
	pbrs->fChain->SetBranchStatus("zet",1);
	pbrs->fChain->SetBranchStatus("aoq",1);




	ULong64_t ul_mbs=0;
	ULong64_t ul_brs=0;
	ULong64_t *pex, *vet, *mpv, *brs;
	ULong64_t *pex_f11;
	pex_f11 = &(ul_mbs);
	pex = &(pmbs->OutputEvent_WR_TS);
	vet = &(pbrs->t_vetar);
	mpv = &(pbrs->tsb_mpv);
	brs = &(pbrs->f11t_mpv);
	ULong64_t pex0=0, vet0=0, mpv0=0, brs0=0;


	Long64_t diff_pex_vet;
	Long64_t diff_brs_mpv;
	Long64_t offset_pex_vet = -150-94;
	Long64_t offset_brs_mpv = -150; 

	Bool_t first=1;
	UInt_t loop_counter=0;
	Long64_t skip_mbs=0, skip_brs=0;
	Long64_t miss_mbs=0, miss_brs=0;

	Bool_t ret_mbs;
	Bool_t ret_brs;





	TFile *fileout = new TFile(Form("ROOT/merged_mbs49/merged_mbs49_brsaccpt.root"),"recreate");
	TTree *treeout = new TTree("merged","merged");
	treeout->SetMaxTreeSize(2*1000*1000*1000);

	Int_t fTreeNumber_mbs;
	Int_t fTreeNumber_brs;
	Long64_t ient_local_mbs=0;
	Long64_t ient_local_brs=0;
	treeout->Branch("fTreeNumber_mbs",&fTreeNumber_mbs,"fTreeNumber_mbs/I");
	treeout->Branch("fTreeNumber_brs",&fTreeNumber_brs,"fTreeNumber_brs/I");
	treeout->Branch("ient_local_mbs",&ient_local_mbs,"ient_local_mbs/L");
	treeout->Branch("ient_local_brs",&ient_local_brs,"ient_local_brs/L");

	vector<Int_t> KL;
	vector<double> STOT, FTle, CalE;

	treeout->Branch("KL",&KL);
	treeout->Branch("STOT",&STOT);
	treeout->Branch("FTle",&FTle);
	treeout->Branch("CalE",&CalE);
	treeout->Branch("ppac_x",&ppac_x,"ppac_x/D");
	treeout->Branch("ppac_y",&ppac_y,"ppac_y/D");

	treeout->Branch("aoq",&(pbrs->aoq),"aoq/D");
	treeout->Branch("zet",&(pbrs->zet),"zet/D");
	//treeout->Branch("pidGood",&(pbrs->pidGood),"pidGood/I");

	treeout->Branch("ts_mbs",&(pmbs->OutputEvent_WR_TS),"ts_mbs/l");
	//treeout->Branch("ts_mbs_f11",&(ul_mbs),"ts_mbs/l");
	treeout->Branch("ts_vet",&(pbrs->t_vetar),"ts_vet/l");
	treeout->Branch("ts_mpv",&(pbrs->tsb_mpv),"ts_mpv/l");
	treeout->Branch("ts_brs",&(pbrs->f11t_mpv),"ts_brs/l");



	Long64_t ient_mbs=0;
	Long64_t ient_brs=0;
	for (
			ient_mbs=0, ient_brs=12307881;
			ient_mbs<nent_mbs && ient_brs<nent_brs;
			ient_mbs++, ient_brs++, loop_counter++
	    )

	{

		ret_mbs = pmbs->GetEntry(ient_mbs);
		ret_brs = pbrs->GetEntry(ient_brs);
		if ((loop_counter&0xffff)==0)
		{
			fprintf(stdout, "A ient_mbs %lld ient_brs %lld\tpex %llu vet %llu mpv %llu brs %llu\t diff_pex_vet %lld diff_brs_mpv %lld\n", ient_mbs,ient_brs, *pex,*vet,*mpv,*brs, diff_pex_vet, diff_brs_mpv);
		}

		if (ret_mbs)
		{
			fTreeNumber_mbs = ch_mbs->GetTreeNumber();
			ient_local_mbs = ch_mbs->GetTree()->GetReadEntry();

		}
		else 
		{
			fTreeNumber_mbs = -1;
			ient_local_mbs = -1;
			ient_mbs--;
		}
		if (ret_brs)
		{
			fTreeNumber_brs = ch_brs->GetTreeNumber();
			ient_local_brs = ch_brs->GetTree()->GetReadEntry();
		}
		else 
		{
			fTreeNumber_brs = -1;
			ient_local_brs = -1;
			ient_brs--;
		}


		diff_pex_vet = Long64_t(*pex-*vet - offset_pex_vet);
		diff_brs_mpv = Long64_t(*brs-*mpv - offset_brs_mpv);

		if ((loop_counter&0xffff)==0)
		{
			fprintf(stdout, "B ient_mbs %lld ient_brs %lld\tpex %llu vet %llu mpv %llu brs %llu\t diff_pex_vet %lld diff_brs_mpv %lld\n", ient_mbs,ient_brs, *pex,*vet,*mpv,*brs, diff_pex_vet, diff_brs_mpv);
		}

		bool good=1;

		if (ret_mbs && ret_brs)
		{
			Long64_t flag_pex_vet = diff_pex_vet/1000;


			if (flag_pex_vet > 0)
			{
				ient_mbs--;
				skip_brs++;
				good=0;
			}
			if (flag_pex_vet < 0)
			{
				skip_mbs++;
				ient_brs--;
				good=0;
			}

			if (!good) continue;

			h2_aoq_zet->Fill(pbrs->aoq, pbrs->zet);


			if (first)
			{
				pex0 = *pex;
				vet0 = *vet;
				mpv0 = *mpv;
				brs0 = *brs;
				first=0;
				fprintf(stdout, "\nFIRST: ient_mbs %lld ient_brs %lld\t", ient_mbs,ient_brs);
				fprintf(stdout, "pex %llu vet %llu mpv %llu brs %llu\n", *pex,*vet,*mpv,*brs);
			}
			//*pex-=pex0;
			//*vet-=vet0;
			//*mpv-=mpv0;
			//*brs-=brs0;

/////////// Do what you want below here ///////////
			int size = pmbs->OutputEvent_hit_PCHA.size();

			KL  . clear(); 
			STOT. clear();
			FTle. clear();
			CalE. clear();

			ppac_mask=0b0000;
			ppac_tx1=-999e20;
			ppac_tx2=-999e20;
			ppac_ty1=-999e20;
			ppac_ty2=-999e20;
			ppac_x=-999e20;
			ppac_y=-999e20;

			for (int idx=0; idx<size; idx++)
			{
				itam = pmbs->OutputEvent_hit_TAM[idx];
				icha = pmbs->OutputEvent_hit_PCHA[idx];
				istot = pmbs->OutputEvent_hit_STOT[idx];
				iftle = pmbs->OutputEvent_hit_FTle[idx];
				icale = pmbs->OutputEvent_hit_CalE[idx];
				iKL = icha + 16*itam;
				if (iKL>=0 && iKL<48)
				{
					KL.push_back(iKL);
					STOT.push_back(istot);
					FTle.push_back(iftle);
					CalE.push_back(icale);
				}

				if (itam== 6 && icha== 0) if (iftle > -1850e3 && iftle<1780e3)
				{
					for (int jdx=0; jdx<size; jdx++) if (pmbs->OutputEvent_hit_TAM[jdx]== 6) if (pmbs->OutputEvent_hit_FTle[jdx]-iftle > -5e3 && pmbs->OutputEvent_hit_FTle[jdx]-iftle < 900e3)
					{
						if (pmbs->OutputEvent_hit_PCHA[jdx]==1){    ppac_tx1 = pmbs->OutputEvent_hit_FTle[jdx]; ppac_mask = ppac_mask | 0b0001;}
						if (pmbs->OutputEvent_hit_PCHA[jdx]==6){    ppac_tx2 = pmbs->OutputEvent_hit_FTle[jdx]; ppac_mask = ppac_mask | 0b0010;}
						if (pmbs->OutputEvent_hit_PCHA[jdx]==5){    ppac_ty1 = pmbs->OutputEvent_hit_FTle[jdx]; ppac_mask = ppac_mask | 0b0100;}
						if (pmbs->OutputEvent_hit_PCHA[jdx]==4){    ppac_ty2 = pmbs->OutputEvent_hit_FTle[jdx]; ppac_mask = ppac_mask | 0b1000;}
					}

					if (ppac_mask==0b1111)
					{
						ppac_x =(-(ppac_tx1-ppac_tx2+74000)/(74000+85000)+0.5)*100; // mm
						ppac_y =( (ppac_ty1-ppac_ty2-76000)/(76000+76000)+0.5)*100;
						//ppac_x =-(ppac_tx1-ppac_tx2)/1.; // delay will be determined later
						//ppac_y = (ppac_ty1-ppac_ty2)/1.;
						//h2_ppac_position->Fill(ppac_x, ppac_y);
					}
				}




			}






			treeout->Fill();



			if (!(pbrs->aoq>2.086 && pbrs->aoq<2.091 && pbrs->zet>46.0 && pbrs->zet<46.7)) continue;



			/*b_veto=0;
			for (int idx=0; idx<size; idx++)
			{
				if (pmbs->OutputEvent_hit_TAM[idx]==5)
					if (pmbs->OutputEvent_hit_PCHA[idx]==1)
						//if (pmbs->OutputEvent_hit_FTle[idx] - pmbs->OutputEvent_hit_TTS[idx] < -100e3)
							b_veto=1;
			}
			if (b_veto) continue;*/


			/*
			   for (int idx=0; idx<size; idx++)
			{
				icha = pmbs->OutputEvent_hit_PCHA[idx];
				double istot = pmbs->OutputEvent_hit_STOT[idx];
				double iftle = pmbs->OutputEvent_hit_FTle[idx];
				//if (pmbs->OutputEvent_hit_TAM[idx]==TAM_VETO)
				//if (pmbs->OutputEvent_hit_TAM[idx]==TAM_GARi)
				if (pmbs->OutputEvent_hit_TAM[idx]==TAM_LaBr)
				{
					h2_STOT_FTle[icha]->Fill(istot,iftle);
				}
			}
			 */

			for (int idx=0; idx<size; idx++)
			{
				itam = pmbs->OutputEvent_hit_TAM[idx];
				icha = pmbs->OutputEvent_hit_PCHA[idx];
				istot = pmbs->OutputEvent_hit_STOT[idx];
				iftle = pmbs->OutputEvent_hit_FTle[idx];
				icale = pmbs->OutputEvent_hit_CalE[idx];
				iKL = icha + 16*itam;

				if(iKL>=0 && iKL<48)
				{
					h2_CalE_FTle_LaBr->Fill(icale,iftle);
				}

				/*for (int jdx=0; jdx<size; jdx++) if (pmbs->OutputEvent_hit_TAM[jdx]==TAM_LaBr)
				{
					jcha = pmbs->OutputEvent_hit_PCHA[jdx];
					jstot = pmbs->OutputEvent_hit_STOT[jdx];
					jftle = pmbs->OutputEvent_hit_FTle[jdx];
	

				}
				*/
			}


////////////////////// do what you want over here ///////////////

		}

	}
	fprintf(stdout, "ient_mbs %lld ient_brs %lld\tpex %llu vet %llu mpv %llu brs %llu\t diff_pex_vet %lld diff_brs_mpv %lld\n", ient_mbs,ient_brs, *pex,*vet,*mpv,*brs, *pex-*vet,*mpv-*brs);
	fprintf(stdout, "skip_mbs %lld skip_brs %lld\n", skip_mbs,skip_brs);














	///////////// Drawing ////////////////////
	TVirtualPad *pad;


	TCanvas *c1 = new TCanvas("c1","c1"); 
	pad = c1->cd(); 
	h2_CalE_FTle_LaBr->Draw("colz");

	TCanvas *c2 = new TCanvas("c2","c2"); 
	pad = c2->cd(); 
	h2_aoq_zet->Draw("colz");



	treeout->Write();
	h2_aoq_zet->Write();
	h2_CalE_FTle_LaBr->Write();
	fileout->Close();


}
