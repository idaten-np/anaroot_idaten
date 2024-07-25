#include <iostream>
#include <string>
#include "TArtStoreManager.hh"
#include "TArtEventStore.hh"
#include "TArtBigRIPSParameters.hh"
#include "TArtDALIParameters.hh"
#include "TArtCalibPID.hh"
#include "TArtCalibDALI.hh"
#include "TArtCalibPPAC.hh"
#include "TArtCalibPlastic.hh"
#include "TArtEventInfo.hh"
#include "TArtPlastic.hh"
#include "TArtPPAC.hh"
#include "TArtRecoPID.hh"
#include "TArtRecoRIPS.hh"
#include "TArtRecoTOF.hh"
#include "TArtRecoBeam.hh"
#include "TArtRIPS.hh"

#include "TArtRawEventObject.hh"
#include "TArtRawSegmentObject.hh"
#include "TArtRawDataObject.hh"


#include "TSystem.h"
#include "TTree.h"
#include "TFile.h"

#include "TClonesArray.h"

#include "TVector3.h"

#include "signal.h"

using namespace std;

/** prints usage **/
void usage(char *argv0)
{
  std::cout << "[MakeFullBigRIPSTree]: Usage: "
	    << argv0 << "-i [input_file] -o [output_file]"
	    << std::endl;
}

