#ifndef PLOT_HH
#define PLOT_HH
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
#include <TGraph2D.h>
#include <TGraph2DErrors.h>
#include <TMath.h>
#include <TH1D.h>
#include <TH2D.h>
#include <TFile.h>
#include <TProfile.h>
#include <TF1.h>
#include <TCanvas.h>
#include <TBrowser.h>
#include <TDirectory.h>
#include <TGraphErrors.h>
#include <TGraph.h>
#include <TMultiGraph.h>
#include <TGaxis.h>
#include <TAxis.h>
#include <TRandom.h>
#include <TLegend.h>
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


class Plot
{

public:

	// Constructor and destructor
	Plot(std::string scanner_mode_string);
	~Plot();

	// Files' strings
	std::string dir_desktop = "/home/baran/Desktop/";
	std::string ieee_file = "root_IEEE_MIC_";
	std::string ieee_file_integrated = "root_IEEE_MIC_integrated_";
	std::string ieee_file_interspill = "root_IEEE_MIC_interspill_";
	std::string complex_analysis_file = "root_complex_analysis_";
	std::string root_ext = ".root";
	std::string scanner_mode;

	// Counters
//	Int_t lines_counter;

	// Processess' strings
//	std::string DimSize = "DimSize";

	// Vectors
//	std::vector<int> dim_size;
//	std::vector<float> voxels_stream;



	// Basic methods
	void plot_IEEE_interspill();
	void plot_IEEE_integrated();
	void plot_complex_profile_analysis();

};
#endif // PLOT_HH









