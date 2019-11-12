#include "PhaseSpace.hh"


PhaseSpace::PhaseSpace(std::string ps_string, std::string scanner_mode, int analysis_mode, bool multiple_flag)
{
	/*
	Constructor
	*/
	temp_event = -1;
	annihilations = 0;
	positron_no = 0;	
	gammas_no = 0;
	positrons_conv = 0;
	positrons_RadioactiveDeacy = 0;
	positrons_Decay = 0;
	positrons_ePairProd = 0;
	dir_path = ps_string;
	file_name = ps_string + file_name_extension;
	scanner_name = scanner_mode;
	mode = analysis_mode;
	std::cout << multiple_flag << std::endl;
	if (analysis_mode == 13)
	{
		file_name_extension = "ps_actor_incoming_tpx.root";
		file_name_extension_split = "ps_actor_incoming_tpx";
	}
	if (multiple_flag)
		multiple_files_flag = true;
	// init method is called depending from the mode you are using 
	// for some of the simulations phaseSpace file 
	// is not produced so there is no need to initialize anything
	else if (analysis_mode != 10 && analysis_mode != 12)
		init(0);
}



PhaseSpace::~PhaseSpace()
{
	/*
	Destructor
	*/
	temp_event = -1;
	annihilations = 0;
	positron_no = 0;
	gammas_no = 0;
}


//Getter for pointer to the nuclei vector
std::vector<int>* PhaseSpace::getNuclei()
{
	return &annihil_nuclei;
}

//Getter for pointer to the positrons vector
std::vector<Positron>* PhaseSpace::getPositrons()
{
	return &positrons;
}

//Getter for pointer to the protons vector
std::vector<Proton>* PhaseSpace::getProtons()
{
	return &protons;
}


int PhaseSpace::gammasVerification (std::vector<Gamma> gammas)
{

	/*
	Method to verify if two gammas are from the same annihilation and have quantified energy(511 keV)
	In Geant4 there are two options for annihilation:
	- atRest - standard one with two gammas antiparallel
	- inFly - interaction of the positron with electron occured when positron has 
		non-zero kinetic energy which gives photons with energies and directions
		calculated based on momentum and energy equlibrium 
	*/	
	
	e1 = gammas[0].e_kin;
	e2 = gammas[1].e_kin;
	flag_gamma1 = false;

	// Add anihilations to the vectors !!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	if (at_rest)
	{
		if (e1 < e_high and e2 < e_high and e1 > e_low and e2 > e_low)
		{
			positrons.push_back(Positron(this->a_runID, gammas[0].event_id, gammas[0].parent_id, this->a_parentID, gammas[0].time, gammas[0].x, gammas[0].y, gammas[0].z, trackID_gamma1, trackID_gamma2));
			this->annihilations++;
			return 1;
		}
		else
		{
//			positrons.push_back(Positron(this->a_runID, this->a_eventID, this->a_trackID, this->a_parentID, this->a_time, this->a_X, this->a_Y, this->a_Z));			
//			this->annihilations++;
			return 0;	
		}

	}	
	else
	{
		if (e1 > e_high or e2 > e_high or e1 < e_low or e2 < e_low)
		{
			positrons.push_back(Positron(this->a_runID, this->a_eventID, this->a_trackID, this->a_parentID, gammas[0].time, gammas[0].x, gammas[0].y, gammas[0].z, trackID_gamma1, trackID_gamma2));			
			this->annihilations++;
			return 1;
		}
		else
		{
			return 0;
		}
	}
}

