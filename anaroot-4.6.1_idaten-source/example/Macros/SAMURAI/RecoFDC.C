
void RecoFDC(char *ridffile, Int_t nanaeve=10000.){

  TArtEventStore *estore = new TArtEventStore;
  estore->Open(ridffile);

  TArtSAMURAIParameters *samuraiparameters = TArtSAMURAIParameters::Instance();
  samuraiparameters->LoadParameter("db/SAMURAIFDC1.xml");
  samuraiparameters->LoadParameter("db/SAMURAIFDC2.xml");

  TArtCalibFDC1Hit *fCalibFDC1Hit = new TArtCalibFDC1Hit;
  TArtCalibFDC2Hit *fCalibFDC2Hit = new TArtCalibFDC2Hit;
  TArtCalibFDC1Track *fCalibFDC1Track = new TArtCalibFDC1Track;
  TArtCalibFDC2Track *fCalibFDC2Track = new TArtCalibFDC2Track;

  TArtStoreManager * sman = TArtStoreManager::Instance();
  TClonesArray *fdc1hits = (TClonesArray *)sman->FindDataContainer("SAMURAIFDC1Hit");
  TClonesArray *fdc2hits = (TClonesArray *)sman->FindDataContainer("SAMURAIFDC2Hit");
  TClonesArray *fdc1trks = (TClonesArray *)sman->FindDataContainer("SAMURAIFDC1Track");
  TClonesArray *fdc2trks = (TClonesArray *)sman->FindDataContainer("SAMURAIFDC2Track");

  char dummy[64];
  char title[64];

  TFile *fdcin = new TFile("db/dc/dayone0098.root","READ"); 
  gROOT->cd();
  TH1F *hist = NULL;

  for(int i=0;i<7;i++){
    sprintf(dummy,"fdc1_ftdc_corr_%d",i);

    hist = (TH1F*)((TH2F*)fdcin->Get(dummy))->ProjectionX();
    fCalibFDC1Track->SetTDCDistribution(hist,i*2);
    delete hist; hist = NULL;
    hist = (TH1F*)((TH2F*)fdcin->Get(dummy))->ProjectionY();
    fCalibFDC1Track->SetTDCDistribution(hist,i*2+1);
    delete hist; hist = NULL;

    sprintf(dummy,"fdc2_ftdc_corr_%d",i);

    hist = (TH1F*)((TH2F*)fdcin->Get(dummy))->ProjectionX();
    fCalibFDC2Track->SetTDCDistribution(hist,i*2);
    delete hist; hist = NULL;
    hist = (TH1F*)((TH2F*)fdcin->Get(dummy))->ProjectionY();
    fCalibFDC2Track->SetTDCDistribution(hist,i*2+1);
    delete hist; hist = NULL;
  }

  // making fdc1 histograms 
  int numlayer = 14;
  int numwire = 32;
  TH2 *hfdc1_ch; // channel distribution
  TH2 *hfdc1_tdc[14]; // tdc distribution for 14 layers

  hfdc1_ch  = new TH2F("fdc1_ch","fdc1 channel distribution",numwire,-0.5,numwire-0.5,numlayer,-0.5,numlayer-0.5);
  for(int i=0;i<numlayer;i++){
    sprintf(dummy,"fdc1_tdc_l%02d",i);
    sprintf(title,"fdc1 tdc distribution for layer-%02d",i);
    hfdc1_tdc[i]  = new TH2F(dummy,title,2000,0,2000,numwire,-0.5,numwire-0.5);
  }

  // making fdc2 histograms 
  numlayer = 14;
  numwire = 112;

  TH2 *hfdc2_ch; // channel distribution
  TH2 *hfdc2_tdc[14]; // tdc distribution for 14 layers

  hfdc2_ch  = new TH2F("fdc2_ch","fdc2 channel distribution",numwire,-0.5,numwire-0.5,numlayer,-0.5,numlayer-0.5);
  for(int i=0;i<numlayer;i++){
    sprintf(dummy,"fdc2_tdc_l%02d",i);
    sprintf(title,"fdc2 tdc distribution for layer-%02d",i);
    hfdc2_tdc[i]  = new TH2F(dummy,title,2000,0,2000,numwire,-0.5,numwire-0.5);
  }

  auto hfdc2trk_chi2 = new TH1F("fdc2trk_chi2","fdc2trk_chi2",100,0,100);

  // start to analyze

  Int_t neve = 0;

  while(estore->GetNextEvent() && neve<nanaeve){

    fCalibFDC1Hit->ClearData();
    fCalibFDC2Hit->ClearData();
    fCalibFDC1Track->ClearData();
    fCalibFDC2Track->ClearData();

    fCalibFDC1Hit->ReconstructData();
    fCalibFDC2Hit->ReconstructData();
    fCalibFDC1Track->ReconstructData();
    fCalibFDC2Track->ReconstructData();

    //
    // fdc1
    //

    if(fdc1hits){
      for(int i=0;i<fdc1hits->GetEntries();i++){

	TArtDCHit *hit = (TArtDCHit *)fdc1hits->At(i);
	int layer = hit->GetLayer();
	int wireid = hit->GetWireID();
	int val = hit->GetTDC();

	hfdc1_tdc[layer]->Fill(val,wireid);
	hfdc1_ch->Fill(wireid,layer);

      }
    } // end of if(fdc1hits){

    //
    // fdc2
    //

    if(fdc2hits){
      for(int i=0;i<fdc2hits->GetEntries();i++){

	TArtDCHit *hit = (TArtDCHit *)fdc2hits->At(i);
	int layer = hit->GetLayer();
	int wireid = hit->GetWireID();
	int val = hit->GetTDC();

	hfdc2_tdc[layer]->Fill(val,wireid);
	hfdc2_ch->Fill(wireid,layer);

      }
    } // end of if(fdc2hits){

    if(fdc2trks){

      if(fdc2trks->GetEntries()>0){
	TArtDCTrack *trk = (TArtDCTrack *)fdc2trks->At(0);
	Double_t chi2 = trk->GetChi2();
	hfdc2trk_chi2->Fill(chi2);
      }

    } // end of if(fdc2trks){

  } // end of while(estore->GetNextEvent() && neve<nanaeve){ 

}

