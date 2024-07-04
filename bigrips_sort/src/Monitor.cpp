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
#include "TH1D.h"
#include "TCanvas.h"
#include "TArtBeam.hh"
#include "TArtIC.hh"

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
    std::cout << "[Monitor]: Usage: "
              << argv0 << "-i [input_file]"
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
    para->LoadParameter("db/BigRIPSPPACnogate.xml");
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


    // define data nodes which are supposed to be dumped to tree
    TClonesArray *info_array = (TClonesArray *)sman->FindDataContainer("EventInfo");
    std::cout << info_array->GetName() << std::endl;

    TClonesArray *ppac_array =
        (TClonesArray *)sman->FindDataContainer("BigRIPSPPAC");
    std::cout << ppac_array->GetName() << std::endl;

    TClonesArray *pla_array =
        (TClonesArray *)sman->FindDataContainer("BigRIPSPlastic");

    TClonesArray *ic_array =
        (TClonesArray *)sman->FindDataContainer("BigRIPSIC");

    TClonesArray *fpl_array =
        (TClonesArray *)sman->FindDataContainer("BigRIPSFocalPlane");

    // PID reconstructed data:
    TClonesArray *rips_array =
        (TClonesArray *)sman->FindDataContainer("BigRIPSRIPS");
    std::cout << rips_array->GetName() << std::endl;

    TClonesArray *tof_array =
        (TClonesArray *)sman->FindDataContainer("BigRIPSTOF");
    std::cout << tof_array->GetName() << std::endl;

    TClonesArray *beam_array =
        (TClonesArray *)sman->FindDataContainer("BigRIPSBeam");

    int neve = 0;
    //  while(estore->GetNextEvent()&& neve<1000){
    std::vector<int> counts(26,0);
    std::vector<std::string> bin_titles = {"f3pl","f7pl","f3ppac_1a","f3ppac_1b","f3ppac_2a","f3ppac_2b","f5ppac_1a","f5ppac_1b","f5ppac_2a","f5ppac_2b","f7ppac_1a","f7ppac_1b","f7ppac_2a","f7ppac_2b","f7ic_0","f7ic_1","f7ic_2","f7ic_3","f7ic_4","f7ic_5","f11ic_0","f11ic_1","f11ic_2","f11ic_3","f11ic_4","f11ic_5"};
    auto add_count = [&counts](const double & val,int id){ if(val>0) counts[id] = counts[id] + 1;};
    TH1D hist("bigrips","BigRIPS detector efficiencies",26,-0.5,25.5);
    while (estore->GetNextEvent())
    {
        if (neve > 200000)
            break;
        if (neve % 1000 == 0)
            std::cout << "event: " << neve << std::endl;

        //%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%
        // Making the BigRIPS tree calibration
        brcalib->ClearData();
        brcalib->ReconstructData();
        // Reconstructiong the PID
        recopid->ClearData();
        recopid->ReconstructData();
	auto beam = (TArtBeam*)beam_array->At(2);
	auto aoq = beam->GetAoQ();
	auto zet = beam->GetZet();
	if (zet < 48 || zet > 55 || aoq < 2 || aoq > 2.1)
	    continue;

	auto f3pla = (TArtPlastic*)pla_array->At(0);
	auto f7pla = (TArtPlastic*)pla_array->At(2);
	add_count(f3pla->GetTime(),0);	
	add_count(f7pla->GetTime(),1);	
	auto f3ppac_1a = (TArtPPAC*)ppac_array->At(4);
	auto f3ppac_1b = (TArtPPAC*)ppac_array->At(5);
	auto f3ppac_2a = (TArtPPAC*)ppac_array->At(6);
	auto f3ppac_2b = (TArtPPAC*)ppac_array->At(7);
	auto f5ppac_1a = (TArtPPAC*)ppac_array->At(9);
	auto f5ppac_1b = (TArtPPAC*)ppac_array->At(10);
	auto f5ppac_2a = (TArtPPAC*)ppac_array->At(11);
	auto f5ppac_2b = (TArtPPAC*)ppac_array->At(12);
	auto f7ppac_1a = (TArtPPAC*)ppac_array->At(14);
	auto f7ppac_1b = (TArtPPAC*)ppac_array->At(15);
	auto f7ppac_2a = (TArtPPAC*)ppac_array->At(16);
	auto f7ppac_2b = (TArtPPAC*)ppac_array->At(17);
	add_count(f3ppac_1a->GetTSumX(),2);	
	add_count(f3ppac_1b->GetTSumX(),3);	
	add_count(f3ppac_2a->GetTSumX(),4);	
	add_count(f3ppac_2b->GetTSumX(),5);	
	add_count(f5ppac_1a->GetTSumX(),6);	
	add_count(f5ppac_1b->GetTSumX(),7);	
	add_count(f5ppac_2a->GetTSumX(),8);	
	add_count(f5ppac_2b->GetTSumX(),9);	
	add_count(f7ppac_1a->GetTSumX(),10);	
	add_count(f7ppac_1b->GetTSumX(),11);	
	add_count(f7ppac_2a->GetTSumX(),12);	
	add_count(f7ppac_2b->GetTSumX(),13);	
	auto f7ic = (TArtIC*)ic_array->At(0);
	auto f11ic = (TArtIC*)ic_array->At(1);
	add_count(f7ic->GetRawADC(0)-2000,14);	
	add_count(f7ic->GetRawADC(1)-2000,15);	
	add_count(f7ic->GetRawADC(2)-2000,16);	
	add_count(f7ic->GetRawADC(3)-2000,17);	
	add_count(f7ic->GetRawADC(4)-2000,18);	
	add_count(f7ic->GetRawADC(5)-2000,19);	
	add_count(f11ic->GetRawADC(0)-300,20);	
	add_count(f11ic->GetRawADC(1)-300,21);	
	add_count(f11ic->GetRawADC(2)-300,22);	
	add_count(f11ic->GetRawADC(3)-300,23);	
	add_count(f11ic->GetRawADC(4)-300,24);	
	add_count(f11ic->GetRawADC(5)-300,25);	

        neve++;
    }
    int xid = 0;
    int f7 = counts.at(1);
    std::cout << "total f7 counts: " << f7 << std::endl;
    for(const auto &count : counts)
    {
   	double ratio = (double)count/(double)f7; 
        std::cout << bin_titles[xid] << "\t" << ratio << std::endl;
	hist.Fill(xid,ratio);
        xid++;
    }

    TCanvas c1("canv");
    hist.Draw("hist");
    std::string pngname = input_file_name + ".png";
    c1.Print(pngname.c_str()); 

    return 0;
}