// init method
void PhaseSpace::init(int files_no)
{

	int files_number = files_no;
	file_name =  dir_path + file_name_extension;

	if (files_number > 0)
	{
		file_name = dir_path + file_name_extension_split + "_" + files_number + root_ext;
	}
	
	f = new TFile(file_name.c_str(), "READ");

	if( f->IsZombie() )
	{
		std::cerr << "Cannot open the file '" << file_name  << "'" << std::endl;
		exit( EXIT_FAILURE );
	}
	
	PhaseSpaceActor = (TTree*)gDirectory->Get("PhaseSpace");

	// SET BRANCH ADDRESSES - TTREE PHASESPACE

	PhaseSpaceActor->SetBranchAddress("AtomicNumber",&AtomicNumber);
	PhaseSpaceActor->SetBranchAddress("Ekine",&Ekine);
	PhaseSpaceActor->SetBranchAddress("Time",&Time);
	PhaseSpaceActor->SetBranchAddress("X",&X);
	PhaseSpaceActor->SetBranchAddress("Y",&Y);
	PhaseSpaceActor->SetBranchAddress("Z",&Z);
	PhaseSpaceActor->SetBranchAddress("CreatorProcess",&CreatorProcess);
	PhaseSpaceActor->SetBranchAddress("ProcessDefinedStep",&ProcessDefinedStep);
	PhaseSpaceActor->SetBranchAddress("TrackID",&TrackID);
	PhaseSpaceActor->SetBranchAddress("ParentID",&ParentID);
	PhaseSpaceActor->SetBranchAddress("EventID",&EventID);
	PhaseSpaceActor->SetBranchAddress("RunID",&RunID);
	PhaseSpaceActor->SetBranchAddress("PDGCode",&PDGCode);
	PhaseSpaceActor->SetBranchAddress("ParticleName",&ParticleName);
	PhaseSpaceActor->SetBranchAddress("EmissionPointX",&EmissionPointX);
	PhaseSpaceActor->SetBranchAddress("EmissionPointY",&EmissionPointY);
	PhaseSpaceActor->SetBranchAddress("EmissionPointZ",&EmissionPointZ);
	PhaseSpaceActor->SetBranchAddress("TOut",&TOut);
	PhaseSpaceActor->SetBranchAddress("TProd",&TProd);
	PhaseSpaceActor->SetBranchAddress("Weight",&Weight);
	PhaseSpaceActor->SetBranchAddress("Mass",&Mass);
	PhaseSpaceActor->SetBranchAddress("dX",&dX);
	PhaseSpaceActor->SetBranchAddress("dY",&dY);
	PhaseSpaceActor->SetBranchAddress("dZ",&dZ);
	PhaseSpaceActor->SetBranchAddress("ProductionVolume",&ProductionVolume);

	// ENTRIES NUMBER
 
	entries = PhaseSpaceActor->GetEntries();
	std::cout << "Number of entries in " << file_name_extension << " file: " << entries << std::endl;
	
	// RADIOACTIVE NUCLEI FOR PROTON RADIOTHERAPY

	radioactiveNuclei.push_back("B8");			//0
	radioactiveNuclei.push_back("C9");			//1
	radioactiveNuclei.push_back("C10");			//2
	radioactiveNuclei.push_back("C11");			//3
	radioactiveNuclei.push_back("N12");			//4
	radioactiveNuclei.push_back("N13");			//5
	radioactiveNuclei.push_back("O13");			//6
	radioactiveNuclei.push_back("O14");			//7
	radioactiveNuclei.push_back("O15");			//8
	radioactiveNuclei.push_back("F18");			//9
	radioactiveNuclei.push_back("P29");			//10
	radioactiveNuclei.push_back("P30");			//11
	radioactiveNuclei.push_back("S30");			//12
	radioactiveNuclei.push_back("S31");			//13
	radioactiveNuclei.push_back("K37");			//14


}



