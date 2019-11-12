#include "Dose.hh"


Dose::Dose(std::string dose_string, std::string scanner_mode, bool dose_flag)
{
	/*
		Constructor
	*/

	dir_path = dose_string;
	scanner_name = scanner_mode;
	if (!dose_flag)
		dose = "Edep";
}

Dose::~Dose()
{
	/*
		Destructor
	*/
	dose = true;
}



void Dose::readRAW ()
{
	/*
	Read the dose from the text file and save the dose z profile to the root file
	*/

	// Read voxels stream
	std::string f_name = dir_path + file_name_extension + dose + raw_ext;
	std::ifstream file (f_name.c_str(), std::ios::in|std::ios::binary);
	if (!file.is_open())
    		std::cerr<<"error while opening file"<<std::endl;
    
	// Set the input file
	std::ofstream txt_output;
	std::string txt_file = "/home/baran/Desktop/root_IEEE_MIC_"+scanner_name+"_dose.txt";
	txt_output.open (txt_file);

	// Create the 1D histogram
	TH1F *z_dose = new TH1F( "Dose profile", "Dose profile", (atoi(std::getenv("PRM_PH_BINS"))), (atof(std::getenv("PRM_PH_MIN"))), (atof(std::getenv("PRM_PH_MAX"))));
	z_dose->GetXaxis()->SetTitle("Z [mm]");
	z_dose->GetYaxis()->SetTitle("Dose [Gy]");

	float f;
	float sum_slice = 0.;
	int sum_voxels = 4000000;	
	int slice_number = 26;
	//loop over all voxels
	for (int i=1; i<=sum_voxels; i++)
	{
		file.read((char*)&f, sizeof(float));
		sum_slice += f;		
		if (i%20000 == 0)
		{
			z_dose->SetBinContent(slice_number,sum_slice);
			txt_output << sum_slice << std::endl;
			slice_number++;
			sum_slice = 0.;
		}

	}
	
	file.close();
	// Create, open and save new root file
	std::string root_f_name = "/home/baran/Desktop/root_IEEE_MIC_"+scanner_name+".root";
	TFile fw(root_f_name.c_str(),"UPDATE");

	gStyle->SetPalette();

	TCanvas* plot = new TCanvas("Dose","Dose",2000,1400);
	plot->Divide(1,1);
	plot->cd(1); z_dose->Draw();z_dose->Write();
	plot->Update();

	TImage *img = TImage::Create();
	img->FromPad(plot);
	root_f_name = "/home/baran/Desktop/root_IEEE_MIC_"+scanner_name+"_dose.png";
	img->WriteImage(root_f_name.c_str());

	fw.Close();
	txt_output.close();



}


