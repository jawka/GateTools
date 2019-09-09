#ifndef PHASESPACE_HH
#define PHASESPACE_HH
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
// C++ libraries
#include <iostream>
#include <fstream>
#include <algorithm>
#include <vector>
#include <sstream>
#include <stdexcept>
#include <cmath>
// Library
#include "Gamma.hh"
#include "Positron.hh"
#include "Proton.hh"

class PhaseSpace
{

public:

	// Constructor and destructor
	PhaseSpace(std::string ps_string, std::string scanner_mode, int analysis_mode, bool multiple_flag);
	~PhaseSpace();

	// Files' strings
	std::string file_name_extension = "ps_actor_phantom.root";
	std::string dir_path;	
	std::string file_name;
	std::string scanner_name;
	std::string file_name_extension_split = "ps_actor_phantom";
	std::string root_ext = ".root";
	
	// Counters
	Int_t annihilations;
	Int_t positron_no;
	Int_t gammas_no;
	Int_t entries;
	Int_t positrons_conv;
	Int_t positrons_RadioactiveDeacy;
	Int_t positrons_Decay;
	Int_t positrons_ePairProd;

	// "Flag" variables
	Int_t a_runID;
	Int_t a_eventID;
	Int_t a_trackID;
	Int_t a_parentID;
	Float_t a_X;
	Float_t a_Y;
	Float_t a_Z;
	Float_t a_time;
	Int_t a_nuclei;
	bool multiple_files_flag = false;

	// Annihilation variables
	bool at_rest = true;
	float e_low = 0.51;
	float e = 511.;
	float e_high = 0.512;
	float e1 = 0.;
	float e2 = 0.;
	int rest1 = 0;
	int rest2 = 0;

	// Leaves initialization - TTree PhaseSpace	
	Int_t		AtomicNumber;
	Float_t		Ekine;
	Float_t		Weight;
	Double_t	Time;
	Int_t		Mass;
	Float_t		X;
	Float_t		Y;
	Float_t		Z;
	Float_t		dX;
	Float_t		dY;
	Float_t		dZ;
	Char_t		ProductionVolume[256];
	Char_t		CreatorProcess[256];
	Char_t		ProcessDefinedStep[256];
	Int_t		TrackID;
	Int_t		ParentID;
	Int_t		EventID;
	Int_t		RunID;
	Float_t		EmissionPointX;
	Float_t		EmissionPointY;
	Float_t		EmissionPointZ;
	Int_t		PDGCode;
	Char_t		ParticleName[256];
	Float_t		TOut;
	Float_t		TProd;

	// Processess' strings
	std::string prod_process = "RadioactiveDecay";
	std::string conv = "conv";
	std::string RadioactiveDecay = "RadioactiveDecay";
	std::string Decay = "Decay";
	std::string ePairProd = "ePairProd";
	std::string protonInelastic = "protonInelastic";
	std::string annihil_process = "annihil";
	
	// Particles' strings
	std::string electron = "e-";
	std::string positron = "e+";
	std::string gamma = "gamma";
	std::string nuclei_name;

	// Additional variables
	int mode = 0;
	Int_t temp_event;
	Int_t nuclei_event;
	Int_t nuclei_track;
	bool flag_annihil = false;
	bool nuclei_prod = false;
	bool flag_gamma1 = false;
	int trackID_gamma1 = 0;
	int trackID_gamma2 = 0;
	int verify = 0;

	// Vectors
	std::vector<int> annihil_nuclei;
	std::vector<Gamma> annihil_gammas;
	std::vector<Positron> positrons;
	std::vector<Proton> protons;
	std::vector<std::string> radioactiveNuclei;

	// ROOT variables
	TTree* PhaseSpaceActor;
	TFile* f;

	// Basic methods
	void init(int files_no);
	void closeRootFile();

	// Analysis methods
	void analysisAnnihilationGammas();
	void analysisPromptGammas();
	void analysisGenericIonsAndAnnihilation();
	void analysisVHEEGammas();
	void analysisTIMEPIX();
	int gammasVerification(std::vector<Gamma> gammas); //clear last elements in vectors if gammas doesn't follow the at_rest/in_fly condition

	// Getters
	std::vector<int>* getNuclei();
	std::vector<Positron>* getPositrons();
	std::vector<Proton>* getProtons();




};
#endif // PHASESPACE_HH