// ANALYSIS OF THE ANNIHILATION GAMMAS
void PhaseSpace::analysisAnnihilationGammas()
{

	// HISTOGRAMS INITIALIZATION

	TH2F *xy = new TH2F( "xy", "XY histogram", 100, -50.0, 50.0, 100, -50.0, 50.0);
	xy->GetXaxis()->SetTitle("X [mm]");
	xy->GetYaxis()->SetTitle("Y [mm]");

	TH1F *z = new TH1F( "z", "Z position of all poositrons", atoi(std::getenv("PRM_PH_BINS")), atof(std::getenv("PRM_PH_MIN")), atof(std::getenv("PRM_PH_MAX")));
	z->GetXaxis()->SetTitle("Z [mm]");
	z->GetYaxis()->SetTitle("Number of particles");

	TH1F *annihil_time_curve = new TH1F( "annihil_time", "Annihilation time", 1000, 0, 1);
	annihil_time_curve->GetXaxis()->SetTitle("Time [s] - 1 ms intervals");
	annihil_time_curve->GetYaxis()->SetTitle("Number of annihilations");

	TH1F *gamma_energy = new TH1F( "Gamma energy", "Gamma energy", 5000, 0, 10);
	gamma_energy->GetXaxis()->SetTitle("Energy [MeV]");
	gamma_energy->GetYaxis()->SetTitle("Number of gammas");

	//positron_no = 0;

	//
	// MAIN ANALYSIS (LOOP)
	//			
	
	Int_t i(0);
	Int_t nbytes(0);
	// Analysis have several steps
	while(i < entries)
	{
		if (i%100000000 == 0)
			std::cout << i/100000000 << " E+08 events analysed." << std::endl;

		nbytes += PhaseSpaceActor->GetEntry( i );
		
		if (temp_event != EventID)
		{		
			temp_event = EventID;
			flag_annihil = false;
		}

		//POSITRONS
		// Finding the positron and storing the positron properties as trackID
		if (positron.compare(ParticleName) == 0)	
		{
//			if (conv.compare(CreatorProcess) !=0 and ePairProd.compare(CreatorProcess) !=0 and RadioactiveDecay.compare(CreatorProcess) !=0)			
//				std::cout << CreatorProcess << std::endl;
			
			if (!flag_annihil and prod_process.compare(CreatorProcess) == 0)
			{
				a_runID = RunID;
				a_eventID = EventID;
				a_trackID = TrackID;
				a_parentID = ParentID;
				a_X = EmissionPointX;
				a_Y = EmissionPointY;
				a_Z = EmissionPointZ;
				a_time = TProd;
				positron_no++;
				flag_annihil = true;
			}

		}

		// GAMMAS
//		else if (PDGCode == 22)
		// Finding the gammas and chacking if they are produced by the dtored positron
		else if (gamma.compare(ParticleName) == 0)				
		{
			
			// FIRST GAMMA
			// first gamma is not analyzed, the second one have to be found
			if (flag_annihil and !flag_gamma1 and annihil_process.compare(CreatorProcess) == 0 and a_trackID == ParentID)
			{
				
				annihil_gammas.push_back(Gamma(RunID, EventID, TrackID, ParentID, Time, Ekine, EmissionPointX, EmissionPointY, EmissionPointZ));
				trackID_gamma1 = TrackID;
				this->flag_gamma1 = true;
			}

			// SECOND GAMMA
			// if the second gamma is found the main analysis is performed (gammasVerification)
			if (flag_annihil and flag_gamma1 and annihil_process.compare(CreatorProcess) == 0 and a_trackID == ParentID and TrackID != trackID_gamma1)
			{
				annihil_gammas.push_back(Gamma(RunID, EventID, TrackID, ParentID, Time, Ekine, EmissionPointX, EmissionPointY, EmissionPointZ));
				trackID_gamma2 = TrackID;

				//Gammas verification				
				verify = gammasVerification(annihil_gammas);
				if (verify == 1)
				{
					xy->Fill(a_X, a_Y);
					z->Fill(a_Z);
					annihil_time_curve->Fill(a_time/1000000000);
					gamma_energy->Fill(annihil_gammas[0].e_kin);
					gamma_energy->Fill(annihil_gammas[1].e_kin);
				}
				this->flag_annihil = false;
				annihil_gammas.clear();				
			}
		}
		i++;
	}

	std::string f_name = "/home/baran/Desktop/root_phaseSpace_"+scanner_name+"_annihilationGammas_RadioactiveDecay_atRest.root";
	TFile fw(f_name.c_str(),"RECREATE");

	gStyle->SetPalette();

	TCanvas* plot = new TCanvas("PhaseSpaceActor","PhaseSpaceActor",2000,1400);
	plot->Divide(2,2);
	plot->cd(1); xy->Draw("COLZ1");xy->Write();
	plot->cd(2); z->Draw();z->Write();
	plot->cd(3); annihil_time_curve->Draw();annihil_time_curve->Write();
	plot->cd(4); gamma_energy->Draw();gamma_energy->Write();
	plot->Update();

	TImage *img = TImage::Create();
	img->FromPad(plot);
	f_name = "/home/baran/Desktop/root_phaseSpace_"+scanner_name+"_annihilationGammas_RadioactiveDecay_atRest.png";
	img->WriteImage(f_name.c_str());
	
	fw.Print();
	fw.Close();
	// mode 9 is for the 2018 IEEE MIC conference (Sydney, Melbourne) conference 
	if (mode == 9)
	{
		f_name = "/home/baran/Desktop/root_IEEE_MIC_"+scanner_name+".root";
		TFile fw2(f_name.c_str(),"UPDATE");
		gStyle->SetPalette();
		TCanvas* plot2 = new TCanvas("PhaseSpaceActor","PhaseSpaceActor",2000,1400);
		plot2->Divide(1,1);
		plot2->cd(1); z->Draw();z->Write();
		plot2->Update();
	
		TImage *img2 = TImage::Create();
		img2->FromPad(plot2);
		f_name = "/home/baran/Desktop/root_IEEE_MIC_"+scanner_name+"_positrons_EmissionZ.png";
		img2->WriteImage(f_name.c_str());
	
		fw2.Close();
	}	
	std::cout << "Number of positrons: " << positron_no << std::endl;
	std::cout << "Size of the positrons vectors: " << positrons.size() << std::endl;
}








// ANALYSIS OF THE PROMPT GAMMAS
void PhaseSpace::analysisPromptGammas()
{


	//Histogram initialization
	TH1F *prod_time_curve = new TH1F( "Production time of prompt gammas", "Production time of prompt gammas", 1200, 0, 12);
	prod_time_curve->GetXaxis()->SetTitle("Time [ns] - 0.01 nanoseconds intervals");
	prod_time_curve->GetYaxis()->SetTitle("Number of prompt gammas");

	TH1F *gamma_energy_hist = new TH1F( "Gamma energy", "Gamma energy", 5000, 0, 10);
	gamma_energy_hist->GetXaxis()->SetTitle("Energy [MeV]");
	gamma_energy_hist->GetYaxis()->SetTitle("Number of gammas");	

	Int_t j(0);
	Int_t bytes(0);

	// MAIN ANALYSIS (LOOP)
	while(j < entries)
	{
		if (j%10000000 == 0)
			std::cout << j/10000000 << " E+07 events analysed." << std::endl;
		bytes += PhaseSpaceActor->GetEntry( j );

		//GAMMAS
//		if (PDGCode == 22)	
		if (gamma.compare(ParticleName) == 0)
		{			
			if (Ekine < 10. and Ekine > 0. and protonInelastic.compare(CreatorProcess) != 0)
			{
				gammas_no++;
				prod_time_curve->Fill(Time);
				gamma_energy_hist->Fill(Ekine);
			}
		}
		j++;
	}

	std::string f_name = "/home/baran/Desktop/root_phaseSpace_"+scanner_name+"_promptGammas.root";
	TFile fw(f_name.c_str(),"RECREATE");

	std::cout << "All produced gammas no: " << gammas_no << std::endl;

	gStyle->SetPalette();

	TCanvas* plot2 = new TCanvas("PhaseSpaceActor","PhaseSpaceActor",2000,1400);
	plot2->Divide(1,2);
	plot2->cd(1); prod_time_curve->Draw(); prod_time_curve->Write();
	plot2->cd(2); gamma_energy_hist->Draw(); gamma_energy_hist->Write();
	gPad->SetLogy();
	plot2->Update();

	TImage *img2 = TImage::Create();
	img2->FromPad(plot2);
	f_name = "/home/baran/Desktop/root_phaseSpace_"+scanner_name+"_promptGammas.png";
	img2->WriteImage(f_name.c_str());
	
	fw.Print();
	fw.Close();

}










