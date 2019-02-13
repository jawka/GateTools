#ifndef HITSROOT_HH
#define HITSROOT_HH
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
#include <stdexcept>
#include <cmath>
#include <cstdlib>
// Library
#include "Hit.hh"
#include "Positron.hh"

class HitsRoot
{

public:

	// Constructor and destructor
	HitsRoot(std::string ro_string, std::string scanner_mode, int analysis_mode);
	~HitsRoot();

	// Files' strings
	std::string file_name_extension = "protonTest.root";	
	std::string scanner_name;
	std::string file_name;
	std::string res_mode;

	int mode=0;

	// Counters
	Int_t entries;
	Int_t iterr;
	Int_t j;
	Int_t vec_size;
	Int_t gamma_no;

	// Annihilation photons counters
	Int_t none;
	Int_t one;
	Int_t two;

	// "Flag" variables
	Int_t a_runID;
	Int_t a_eventID;
	Int_t a_trackID;
	Int_t a_parentID;
	Float_t a_X;
	Float_t a_Y;
	Float_t a_Z;
	Float_t a_time;
	Int_t a_trackID_gamma1;
	Int_t a_trackID_gamma2;

	// Leaves initialization - TTree HitsChain
	Int_t		PDGEncoding;
	Int_t		trackID;
	Int_t		parentID;
	Double_t	trackLocalTime;
	Double_t	time;
	Float_t		edep;
	Float_t		stepLength;
	Float_t		trackLength;
	Float_t		posX;
	Float_t		posY;
	Float_t		posZ;
	Float_t		localPosX;
	Float_t		localPosY;
	Float_t		localPosZ;
	Float_t		momDirX;
	Float_t		momDirY;
	Float_t		momDirZ;
	Int_t		baseID;
	Int_t		level1ID;
	Int_t		level2ID;
	Int_t		level3ID;
	Int_t		level4ID;
	Int_t		level5ID;
	Int_t		layerID;
	Int_t		photonID;
	Int_t		nPhantomCompton;
	Int_t		nCrystalCompton;
	Int_t		nPhantomRayleigh;
	Int_t		nCrystalRayleigh;
	Int_t		primaryID;
	Float_t		sourcePosX;
	Float_t		sourcePosY;
	Float_t		sourcePosZ;
	Int_t		sourceID;
	Int_t		eventID;
	Int_t		runID;
	Float_t		axialPos;
	Float_t		rotationAngle;
	Int_t		volumeID;
	Char_t		processName[20];
	Char_t		comptVolName[5];
	Char_t		RayleighVolName[5];

	// Analysis variables	
	Int_t compton_no = 1;	
	Int_t temp_trackID = 0;
	Int_t temp_eventID = 0;	
	Int_t interspill_counter = 0;	
	Int_t inbeam_counter = 0;	
	Float_t time_slice = 1000000.; //1 ms in nanoseconds	
	Float_t no_primaries = 10000000.;
	Float_t light_velocity = 300.; //mm per ns
	Float_t delta_length = 0.;	
	Float_t LOR_length = 0.;	
	Float_t scaling_param = 0.;	
	Float_t scaling_param_sfr = 0.;	

	
	// Radiation phases counters
	Int_t inbeam_scattered = 0;
	Int_t interspill_scattered = 0;
	Int_t offbeam_scattered = 0;
	Int_t inbeam_non_scattered = 0;
	Int_t interspill_non_scattered = 0;
	Int_t offbeam_non_scattered = 0;

	// Blurring sigmas for z position estimation
	TRandom *r = new TRandom3();
	Float_t sigma_blurring_x; //in mm
	Float_t sigma_blurring_y; //in mm
	Float_t sigma_blurring_z; //in mm
	Float_t sigma_blurring_t; //in ns

	// Prim values of the x, y, z, t
	Float_t x1; //in mm
	Float_t x2; //in mm
	Float_t y1; //in mm
	Float_t y2; //in mm
	Float_t z1; //in mm
	Float_t z2; //in mm
	Float_t t1; //in s
	Float_t t2; //in s
	Float_t x_prim; //in mm
	Float_t y_prim; //in mm
	Float_t z_prim; //in mm
	Float_t sfr_y; //in mm

	// ROOT variables
	TFile* f;
	TTree* HitsChain;

	// Vectors
	std::vector<Positron>* positrons;
	std::vector<Hit> coincidence_hits;

	// Basic methods
	void Reset();
	void closeRootFile();
	void init();
	void setTempElements();

	// Analysis methods
	bool scatteringFlag(std::vector<Hit> vec_hits);
	float z_range_estimation(std::vector<Hit> vec_hits);
	float z_straightforward_recon(std::vector<Hit> vec_hits);
	int hitsVectorAnalysis(std::vector<Hit> vec_hits, TH1F* annihil_gamma_energy_deposition_spectrum);
	void analysis();
	void spectra_energy_analysis();

	// Setters
	void setPositrons(std::vector<Positron>* vec_positrons);



};
#endif // HITSROOT_HH