int main(int argc, char **argv)
{
  std::string input_file_name;
  std::string output_file_name("output.root");

  /** parsing commandline arguments **/
  int opt = 0;
  while ((opt = getopt(argc, argv, "o:i:")) != -1)
    {
      switch (opt)
        {
        case 'o':
	  output_file_name = optarg;
	  break;
        case 'i':
	  input_file_name = optarg;
	  break;
        default:
	  usage(argv[0]);
	  return 1;
	  break;
        }
    }

  if (input_file_name.empty())
    {
      usage(argv[0]);
      return 1;
    }

  //  signal(SIGINT,stop_interrupt); // CTRL + C , interrupt

  // Create StoreManager both for calibration "TArtCalib..." and treatment "TArtReco..."
  //------------------------------------------------------------------------------------
  TArtStoreManager *sman = TArtStoreManager::Instance();

  // Create EventStore to control the loop and get the EventInfo
  //------------------------------------------------------------
  TArtEventStore *estore = new TArtEventStore();
  // estore->SetInterrupt(&stoploop);
  estore->Open(input_file_name.c_str());
  std::cout << "estore ->" << input_file_name << std::endl;

  // Create BigRIPSParameters to get Plastics, PPACs, ICs and FocalPlanes parameters from ".xml" files
  //--------------------------------------------------------------------------------------------------
  TArtBigRIPSParameters *para = TArtBigRIPSParameters::Instance();
  para->LoadParameter("db/BigRIPSPPAC.xml");
  para->LoadParameter("db/BigRIPSPlastic.xml");
  para->LoadParameter("db/BigRIPSIC.xml");
  para->LoadParameter("db/FocalPlane.xml");
  para->SetFocusPosOffset(8, 231);

  // Create CalibPID to get and calibrate raw data ( CalibPID ->
  //[CalibPPAC , CalibIC, CalibPlastic , CalibFocalPlane]
  TArtCalibPID *brcalib = new TArtCalibPID();
  TArtCalibPPAC *ppaccalib = brcalib->GetCalibPPAC();
  TArtCalibPlastic *plasticcalib = brcalib->GetCalibPlastic();

  // Create RecoPID to get calibrated data and to reconstruct TOF, AoQ, Z, ... (RecoPID ->
  //[ RecoTOF , RecoRIPS , RecoBeam] )
  TArtRecoPID *recopid = new TArtRecoPID();

  para->PrintListOfPPACPara();
  // return;

  // Definition of observables we want to reconstruct
  std::cout << "Defining bigrips parameters" << std::endl;

  // modified by Kasia, 2015-04-06
  TArtRIPS *rips3to5 = recopid->DefineNewRIPS(3, 5, "matrix/mat1.mat", "D3");                    // F3 - F5
  TArtRIPS *rips5to7 = recopid->DefineNewRIPS(5, 7, "matrix/mat2.mat", "D5");                    // F5 - F7
  //TArtRIPS *rips8to10 = recopid->DefineNewRIPS(8, 10, "matrix/F8F10_LargeAccAchr.mat", "D7");    // F8 - F10
  //TArtRIPS *rips10to11 = recopid->DefineNewRIPS(10, 11, "matrix/F10F11_LargeAccAchr.mat", "D8"); // F10 - F11

  //rips3to5->SetCenterBrho(4.3547);
  //rips5to7->SetCenterBrho(3.8693);

  // Reconstruction of TOF DefineNewTOF(fisrt plane, second plane, time offset)
  TArtTOF *tof3to7 = recopid->DefineNewTOF("F3pl", "F7pl", 295.0, 5); // F3 - F7
  //TArtTOF *tof3to7 = recopid->DefineNewTOF("F3pl", "F7pl", 301.7, 5); // F3 - F7
  // TArtTOF *tof8to11 = recopid->DefineNewTOF("F8pl","F11pl-1",199.4,10); // F8 - F11
  //TArtTOF *tof8to11 = recopid->DefineNewTOF("F8pl", "F11long", -108.5, 10); // F8 - F11

  // Reconstruction of IC observables for ID
  TArtBeam *beam_br_35 = recopid->DefineNewBeam(rips3to5, tof3to7, "F7IC");
  TArtBeam *beam_br_57 = recopid->DefineNewBeam(rips5to7, tof3to7, "F7IC");
  TArtBeam *beam_br_37 = recopid->DefineNewBeam(rips3to5, rips5to7, tof3to7, "F7IC");
  TArtBeam *beam_br_37_11 = recopid->DefineNewBeam(rips3to5, rips5to7, tof3to7, "F11IC");
  //TArtBeam *beam_zd_810 = recopid->DefineNewBeam(rips8to10, tof8to11, "F11IC");
  //TArtBeam *beam_zd_1011 = recopid->DefineNewBeam(rips10to11, tof8to11, "F11IC");
  //TArtBeam *beam_zd_811 = recopid->DefineNewBeam(rips8to10, rips10to11, tof8to11, "F11IC");

  // to get trigger pattern
  TArtEventInfo *evtinfo = new TArtEventInfo();
  int trg = -777;
  cout << "trigger " << trg << endl;

  TFile *fout = new TFile(output_file_name.c_str(), "RECREATE");
  TTree *tree = new TTree("tree", "tree");

  // define data nodes which are supposed to be dumped to tree
  TClonesArray *info_array = (TClonesArray *)sman->FindDataContainer("EventInfo");
  std::cout << info_array->GetName() << std::endl;
  tree->Branch(info_array->GetName(), &info_array);

  TClonesArray *ppac_array =
    (TClonesArray *)sman->FindDataContainer("BigRIPSPPAC");
  std::cout << ppac_array->GetName() << std::endl;
  tree->Branch(ppac_array->GetName(), &ppac_array);

  TClonesArray *pla_array =
    (TClonesArray *)sman->FindDataContainer("BigRIPSPlastic");
  tree->Branch(pla_array->GetName(), &pla_array);

  TClonesArray *ic_array =
    (TClonesArray *)sman->FindDataContainer("BigRIPSIC");
  tree->Branch(ic_array->GetName(), &ic_array);

  TClonesArray *fpl_array =
    (TClonesArray *)sman->FindDataContainer("BigRIPSFocalPlane");
  tree->Branch(fpl_array->GetName(), &fpl_array);

  // PID reconstructed data:
  TClonesArray *rips_array =
    (TClonesArray *)sman->FindDataContainer("BigRIPSRIPS");
  std::cout << rips_array->GetName() << std::endl;
  tree->Branch(rips_array->GetName(), &rips_array);

  TClonesArray *tof_array =
    (TClonesArray *)sman->FindDataContainer("BigRIPSTOF");
  std::cout << tof_array->GetName() << std::endl;
  tree->Branch(tof_array->GetName(), &tof_array);

  TClonesArray *beam_array =
    (TClonesArray *)sman->FindDataContainer("BigRIPSBeam");
  std::cout << beam_array->GetName() << std::endl;
  tree->Branch(beam_array->GetName(), &beam_array);
  ULong64_t f11t_mpv = 0;
  ULong64_t t_vetar = 0;
  ULong64_t tsb_mpv = 0;

  tree->Branch("f11t_mpv", &f11t_mpv, "f11t_mpv/l");
  tree->Branch("t_vetar", &t_vetar, "t_vetar/l");
  tree->Branch("tsb_mpv", &tsb_mpv, "tsb_mpv/l");

  int neve = 0;
  //    while(estore->GetNextEvent()&& neve<1e4)
  while (estore->GetNextEvent())
    {  


      if ((neve&0x3ff) == 0)
	std::cout << "event: " << neve << std::endl;

      //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
      // Making the BigRIPS tree calibration
      brcalib->ClearData();
      brcalib->ReconstructData();
      // Reconstructiong the PID
      recopid->ClearData();
      recopid->ReconstructData();

      f11t_mpv = 0;
      t_vetar = 0;
      tsb_mpv = 0;
      TArtRawEventObject* reo = estore->GetRawEventObject();
      for(int j = 0; j < reo->GetNumSeg(); j++){
	TArtRawSegmentObject* rso = reo->GetSegment(j);
	if (j==12 && rso->GetDevice()==1 && rso->GetFP()==41 && rso->GetDetector()==60 && rso->GetModule()==8){
	  TArtRawDataObject* rdo0 = rso->GetData(0);
	  TArtRawDataObject* rdo1 = rso->GetData(1);
	  ULong64_t ts0 = rdo0->GetVal();
	  ULong64_t ts1 = rdo1->GetVal();
		  
	  //	  std::cout<<"EFN:68(F11 MPV), "<<rdo0->GetVal()<<std::endl;
	  //	  f11t_mpv = rdo->GetVal()*10;
	  //	  std::cout<<"EFN:68(F11 MPV), "<<(ts0<<32)+ts1<<std::endl;
	  f11t_mpv = ((ts1<<32)+ts0)*10;
	}
	else if (j==13 && rso->GetDevice()==1 && rso->GetFP()==11 && rso->GetDetector()==60 && rso->GetModule()==62){
	  TArtRawDataObject* rdo0 = rso->GetData(0);
	  TArtRawDataObject* rdo1 = rso->GetData(1);
	  ULong64_t ts0 = rdo0->GetVal();
	  ULong64_t ts1 = rdo1->GetVal();
	  //	std::cout<<"EFN:81(WR Vetar), "<<rdo->GetVal()<<std::endl;
	  //	  t_vetar = rdo->GetVal()*8;
	  //	  std::cout<<"EFN:81(WR Vetar), "<<(ts0<<32)+ts1<<std::endl;
	  t_vetar = ((ts1<<32)+ts0)*8;
	}
	else if (j==14 && rso->GetDevice()==0 && rso->GetFP()==3 && rso->GetDetector()==60 && rso->GetModule()==8){
	  TArtRawDataObject* rdo0 = rso->GetData(0);
	  TArtRawDataObject* rdo1 = rso->GetData(1);
	  ULong64_t ts0 = rdo0->GetVal();
	  ULong64_t ts1 = rdo1->GetVal();
	  
	  //	std::cout<<"EFN:81(TSB MPV), "<<rdo->GetVal()<<std::endl;
	  //	  tsb_mpv = rdo->GetVal()*10;
	  //	  std::cout<<"EFN:81(TSB MPV), "<<(ts0<<32)+ts1<<std::endl;
	  tsb_mpv = ((ts1<<32)+ts0)*10;
	}
      }	

      tree->Fill();
      neve++;
    }
  cout << "Writing the tree." << endl;

  fout->Write();
  fout->Close();
  return 0;
}