void PhaseSpace::analysisGenericIonsAndAnnihilation()
{

	/*

	The method creates profiles of the positrons with the distinguishment between different nuclei.  
	Additionally the time histogram of the produced positrons is created.
	*/

	// HISTOGRAMS INITIALIZATION

	positron_no = 0;

	TH1F *tB8 = new TH1F( "B8", "B8", 1000, 0, 5);
	tB8->GetXaxis()->SetTitle("Production time [s] - 5 ms intervals");
	tB8->GetYaxis()->SetTitle("Number of produced positrons (e+)");
	TH1F *zB8 = new TH1F( "B8", "B8", atoi(std::getenv("PRM_PH_BINS")), atof(std::getenv("PRM_PH_MIN")), atof(std::getenv("PRM_PH_MAX")));
	zB8->GetXaxis()->SetTitle("Z [mm]");
	zB8->GetYaxis()->SetTitle("Number of produced positrons (e+)");

	TH1F *tC9 = new TH1F( "C9", "C9", 1000, 0, 1);
	tC9->GetXaxis()->SetTitle("Production time [s] - 1 ms intervals");
	tC9->GetYaxis()->SetTitle("Number of produced positrons (e+)");
	TH1F *zC9 = new TH1F( "C9", "C9", atoi(std::getenv("PRM_PH_BINS")), atof(std::getenv("PRM_PH_MIN")), atof(std::getenv("PRM_PH_MAX")));
	zC9->GetXaxis()->SetTitle("Z [mm]");
	zC9->GetYaxis()->SetTitle("Number of produced positrons (e+)");

	TH1F *tC10 = new TH1F( "C10", "C10", 1000, 0, 150);
	tC10->GetXaxis()->SetTitle("Production time [s] - 0.15 s intervals");
	tC10->GetYaxis()->SetTitle("Number of produced positrons (e+)");
	TH1F *zC10 = new TH1F( "C10", "C10", atoi(std::getenv("PRM_PH_BINS")), atof(std::getenv("PRM_PH_MIN")), atof(std::getenv("PRM_PH_MAX")));
	zC10->GetXaxis()->SetTitle("Z [mm]");
	zC10->GetYaxis()->SetTitle("Number of produced positrons (e+)");

	TH1F *tC11 = new TH1F( "C11", "C11", 1000, 0, 200);
	tC11->GetXaxis()->SetTitle("Production time [min] - 0.2 min intervals");
	tC11->GetYaxis()->SetTitle("Number of produced positrons (e+)");
	TH1F *zC11 = new TH1F( "C11", "C11", atoi(std::getenv("PRM_PH_BINS")), atof(std::getenv("PRM_PH_MIN")), atof(std::getenv("PRM_PH_MAX")));
	zC11->GetXaxis()->SetTitle("Z [mm]");
	zC11->GetYaxis()->SetTitle("Number of produced positrons (e+)");

	TH1F *tN12 = new TH1F( "N12", "N12", 1000, 0, 100);
	tN12->GetXaxis()->SetTitle("Production time [ms] - 0.1 ms intervals");
	tN12->GetYaxis()->SetTitle("Number of produced positrons (e+)");
	TH1F *zN12 = new TH1F( "N12", "N12", atoi(std::getenv("PRM_PH_BINS")), atof(std::getenv("PRM_PH_MIN")), atof(std::getenv("PRM_PH_MAX")));
	zN12->GetXaxis()->SetTitle("Z [mm]");
	zN12->GetYaxis()->SetTitle("Number of produced positrons (e+)");

	TH1F *tN13 = new TH1F( "N13", "N13", 1000, 0, 100);
	tN13->GetXaxis()->SetTitle("Production time [min] - 0.1 min intervals");
	tN13->GetYaxis()->SetTitle("Number of produced positrons (e+)");
	TH1F *zN13 = new TH1F( "N13", "N13", atoi(std::getenv("PRM_PH_BINS")), atof(std::getenv("PRM_PH_MIN")), atof(std::getenv("PRM_PH_MAX")));
	zN13->GetXaxis()->SetTitle("Z [mm]");
	zN13->GetYaxis()->SetTitle("Number of produced positrons (e+)");

	TH1F *tO13 = new TH1F( "O13", "O13", 1000, 0, 80);
	tO13->GetXaxis()->SetTitle("Production time [ms] - 0.08 ms intervals");
	tO13->GetYaxis()->SetTitle("Number of produced positrons (e+)");
	TH1F *zO13 = new TH1F( "O13", "O13", atoi(std::getenv("PRM_PH_BINS")), atof(std::getenv("PRM_PH_MIN")), atof(std::getenv("PRM_PH_MAX")));
	zO13->GetXaxis()->SetTitle("Z [mm]");
	zO13->GetYaxis()->SetTitle("Number of produced positrons (e+)");

	TH1F *tO14 = new TH1F( "O14", "O14", 1000, 0, 10);
	tO14->GetXaxis()->SetTitle("Production time [min] - 0.01 min intervals");
	tO14->GetYaxis()->SetTitle("Number of produced positrons (e+)");
	TH1F *zO14 = new TH1F( "O14", "O14", atoi(std::getenv("PRM_PH_BINS")), atof(std::getenv("PRM_PH_MIN")), atof(std::getenv("PRM_PH_MAX")));
	zO14->GetXaxis()->SetTitle("Z [mm]");
	zO14->GetYaxis()->SetTitle("Number of produced positrons (e+)");

	TH1F *tO15 = new TH1F( "O15", "O15", 1000, 0, 20);
	tO15->GetXaxis()->SetTitle("Production time [min] - 0.02 min intervals");
	tO15->GetYaxis()->SetTitle("Number of produced positrons (e+)");
	TH1F *zO15 = new TH1F( "O15", "O15", atoi(std::getenv("PRM_PH_BINS")), atof(std::getenv("PRM_PH_MIN")), atof(std::getenv("PRM_PH_MAX")));
	zO15->GetXaxis()->SetTitle("Z [mm]");
	zO15->GetYaxis()->SetTitle("Number of produced positrons (e+)");

	TH1F *tF18 = new TH1F( "F18", "F18", 1000, 0, 700);
	tF18->GetXaxis()->SetTitle("Production time [min] - 0.7 min intervals");
	tF18->GetYaxis()->SetTitle("Number of produced positrons (e+)");
	TH1F *zF18 = new TH1F( "F18", "F18", atoi(std::getenv("PRM_PH_BINS")), atof(std::getenv("PRM_PH_MIN")), atof(std::getenv("PRM_PH_MAX")));
	zF18->GetXaxis()->SetTitle("Z [mm]");
	zF18->GetYaxis()->SetTitle("Number of produced positrons (e+)");

	TH1F *tP29 = new TH1F( "P29", "P29", 1000, 0, 30);
	tP29->GetXaxis()->SetTitle("Production time [s] - 30 ms intervals");
	tP29->GetYaxis()->SetTitle("Number of produced positrons (e+)");
	TH1F *zP29 = new TH1F( "P29", "P29", atoi(std::getenv("PRM_PH_BINS")), atof(std::getenv("PRM_PH_MIN")), atof(std::getenv("PRM_PH_MAX")));
	zP29->GetXaxis()->SetTitle("Z [mm]");
	zP29->GetYaxis()->SetTitle("Number of produced positrons (e+)");

	TH1F *tP30 = new TH1F( "P30", "P30", 1000, 0, 15);
	tP30->GetXaxis()->SetTitle("Production time [min] - 0.015 min intervals");
	tP30->GetYaxis()->SetTitle("Number of produced positrons (e+)");
	TH1F *zP30 = new TH1F( "P30", "P30", atoi(std::getenv("PRM_PH_BINS")), atof(std::getenv("PRM_PH_MIN")), atof(std::getenv("PRM_PH_MAX")));
	zP30->GetXaxis()->SetTitle("Z [mm]");
	zP30->GetYaxis()->SetTitle("Number of produced positrons (e+)");

	TH1F *tS30 = new TH1F( "S30", "S30", 1000, 0, 8);
	tS30->GetXaxis()->SetTitle("Production time [s] - 8 ms intervals");
	tS30->GetYaxis()->SetTitle("Number of produced positrons (e+)");
	TH1F *zS30 = new TH1F( "S30", "S30", atoi(std::getenv("PRM_PH_BINS")), atof(std::getenv("PRM_PH_MIN")), atof(std::getenv("PRM_PH_MAX")));
	zS30->GetXaxis()->SetTitle("Z [mm]");
	zS30->GetYaxis()->SetTitle("Number of produced positrons (e+)");

	TH1F *tS31 = new TH1F( "S31", "S31", 1000, 0, 20);
	tS31->GetXaxis()->SetTitle("Production time [s] - 20 ms intervals");
	tS31->GetYaxis()->SetTitle("Number of produced positrons (e+)");
	TH1F *zS31 = new TH1F( "S31", "S31", atoi(std::getenv("PRM_PH_BINS")), atof(std::getenv("PRM_PH_MIN")), atof(std::getenv("PRM_PH_MAX")));
	zS31->GetXaxis()->SetTitle("Z [mm]");
	zS31->GetYaxis()->SetTitle("Number of produced positrons (e+)");

	TH1F *tK37 = new TH1F( "K37", "K37", 1000, 0, 10);
	tK37->GetXaxis()->SetTitle("Production time [s] - 10 ms intervals");
	tK37->GetYaxis()->SetTitle("Number of produced positrons (e+)");
	TH1F *zK37 = new TH1F( "K37", "K37", atoi(std::getenv("PRM_PH_BINS")), atof(std::getenv("PRM_PH_MIN")), atof(std::getenv("PRM_PH_MAX")));
	zK37->GetXaxis()->SetTitle("Z [mm]");
	zK37->GetYaxis()->SetTitle("Number of produced positrons (e+)");

	TH1F *tAll = new TH1F( "All", "All", 1000, 0, 1);
	tAll->GetXaxis()->SetTitle("Production time [min] - 0.001 min intervals");
	tAll->GetYaxis()->SetTitle("Number of produced positrons (e+)");
	TH1F *zAll = new TH1F( "All", "All", atoi(std::getenv("PRM_PH_BINS")), atof(std::getenv("PRM_PH_MIN")), atof(std::getenv("PRM_PH_MAX")));
	zAll->GetXaxis()->SetTitle("Z [mm]");
	zAll->GetYaxis()->SetTitle("Number of produced positrons (e+)");


	// MAIN ANALYSIS (LOOP)
	
	Int_t i(0);
	Int_t nbytes(0);

	while(i < entries)
	{
		if (i%10000000 == 0)
			std::cout << i/10000000 << " E+07 events analysed." << std::endl;

		nbytes += PhaseSpaceActor->GetEntry( i );
		
		if (temp_event != EventID)
		{		
			temp_event = EventID;
			flag_annihil = false;
			nuclei_prod = false;
		}

		if (std::find(radioactiveNuclei.begin(), radioactiveNuclei.end(), std::string(ParticleName)) != radioactiveNuclei.end() )
		{
			for (int ii = 0; ii < radioactiveNuclei.size(); ii++)
			{	
				if (radioactiveNuclei[ii].compare(ParticleName) == 0)
				{
					a_nuclei = ii;
					nuclei_event = EventID;
					nuclei_track = TrackID;
					nuclei_prod = true;
					break;
				}
			}
		}

		//POSITRONS
		if (positron.compare(ParticleName) == 0)	
		{			
			
			if (!flag_annihil and nuclei_prod and RadioactiveDecay.compare(CreatorProcess) == 0)
			{	
				positron_no++;

				nuclei_name = std::string(ParticleName);
				flag_annihil = true;

				tAll->Fill(Time/1000000000); zAll->Fill(EmissionPointZ);
			
				switch (a_nuclei)
				{				
					case 0:
						tB8->Fill(Time/1000000000); zB8->Fill(EmissionPointZ);
						break;
					case 1:
						tC9->Fill(Time/1000000000); zC9->Fill(EmissionPointZ);
						break;
					case 2:
						tC10->Fill(Time/1000000000); zC10->Fill(EmissionPointZ);
						break;
					case 3:
						tC11->Fill(Time/60000000000); zC11->Fill(EmissionPointZ);
						break;
					case 4:
						tN12->Fill(Time/1000000); zN12->Fill(EmissionPointZ);
						break;
					case 5:
						tN13->Fill(Time/60000000000); zN13->Fill(EmissionPointZ);
						break;
					case 6:
						tO13->Fill(Time/1000000); zO13->Fill(EmissionPointZ);
						break;
					case 7:
						tO14->Fill(Time/60000000000); zO14->Fill(EmissionPointZ);
						break;
					case 8:
						tO15->Fill(Time/60000000000); zO15->Fill(EmissionPointZ);
						break;
					case 9:
						tF18->Fill(Time/60000000000); zF18->Fill(EmissionPointZ);
						break;
					case 10:
						tP29->Fill(Time/1000000000); zP29->Fill(EmissionPointZ);
						break;
					case 11:
						tP30->Fill(Time/60000000000); zP30->Fill(EmissionPointZ);
						break;
					case 12:
						tS30->Fill(Time/1000000000); zS30->Fill(EmissionPointZ);
						break;
					case 13:
						tS31->Fill(Time/1000000000); zS31->Fill(EmissionPointZ);
						break;
					case 14:
						tK37->Fill(Time/1000000000); zK37->Fill(EmissionPointZ);
						break;
					default:
						std::cout << "Not taken into account radioactive nuclei:" << nuclei_name << std::endl;
						break;
				}
			}
		}
		i++;
	}

	std::cout << "Positrons number: " << positron_no << std::endl;

	std::string f_name = "/home/baran/Desktop/root_phaseSpace_"+scanner_name+"_positrons.root";
	TFile fw(f_name.c_str(),"RECREATE");

	gStyle->SetPalette();

	// First plot - time
	TCanvas* plot1 = new TCanvas("PhaseSpaceActor1","PhaseSpaceActor1",2000,1400);
	plot1->Divide(4,4);
	plot1->cd(1); tB8->Draw(); tB8->Write();
	plot1->cd(2); tC9->Draw(); tC9->Write();
	plot1->cd(3); tC10->Draw(); tC10->Write();
	plot1->cd(4); tC11->Draw(); tC11->Write();
	plot1->cd(5); tN12->Draw(); tN12->Write();
	plot1->cd(6); tN13->Draw(); tN13->Write();
	plot1->cd(7); tO13->Draw(); tO13->Write();
	plot1->cd(8); tO14->Draw(); tO14->Write();
	plot1->cd(9); tO15->Draw(); tO15->Write();
	plot1->cd(10); tF18->Draw(); tF18->Write();
	plot1->cd(11); tP29->Draw(); tP29->Write();
	plot1->cd(12); tP30->Draw(); tP30->Write();
	plot1->cd(13); tS30->Draw(); tS30->Write();
	plot1->cd(14); tS31->Draw(); tS31->Write();
	plot1->cd(15); tK37->Draw(); tK37->Write();
	plot1->cd(15); tAll->Draw(); tAll->Write();
	plot1->Update();

	TImage *img1 = TImage::Create();
	img1->FromPad(plot1);
	f_name = "/home/baran/Desktop/root_phaseSpace_"+scanner_name+"_positrons_prodTime.png";
	img1->WriteImage(f_name.c_str());


	// Second plot - position Z
	TCanvas* plot2 = new TCanvas("PhaseSpaceActor2","PhaseSpaceActor2",2000,1400);
	plot2->Divide(4,4);
	plot2->cd(1); zB8->Draw(); zB8->Write();
	plot2->cd(2); zC9->Draw(); zC9->Write();
	plot2->cd(3); zC10->Draw(); zC10->Write();
	plot2->cd(4); zC11->Draw(); zC11->Write();
	plot2->cd(5); zN12->Draw(); zN12->Write();
	plot2->cd(6); zN13->Draw(); zN13->Write();
	plot2->cd(7); zO13->Draw(); zO13->Write();
	plot2->cd(8); zO14->Draw(); zO14->Write();
	plot2->cd(9); zO15->Draw(); zO15->Write();
	plot2->cd(10); zF18->Draw(); zF18->Write();
	plot2->cd(11); zP29->Draw(); zP29->Write();
	plot2->cd(12); zP30->Draw(); zP30->Write();
	plot2->cd(13); zS30->Draw(); zS30->Write();
	plot2->cd(14); zS31->Draw(); zS31->Write();
	plot2->cd(15); zK37->Draw(); zK37->Write();
	plot2->cd(16); zAll->Draw(); zAll->Write();
	plot2->Update();

	TImage *img2 = TImage::Create();
	img2->FromPad(plot2);
	f_name = "/home/baran/Desktop/root_phaseSpace_"+scanner_name+"_positrons_posZ.png";
	img2->WriteImage(f_name.c_str());

	fw.Print();
	fw.Close();

}










