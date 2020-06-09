#ifndef COINCIDENCES_HH
#define COINCIDENCES_HH
// ROOT libraries
#include <TROOT.h>
#include <TFile.h>
#include <TTree.h>
#include <TH2.h>
#include <TCanvas.h>
#include <TStyle.h>
#include <TImage.h>
#include <TList.h>
#include <TVector.h>
#include <TRandom.h>
#include <TRandom1.h>
#include <TRandom2.h>
#include <TRandom3.h>
// C++ libraries
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <sstream>
#include <string.h>
#include <stdexcept>
#include <cmath>
#include <cstdlib>
// Library

class Coincidences
{

public:

	// Constructor and destructor
	Coincidences(std::string dir_path, std::string scanner_mode, int analysis_mode);
	~Coincidences();

	// Files' strings
	std::string default_file_name = "sensitivity_results.root";	// !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	std::string scanner_name;
	std::string file_no;
	std::string old_file_name;
	std::string file_name_for_number;
	std::string new_file_name;
	std::string res_mode;



	// Counters
	Int_t entries;
	Int_t iterr;
	Int_t rsector_barrel; // in fact half of rsector
	Int_t dualhead_crystal; // in fact 13; 12 due to python numbering
	Int_t dualhead_module; // in fact one more; as it is due to python numbering


	// Original variables
	Float_t		osourcePosX1;
	Float_t		osourcePosY1;
	Float_t		osourcePosZ1;
	Float_t		osourcePosX2;
	Float_t		osourcePosY2;
	Float_t		osourcePosZ2;
	Float_t		oglobalPosX1;
	Float_t		oglobalPosY1;
	Float_t		oglobalPosZ1;
	Float_t		oglobalPosX2;
	Float_t		oglobalPosY2;
	Float_t		oglobalPosZ2;
	Int_t		orsectorID1;
	Int_t		orsectorID2;
	Int_t		omoduleID1;
	Int_t		omoduleID2;
	Int_t		ocrystalID1;
	Int_t		ocrystalID2;
	Int_t		olayerID1;
	Int_t		olayerID2;

	// Leaves initialization - TTree CoincidencesChain
	Int_t		runID;
	Float_t         axialPos;
	Float_t         rotationAngle;
	Int_t           eventID1;
	Int_t           sourceID1;
	Float_t         sourcePosX1;
	Float_t         sourcePosY1;
	Float_t         sourcePosZ1;
	Double_t        time1;
	Float_t         energy1;
	Float_t         globalPosX1;
	Float_t         globalPosY1;
	Float_t         globalPosZ1;
	Int_t           gantryID1;
	Int_t           rsectorID1;
	Int_t           moduleID1;
	Int_t           submoduleID1;
	Int_t           crystalID1;
	Int_t           layerID1;
	Int_t           comptonPhantom1;
	Int_t           comptonCrystal1;
	Int_t           RayleighPhantom1;
	Int_t           RayleighCrystal1;
	Int_t           eventID2;
	Int_t           sourceID2;
	Float_t         sourcePosX2;
	Float_t         sourcePosY2;
	Float_t         sourcePosZ2;
	Double_t 	time2;
	Float_t         energy2;   
	Float_t         globalPosX2;
	Float_t         globalPosY2;      
	Float_t         globalPosZ2;
	Int_t           gantryID2;
	Int_t           rsectorID2;
	Int_t           moduleID2;
	Int_t           submoduleID2;
	Int_t           crystalID2;
	Int_t           layerID2;
	Int_t           comptonPhantom2;
	Int_t           comptonCrystal2;   
	Int_t           RayleighPhantom2;
	Int_t           RayleighCrystal2;
	Float_t         sinogramS;
	Float_t         sinogramTheta;
	Char_t          comptVolName1[40];
	Char_t          comptVolName2[40];
	Char_t          RayleighVolName1[40];
	Char_t          RayleighVolName2[40];


	// Analysis variables	
	Float_t inbeam_length = 1000000.; //1 ms in nanoseconds	
	bool barrel_flag;
	bool debug = false;
	int mode=0;

	// Inbeam/offbeam counters
	Int_t inbeam = 0;
	Int_t offbeam = 0;

	// Blurring sigmas for z position estimation
	TRandom *r = new TRandom3();
	Float_t sigma_blurring = 11.; //in mm

	// ROOT variables
	TFile* f;
	TTree* CoincidencesChain;

	// Basic methods
	void Reset();
	void closeRootFile();
	void init();

	// Analysis methods
	void extend_sensitivity_FOV();

};
#endif // COINCIDENCES_HH









