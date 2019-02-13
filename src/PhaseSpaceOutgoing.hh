#ifndef PHASESPACEOUTGOING_HH
#define PHASESPACEOUTGOING_HH
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

class PhaseSpaceOutgoing
{

public:

	// Constructor and destructor
	PhaseSpaceOutgoing(std::string pso_string, std::string scanner_mode);
	~PhaseSpaceOutgoing();

	// Files' strings	
	std::string file_name_extension = "ps_actor_phantom_outgoing.root";	
	std::string file_name;
	std::string scanner_name;

	// Counters
	Int_t annihilations;
	Int_t positron_no;
	Int_t gammas_no;
	Int_t annihil_gammas_no;
	Int_t annihil_gammas_middle_no;
	Int_t entries;

	// "Flag" variables
	Int_t a_runID;
	Int_t a_eventID;
	Int_t a_trackID;
	Int_t a_parentID;
	Int_t a_X;
	Int_t a_Y;
	Int_t a_Z;
	Int_t a_time;

	// Annihilation variables
	Int_t temp_event;
	bool at_rest = true;
	float e_low = 0.5109;
	float e = 511;
	float e_high = 0.5111;
	int e1 = 0;
	int e2 = 0;
	int rest1 = 0;
	int rest2 = 0;

	// Leaves initialization - TTree PhaseSpace	
	Int_t		AtomicNumber;
	Float_t		Ekine;
	Float_t		Weight;
	Float_t		Time;
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
	std::string annihil_process = "annihil";
	std::string RadioactiveDecay = "RadioactiveDecay";
	std::string Decay = "RadioactiveDecay";
	std::string ePairProd = "ePairProd";
	std::string protonInelastic = "protonInelastic";

	// Particles' strings
	std::string electron = "e-";
	std::string positron = "e+";
	std::string gamma = "gamma";
	std::string neutron = "neutron";
	std::string proton = "proton";

	// Additional variables
	bool flag_annihil = false;
	bool flag_prod = false;
	bool flag_gamma1 = false;
	int trackID_gamma1 = 0;
	int verify = 0;

	// Vectors
	std::vector<int> annihil_runID;
	std::vector<int> annihil_eventID;
	std::vector<int> annihil_trackID;
	std::vector<int> annihil_parentID;
	std::vector<float> annihil_X;
	std::vector<float> annihil_Y;
	std::vector<float> annihil_Z;
	std::vector<float> annihil_time;
	std::vector<Gamma> annihil_gammas;
	std::vector<std::string> radioactiveNuclei;

	// ROOT variables
	TTree* PhaseSpaceActor;
	TFile* f;

	// Basic methods
	void init();
	void closeRootFile();

	// Analysis methods
	void analysisPromptGammas();
	void analysisVHEEGammas();
	void complexAnalysisVHEEGammas();
	void gammasTimeAnalysisVHEE();
	void outgoing_spectra_energy_analysis();

	// Getters



};
#endif // PHASESPACEOUTGOING_HH