// ANALYSIS FOR THE Very High Eenergy Electrons (VHEE) - annihilation analysis
void PhaseSpace::analysisVHEEGammas()
{
	// HISTOGRAMS INITIALIZATION

	TH2F *xy = new TH2F( "xy", "XY histogram", 100, -50.0, 50.0, 100, -50.0, 50.0);
	xy->GetXaxis()->SetTitle("X [mm]");
	xy->GetYaxis()->SetTitle("Y [mm]");

	TH1F *z = new TH1F( "z", "Z range", atoi(std::getenv("PRM_PH_BINS")), atof(std::getenv("PRM_PH_MIN")), atof(std::getenv("PRM_PH_MAX")));
	z->GetXaxis()->SetTitle("Z [mm]");
	z->GetYaxis()->SetTitle("Number of particles");

	TH1F *prod_time_curve = new TH1F( "prod_time", "Positron annihilation time", 1200, 0, 12);
	prod_time_curve->GetXaxis()->SetTitle("Time [ns] - 0.01 nanoseconds intervals");
	prod_time_curve->GetYaxis()->SetTitle("Number of prompt gammas");

	TH1F *gamma_energy = new TH1F( "Gamma energy", "Gamma energy", 5000, 0, 10);
	gamma_energy->GetXaxis()->SetTitle("Energy [MeV]");
	gamma_energy->GetYaxis()->SetTitle("Number of gammas");

	TH2F *z_time = new TH2F( "Z position versus production time", "Z position versus production time", 1000, 0, 10, atoi(std::getenv("PRM_PH_BINS")), atof(std::getenv("PRM_PH_MIN")), atof(std::getenv("PRM_PH_MAX")));
	z_time->GetXaxis()->SetTitle("Production Time [ns]");
	z_time->GetYaxis()->SetTitle("Z position [mm]");

	TH1F *gamma_prod_time = new TH1F( "Gammas production time from the incident electron production", "Gammas production time from the incident electron production", 1000, 0, 100);
	gamma_prod_time->GetXaxis()->SetTitle("Time [ns]");
	gamma_prod_time->GetYaxis()->SetTitle("Number of gammas");

	TH1F *annihil_gamma_prod_time = new TH1F( "Annihilation gammas production time from the incident electron production", "Annihilation gammas production time from the incident electron production", 1000, 0, 100);
	annihil_gamma_prod_time->GetXaxis()->SetTitle("Time [ns]");
	annihil_gamma_prod_time->GetYaxis()->SetTitle("Number of annihilation gammas");

	// MAIN ANALYSIS (LOOP)
	
	Int_t i(0);
	Int_t nbytes(0);

	std::cout << "-------SETUP-------" << std::endl;
	std::cout << "Number of all positrons: " << positron_no << std::endl;
	std::cout << "Number of positrons from conversion: " << positrons_conv << std::endl;
	std::cout << "Number of positrons from RadioactiveDecay: " << positrons_RadioactiveDeacy << std::endl;
	std::cout << "Number of positrons from ePairProd: " << positrons_ePairProd << std::endl;
	std::cout << "Number of positrons from Decay: " << positrons_Decay << std::endl;
	std::cout << "--------END--------" << std::endl;

	while(i < entries)
	{
		if (i%10000000 == 0)
			std::cout << i/10000000 << " E+07 events analysed." << std::endl;
		
		nbytes += PhaseSpaceActor->GetEntry( i );
		
		if (temp_event != EventID)
		{		
			temp_event = EventID;
			flag_annihil = false;
		}

		//POSITRONS
		if (positron.compare(ParticleName) == 0)	
		{
			
			a_runID = RunID;
			a_eventID = EventID;
			a_trackID = TrackID;
			a_parentID = ParentID;
			a_X = EmissionPointX;
			a_Y = EmissionPointY;
			a_Z = EmissionPointZ;
			a_time = Time;
			positron_no++;
			if (conv.compare(CreatorProcess) == 0)
				positrons_conv++;
			else if (RadioactiveDecay.compare(CreatorProcess) == 0)
				positrons_RadioactiveDeacy++;
			else if (ePairProd.compare(CreatorProcess) == 0)
				positrons_ePairProd++;
			else if (Decay.compare(CreatorProcess) == 0)
				positrons_Decay++;
			else
				std::cout << CreatorProcess << std::endl;
			flag_annihil = true;
			xy->Fill(a_X, a_Y);
			z->Fill(a_Z);
			z_time->Fill(a_time, a_Z);
			prod_time_curve->Fill(a_time);			
		}

		// GAMMAS
//		else if (PDGCode == 22)
		else if (gamma.compare(ParticleName) == 0)				
		{			
			gamma_prod_time->Fill(TProd+TOut);
			if (annihil_process.compare(CreatorProcess) == 0)
			{
				annihil_gamma_prod_time->Fill(TProd+TOut);
			}
		}
		i++;
	}

	std::cout << "------RESULTS------" << std::endl;
	std::cout << "Number of all produced positrons: " << positron_no << " - " << (float) 100*(float)positron_no/(float)entries << "%" << " of all produced particles" << std::endl;
	std::cout << "Number of all produced positrons per primary electron: " << (float) (float)positron_no/10000000. << std::endl;
	std::cout << "Number of positrons produced by conversion: " << positrons_conv << " - " << (float) 100*(float)positrons_conv/(float)positron_no << "%" << " of all produced positrons" << std::endl;
	std::cout << "Number of positrons produced by RadioactiveDecay: " << positrons_RadioactiveDeacy << " - " << (float) 100*(float)positrons_RadioactiveDeacy/(float)positron_no << "%" << " of all produced positrons" << std::endl;
	std::cout << "Number of positrons produced by ePairProd: " << positrons_ePairProd << " - " << (float) 100*(float)positrons_ePairProd/(float)positron_no << "%" << " of all produced positrons" << std::endl;
	std::cout << "Number of positrons produced by Decay: " << positrons_Decay << " - " << (float) 100*(float)positrons_Decay/(float)positron_no << "%" << " of all produced positrons" << std::endl;
	std::cout << "--------END--------" << std::endl;

	// FIRST FILE FOR POSITRONS ANALYSIS

	std::string f_name = "/home/baran/Desktop/root_phaseSpace_"+scanner_name+"_VHEE_all_positrons.root";
	TFile fw(f_name.c_str(),"RECREATE");

	TCanvas* plot = new TCanvas("PhaseSpaceActor1","PhaseSpaceActor1",2000,1400);
	plot->Divide(1,2);
	plot->cd(1); xy->Draw("COLZ1");xy->Write();
	plot->cd(2); z->Draw();z->Write();
	plot->Update();

	TImage *img = TImage::Create();
	img->FromPad(plot);
	f_name = "/home/baran/Desktop/root_phaseSpace_"+scanner_name+"_VHEE_all_positrons.png";
	img->WriteImage(f_name.c_str());

	fw.Print();
	fw.Close();


	// SECOND FILE FOR GAMMAS ANALYSIS
	f_name = "/home/baran/Desktop/root_phaseSpace_"+scanner_name+"_VHEE_gammas_time_production.root";
	TFile fw2(f_name.c_str(),"RECREATE");

	TCanvas* plot2 = new TCanvas("PhaseSpaceActor2","PhaseSpaceActor2",2000,1400);
	plot2->Divide(1,2);
	plot2->cd(1); gamma_prod_time->Draw();gamma_prod_time->Write();
	plot2->cd(2); annihil_gamma_prod_time->Draw();annihil_gamma_prod_time->Write();
	plot2->Update();

	TImage *img2 = TImage::Create();
	img2->FromPad(plot2);
	f_name = "/home/baran/Desktop/root_phaseSpace_"+scanner_name+"_VHEE_gammas_time_production.png";
	img2->WriteImage(f_name.c_str());

	fw2.Print();
	fw2.Close();


}





void PhaseSpace::analysisTIMEPIX()
{

	// HISTOGRAMS INITIALIZATION IF NEEDED


	std::cout << "Method to implment as requested by Antoni and Paulina (PhaseSpace Incoming)" << std::endl;
/*
	Int_t i(0);
	Int_t nbytes(0);

	while(i < entries)
	{
		nbytes += PhaseSpaceActor->GetEntry( i );

	// CONDITION TO ADD TO THE VECTOR (I am not sure what kind of conditions you would like to apply)
		if (!!! FILL IN THE CONDITION !!!)	
		{
		// Example command to fill the vector with protons 
			positrons.push_back(Proton(RunID, EventID, TrackID, ParentID, Time, X, Y, Z, dX, dY, dZ))
		}
		i++;
	}
*/

}





void PhaseSpace::closeRootFile()
{
	f->Close();
}
