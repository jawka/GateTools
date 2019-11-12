#ifndef DOSE_HH
#define DOSE_HH
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
#include <iostream>
#include <fstream>
// Library


class Dose
{

public:

	// Constructor and destructor
	Dose(std::string dose_string, std::string scanner_mode, bool dose_flag);
	~Dose();

	// Files' strings
	std::string file_name_extension = "dose_phantom-";
	std::string dir_path;
	std::string scanner_name;
	std::string dose = "Dose";
	std::string raw_ext = ".raw";
	std::string mhd_ext = ".mhd";
	
	
	// Counters
	Int_t lines_counter;

	
	// Vectors
	std::vector<int> dim_size;
	std::vector<float> voxels_stream;
	

	// Methods
	void readRAW();


};
#endif // DOSE_HH









