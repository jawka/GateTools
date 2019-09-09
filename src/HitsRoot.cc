#include "HitsRoot.hh"


HitsRoot::HitsRoot(std::string ro_string, std::string scanner_mode, int analysis_mode)
{
	entries = 0;
	iterr = 0;
	none = 0;
	one = 0;
	two = 0;
	mode = analysis_mode;
	file_name = ro_string + file_name_extension;
	scanner_name = scanner_mode;
	sfr_y = atof(std::getenv("PRM_ANNIHIL_Y"));
	res_mode = std::getenv("PRM_RES_MODE");

	if (!res_mode.compare("barrel"))
	{
		std::cout << "Barrel blurring applied." << std::endl;
		sigma_blurring_x = 0.0; //in mm 2.0 ???????
		sigma_blurring_y = 0.0; //in mm 2.0 ???????
		sigma_blurring_z = 7.8; //in mm 2.0 ???????
		sigma_blurring_t = 0.0; //in ns
	}
	else if (!res_mode.compare("lso"))
	{
		std::cout << "LSO blurring applied." << std::endl;
		sigma_blurring_x = 1.07; //in mm
		sigma_blurring_y = 1.07; //in mm
		sigma_blurring_z = 1.07; //in mm
		sigma_blurring_t = 0.0; //in ns #TODO
	}
	else if (!res_mode.compare("dualhead"))
	{
		std::cout << "Dualhead blurring applied." << std::endl;
		sigma_blurring_x = 7.8; //in mm 7.8 ???????
		sigma_blurring_y = 0.0; //in mm 2.0 ???????
		sigma_blurring_z = 6.0; //in mm 
		sigma_blurring_t = 0.0; //in ns
		std::cout << "Blurring:" << std::endl << "x:" << sigma_blurring_x << std::endl << "y:" << sigma_blurring_y << std::endl << "z:" << sigma_blurring_z << std::endl;
	}
	else 
	{
		sigma_blurring_x = atof(std::getenv("PRM_RES_X")); //in mm
		sigma_blurring_y = atof(std::getenv("PRM_RES_Y")); //in mm
		sigma_blurring_z = atof(std::getenv("PRM_RES_Z")); //in mm
		sigma_blurring_t = atof(std::getenv("PRM_RES_T")); //in ns
		std::cout << "Blurring:" << std::endl << "x:" << sigma_blurring_x << std::endl << "y:" << sigma_blurring_y << std::endl << "z:" << sigma_blurring_z << std::endl;
	}
	
	if (atoi(std::getenv("PRM_MODE")) != 10 && atoi(std::getenv("PRM_MODE")) != 12)
	{
		init();
	}
}



HitsRoot::~HitsRoot()
{
	Reset();
}



void HitsRoot::Reset()
{
	entries = 0;
	iterr = 0;
	none = 0;
	one = 0;
	two = 0;
}



// Setting up the temp values of annihilation properties from
void HitsRoot::setTempElements()
{
	a_runID = positrons->front().run_id;
	a_eventID = positrons->front().event_id;
	a_trackID = positrons->front().track_id;
	a_parentID = positrons->front().parent_id;
	a_X = positrons->front().x;
	a_Y = positrons->front().y;
	a_Z = positrons->front().z;
	a_time = positrons->front().time;
	a_trackID_gamma1 = positrons->front().gamma_track1;
	a_trackID_gamma2 = positrons->front().gamma_track2;

}


//Setter for pointer to the positrons vector
void HitsRoot::setPositrons(std::vector<Positron>* vec_positrons)
{
	positrons = vec_positrons;
}

void HitsRoot::setProtons(std::vector<Proton>* vec_protons)
{
	protons = vec_protons;
}


int HitsRoot::hitsVectorAnalysis(std::vector<Hit> vec_hits, TH1F* annihil_gamma_energy_deposition_spectrum)
{
	//CODE FOR THE ANALYSIS !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	temp_trackID = vec_hits.front().track_id;
	Float_t e1 = 0.;
	Float_t e2 = 0.;
	compton_no = 1;
	
	for (Int_t j=0; j<vec_hits.size(); j++)
	{			
		if (compton_no == 1)
		{
			if (vec_hits.at(j).e_dep > e1)
				e1 = vec_hits.at(j).e_dep;
		}
		if(vec_hits.at(j).track_id != temp_trackID) 
		{	
			compton_no = 2;
			if (vec_hits.at(j).e_dep > e2)
				e2 = vec_hits.at(j).e_dep;
		}
	}
	if (compton_no == 2)
	{	
		annihil_gamma_energy_deposition_spectrum->Fill(e1*1000.);
		annihil_gamma_energy_deposition_spectrum->Fill(e2*1000.);
	}
	return compton_no;
}






float HitsRoot::z_straightforward_recon(std::vector<Hit> vec_hits)
{

	//CODE FOR THE ANALYSIS !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	temp_trackID = vec_hits.front().track_id;
//	bool flag_first = true;
//	bool flag_second = true;
//	std::cout << "parentID (positron track_ID): " << a_trackID << "; trackID_1: " << a_trackID_gamma1 << "; trackID_2: " <<  a_trackID_gamma2 << std::endl;
	coincidence_hits.clear();
	compton_no = 1;
	if (vec_hits.size() == 2)
		coincidence_hits = vec_hits;
	else
	{
		for (Int_t j=0; j<vec_hits.size(); j++)
		{			
			coincidence_hits.push_back(vec_hits.at(j));			
			if (vec_hits.at(j).track_id == temp_trackID && coincidence_hits.size() == 2)
			{			
				coincidence_hits.erase(coincidence_hits.end());
			}
			if(vec_hits.at(j).track_id != temp_trackID && coincidence_hits.size() == 3) 			
			{	
				coincidence_hits.erase(coincidence_hits.end());
			}
		}
	}



	if (coincidence_hits.size() !=2)
	{
		std::cout << "vec_hits size: " << vec_hits.size() << std::endl;
		for (Int_t ii=0; ii<vec_hits.size(); ii++)
		{
			std::cout << "parent_ID: " << vec_hits.at(ii).parent_id << "; track_id: " << vec_hits.at(ii).track_id << "; edep: " << vec_hits.at(ii).e_dep << "; nPhantomCompton: " << vec_hits.at(ii).nPhantomCompton << "; nPhantomRayleigh: " << vec_hits.at(ii).nPhantomRayleigh << "; nCrystalCompton: " << vec_hits.at(ii).nCrystalCompton << "; nCrystalRayleigh: " << vec_hits.at(ii).nCrystalRayleigh << std::endl;


		}
		
		std::cout << "coincidence_hits size: " << coincidence_hits.size() << std::endl;
		for (Int_t ii=0; ii<coincidence_hits.size(); ii++)
		{
			std::cout << "parent_ID: " << vec_hits.at(ii).parent_id << "; track_id: " << coincidence_hits.at(ii).track_id << "; edep: " << coincidence_hits.at(ii).e_dep << "; nPhantomCompton: " << coincidence_hits.at(ii).nPhantomCompton << "; nPhantomRayleigh: " << coincidence_hits.at(ii).nPhantomRayleigh << "; nCrystalCompton: " << coincidence_hits.at(ii).nCrystalCompton << "; nCrystalRayleigh: " << coincidence_hits.at(ii).nCrystalRayleigh << std::endl;

		}
		std::cout << std::endl;
	}




	y1 = coincidence_hits.at(0).y; //in mm
	y2 = coincidence_hits.at(1).y; //in mm
	
	// Scalling factor calculation
	scaling_param_sfr = (y1-sfr_y)/(y2-y1);

	// Annihilation point calculation
	x_prim = x1 + scaling_param_sfr*(x2-x1);
	y_prim = sfr_y;
	z_prim = z1 + scaling_param_sfr*(z2-z1);


	return z_prim;

}





float HitsRoot::z_range_estimation(std::vector<Hit> vec_hits)
{
	//CODE FOR THE ANALYSIS !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
	temp_trackID = vec_hits.front().track_id;
//	bool flag_first = true;
//	bool flag_second = true;
//	std::cout << "parentID (positron track_ID): " << a_trackID << "; trackID_1: " << a_trackID_gamma1 << "; trackID_2: " <<  a_trackID_gamma2 << std::endl;
	coincidence_hits.clear();
	compton_no = 1;
	if (vec_hits.size() == 2)
		coincidence_hits = vec_hits;
	else
	{
		for (Int_t j=0; j<vec_hits.size(); j++)
		{			
			coincidence_hits.push_back(vec_hits.at(j));			
			if (vec_hits.at(j).track_id == temp_trackID && coincidence_hits.size() == 2)
			{			
				coincidence_hits.erase(coincidence_hits.end());
			}
			if(vec_hits.at(j).track_id != temp_trackID && coincidence_hits.size() == 3) 			
			{	
				coincidence_hits.erase(coincidence_hits.end());
			}
		}
	}



	if (coincidence_hits.size() !=2)
	{
		std::cout << "vec_hits size: " << vec_hits.size() << std::endl;
		for (Int_t ii=0; ii<vec_hits.size(); ii++)
		{
			std::cout << "parent_ID: " << vec_hits.at(ii).parent_id << "; track_id: " << vec_hits.at(ii).track_id << "; edep: " << vec_hits.at(ii).e_dep << "; nPhantomCompton: " << vec_hits.at(ii).nPhantomCompton << "; nPhantomRayleigh: " << vec_hits.at(ii).nPhantomRayleigh << "; nCrystalCompton: " << vec_hits.at(ii).nCrystalCompton << "; nCrystalRayleigh: " << vec_hits.at(ii).nCrystalRayleigh << std::endl;


		}
		
		std::cout << "coincidence_hits size: " << coincidence_hits.size() << std::endl;
		for (Int_t ii=0; ii<coincidence_hits.size(); ii++)
		{
			std::cout << "parent_ID: " << vec_hits.at(ii).parent_id << "; track_id: " << coincidence_hits.at(ii).track_id << "; edep: " << coincidence_hits.at(ii).e_dep << "; nPhantomCompton: " << coincidence_hits.at(ii).nPhantomCompton << "; nPhantomRayleigh: " << coincidence_hits.at(ii).nPhantomRayleigh << "; nCrystalCompton: " << coincidence_hits.at(ii).nCrystalCompton << "; nCrystalRayleigh: " << coincidence_hits.at(ii).nCrystalRayleigh << std::endl;

		}
		std::cout << std::endl;
	}

	// Hits points blurring

	
	if (!res_mode.compare("barrel"))
	{
		x1 = coincidence_hits.at(0).x + r->Uniform(sigma_blurring_x) - sigma_blurring_x/2; //in mm
		x2 = coincidence_hits.at(1).x + r->Uniform(sigma_blurring_x) - sigma_blurring_x/2; //in mm
		
		y1 = coincidence_hits.at(0).y + r->Uniform(sigma_blurring_y) - sigma_blurring_y/2; //in mm
		y2 = coincidence_hits.at(1).y + r->Uniform(sigma_blurring_y) - sigma_blurring_y/2; //in mm
	
		z1 = r->Gaus(coincidence_hits.at(0).z, sigma_blurring_z); //in mm
		z2 = r->Gaus(coincidence_hits.at(1).z, sigma_blurring_z); //in mm

		t1 = r->Gaus(coincidence_hits.at(0).time, sigma_blurring_t); //in ns
		t2 = r->Gaus(coincidence_hits.at(1).time, sigma_blurring_t); //in ns
	}

	else if (!res_mode.compare("dualhead"))
	{
		x1 = r->Gaus(coincidence_hits.at(0).x, sigma_blurring_x); //in mm
		x2 = r->Gaus(coincidence_hits.at(1).x, sigma_blurring_x); //in mm
	
		y1 = coincidence_hits.at(0).y + r->Uniform(sigma_blurring_y) - sigma_blurring_y/2; //in mm
		y2 = coincidence_hits.at(1).y + r->Uniform(sigma_blurring_y) - sigma_blurring_y/2; //in mm
	
		z1 = coincidence_hits.at(0).z + r->Uniform(sigma_blurring_z) - sigma_blurring_z/2; //in mm
		z2 = coincidence_hits.at(1).z + r->Uniform(sigma_blurring_z) - sigma_blurring_z/2; //in mm

		t1 = r->Gaus(coincidence_hits.at(0).time, sigma_blurring_t); //in ns
		t2 = r->Gaus(coincidence_hits.at(1).time, sigma_blurring_t); //in ns
	}

	else
	{
		x1 = r->Gaus(coincidence_hits.at(0).x, sigma_blurring_x); //in mm
		x2 = r->Gaus(coincidence_hits.at(1).x, sigma_blurring_x); //in mm
	
		y1 = r->Gaus(coincidence_hits.at(0).y, sigma_blurring_y); //in mm
		y2 = r->Gaus(coincidence_hits.at(1).y, sigma_blurring_y); //in mm
	
		z1 = r->Gaus(coincidence_hits.at(0).z, sigma_blurring_z); //in mm
		z2 = r->Gaus(coincidence_hits.at(1).z, sigma_blurring_z); //in mm

		t1 = r->Gaus(coincidence_hits.at(0).time, sigma_blurring_t); //in ns
		t2 = r->Gaus(coincidence_hits.at(1).time, sigma_blurring_t); //in ns
	}

/*	if (std::getenv("PRM_RES_MODE") == "barrel")
	{
		if (z1 > atof(std::getenv("PRM_PH_MAX"))-50.) {z1 = atof(std::getenv("PRM_PH_MAX"))-50.;}
		if (z1 < atof(std::getenv("PRM_PH_MIN"))+50.) {z1 = atof(std::getenv("PRM_PH_MIN"))+50.;}
		if (z2 > atof(std::getenv("PRM_PH_MAX"))-50.) {z2 = atof(std::getenv("PRM_PH_MAX"))-50.;}
		if (z2 < atof(std::getenv("PRM_PH_MIN"))+50.) {z2 = atof(std::getenv("PRM_PH_MIN"))+50.;}
	}
*/

	// ANNIHILATION POINT CALCULATION - BASED ON TOF
/*	delta_length = light_velocity*(t1-t2)/2.;
	LOR_length = sqrt(pow((x2-x1),2.)+pow((y2-y1),2.)+pow((z2-z1),2.));
	scaling_param = 0.5 + (delta_length/LOR_length);
	x_prim = x1 + scaling_param*(x2-x1);
	y_prim = y1 + scaling_param*(y2-y1);
	z_prim = z1 + scaling_param*(z2-z1);
*/



	// ANNIHILATION POINT CALCULATION - BASED ON ANTONI'S IDEA
	x_prim = (x1+x2)/2;
	y_prim = (y1+y2)/2;
	z_prim = (z1+z2)/2;


	// Check
/*		
	std::cout << "Primary hits:" << std::endl; 
	std::cout << "H1: " << coincidence_hits.at(0).time << " ns; " << coincidence_hits.at(0).x <<" mm; " << coincidence_hits.at(0).y << " mm; " << coincidence_hits.at(0).z << " mm " << std::endl;
	std::cout << "H2: " << coincidence_hits.at(1).time << " ns; " << coincidence_hits.at(1).x <<" mm; " << coincidence_hits.at(1).y << " mm; " << coincidence_hits.at(1).z << " mm" << std::endl;

	std::cout << "Blurred hits:" << std::endl;
	std::cout << "H1: " << t1 << " ns; " << x1 <<" mm; " << y1 << " mm; " << z1 << " mm" << std::endl;
	std::cout << "H2: " << t2 << " ns; " << x2 <<" mm; " << y2 << " mm; " << z2 << " mm" << std::endl;

	std::cout << "delta_time: " << delta_time << " ns" << std::endl;	
	std::cout << "delta_length: " << delta_length << " mm" << std::endl;
	std::cout << "LOR_length: " << LOR_length << " mm" << std::endl;
	std::cout << "scaling_param: " << scaling_param << std::endl;
	std::cout << "Prim vector length: " << (x2-x1) << " mm; " << (y2-y1) << " mm; " << (z2-z1) << " mm" << std::endl;

	std::cout << "New annihilation point: " << x_prim <<" mm; " << y_prim << " mm; " << z_prim << " mm" << std::endl << std::endl << std::endl;
*/
	return z_prim;
}


bool HitsRoot::scatteringFlag (std::vector<Hit> vec_hits)
{
	
	for (Int_t j=0; j<vec_hits.size(); j++)
	{			
		if (vec_hits.at(j).nPhantomCompton !=0 || vec_hits.at(j).nPhantomRayleigh !=0 || vec_hits.at(j).nPhantomCompton !=0 || vec_hits.at(j).nPhantomRayleigh !=0)
		{
//			std::cout << "Compton no.: " << vec_hits.at(j).nPhantomCompton << "; Rayleigh no.: " << vec_hits.at(j).nPhantomRayleigh << std::endl << std::endl;
			return true;
		}
	}
	return false;
}



void HitsRoot::init()
{

	f = new TFile(file_name.c_str(), "READ");

	if( f->IsZombie() )
	{
		std::cerr << "Cannot open the file: '" << file_name  << "'" << std::endl;
		exit( EXIT_FAILURE );
	}
	
	HitsChain = (TTree*)gDirectory->Get("Hits");
	
	//
	// SET BRANCH ADDRESSES - TTREE PHASESPACE
	//

	HitsChain->SetBranchAddress("PDGEncoding",&PDGEncoding);
	HitsChain->SetBranchAddress("trackID",&trackID);
	HitsChain->SetBranchAddress("parentID",&parentID);
	HitsChain->SetBranchAddress("trackLocalTime",&trackLocalTime);
	HitsChain->SetBranchAddress("time",&time);
	HitsChain->SetBranchAddress("edep",&edep);
	HitsChain->SetBranchAddress("stepLength",&stepLength);
	HitsChain->SetBranchAddress("trackLength",&trackLength);
	HitsChain->SetBranchAddress("posX",&posX);
	HitsChain->SetBranchAddress("posY",&posY);
	HitsChain->SetBranchAddress("posZ",&posZ);
	HitsChain->SetBranchAddress("localPosX",&localPosX);
	HitsChain->SetBranchAddress("localPosY",&localPosY);
	HitsChain->SetBranchAddress("localPosZ",&localPosY);
	HitsChain->SetBranchAddress("momDirX",&momDirX);
	HitsChain->SetBranchAddress("momDirY",&momDirY);
	HitsChain->SetBranchAddress("momDirZ",&momDirZ);
	HitsChain->SetBranchAddress("baseID",&baseID);
	HitsChain->SetBranchAddress("runID",&runID);
	HitsChain->SetBranchAddress("level1ID",&level1ID);
	HitsChain->SetBranchAddress("level2ID",&level2ID);
	HitsChain->SetBranchAddress("level3ID",&level3ID);
	HitsChain->SetBranchAddress("level4ID",&level4ID);
	HitsChain->SetBranchAddress("layerID",&layerID);
	HitsChain->SetBranchAddress("level5ID",&level5ID);
	HitsChain->SetBranchAddress("photonID",&photonID);
	HitsChain->SetBranchAddress("nPhantomCompton",&nPhantomCompton);
	HitsChain->SetBranchAddress("nCrystalCompton",&nCrystalCompton);
	HitsChain->SetBranchAddress("nPhantomRayleigh",&nPhantomRayleigh);
	HitsChain->SetBranchAddress("nCrystalRayleigh",&nCrystalRayleigh);
	HitsChain->SetBranchAddress("primaryID",&primaryID);
	HitsChain->SetBranchAddress("sourcePosX",&sourcePosX);
	HitsChain->SetBranchAddress("sourcePosY",&sourcePosY);
	HitsChain->SetBranchAddress("sourcePosZ",&sourcePosZ);
	HitsChain->SetBranchAddress("sourceID",&sourceID);
	HitsChain->SetBranchAddress("eventID",&eventID);
	HitsChain->SetBranchAddress("runID",&runID);
	HitsChain->SetBranchAddress("axialPos",&axialPos);
	HitsChain->SetBranchAddress("rotationAngle",&rotationAngle);
	HitsChain->SetBranchAddress("volumeID",&volumeID);
	HitsChain->SetBranchAddress("processName",&processName);
	HitsChain->SetBranchAddress("comptVolName",&comptVolName);
	HitsChain->SetBranchAddress("RayleighVolName",&RayleighVolName);

	//
	// ENTRIES NUMBER
	//

	entries = HitsChain->GetEntries();
	std::cout << "Number of entries in " << file_name_extension << " file: " << entries << std::endl;
}


void HitsRoot::analysis()
{
	std::string crystal_proc = std::getenv("PRM_EDEP_PROCESS");

	//
	// HISTOGRAMS INITIALIZATION
	//

	std::cout << "Histograms initialization ..." << std::endl;
	
	TH1F *z_none_compton = new TH1F( "z", "Annihilations with no compton scattering within the JPET plastic", atoi(std::getenv("PRM_PH_BINS")), atof(std::getenv("PRM_PH_MIN")), atof(std::getenv("PRM_PH_MAX")));
	z_none_compton->GetXaxis()->SetTitle("Z [mm]");
	z_none_compton->GetYaxis()->SetTitle("Number of annihilations");

	TH1F *time_none_compton = new TH1F( "annihil_time", "Annihilations with no compton scattering within the JPET plastic", 500, 0, 5000);
	time_none_compton->GetXaxis()->SetTitle("Time [s] - 10 seconds intervals");
	time_none_compton->GetYaxis()->SetTitle("Number of annihilations");

	TH1F *z_one_compton = new TH1F( "z", "Annihilations with one compton scattering within the JPET plastic", atoi(std::getenv("PRM_PH_BINS")), atof(std::getenv("PRM_PH_MIN")), atof(std::getenv("PRM_PH_MAX")));
	z_one_compton->GetXaxis()->SetTitle("Z [mm]");
	z_one_compton->GetYaxis()->SetTitle("Number of annihilations");
	
	TH1F *time_one_compton = new TH1F( "annihil_time", "Annihilations with one compton scattering within the JPET plastic", 500, 0, 5000);
	time_one_compton->GetXaxis()->SetTitle("Time [s] - 10 seconds intervals");
	time_one_compton->GetYaxis()->SetTitle("Number of annihilations");

	TH1F *z_two_compton = new TH1F( "z", "Annihilations with two compton scatterings within the JPET plastic", atoi(std::getenv("PRM_PH_BINS")), atof(std::getenv("PRM_PH_MIN")), atof(std::getenv("PRM_PH_MAX")));
	z_two_compton->GetXaxis()->SetTitle("Z [mm]");
	z_two_compton->GetYaxis()->SetTitle("Number of annihilations");
	
	TH1F *time_two_compton = new TH1F( "annihil_time", "Annihilations with two compton scatterings within the JPET plastic", 500, 0, 5000);
	time_two_compton->GetXaxis()->SetTitle("Time [s] - 10 seconds intervals");
	time_two_compton->GetYaxis()->SetTitle("Number of annihilations");

	TH1F *interspill_time_two_compton = new TH1F( "Annihilations with two compton scatterings within the JPET plastic in interspill phase", "Annihilations with two compton scatterings within the JPET plastic in interspill phase", 120, 0., 120.); // in seconds
	interspill_time_two_compton->GetXaxis()->SetTitle("Time [s] - 10 seconds intervals");
	interspill_time_two_compton->GetYaxis()->SetTitle("Number of annihilations");

	TH1F *inbeam_time_two_compton = new TH1F( "Annihilations with two compton scatterings within the JPET plastic in inbeam phase", "Annihilations with two compton scatterings within the JPET plastic in inbeam phase", 110, 0, 120);   // in miliseconds
	inbeam_time_two_compton->GetXaxis()->SetTitle("Time [ms] - 10 miliseconds intervals");
	inbeam_time_two_compton->GetYaxis()->SetTitle("Number of annihilations");


	TH1F *annihil_gamma_energy_deposition_spectrum = new TH1F( "Annihilation gammas deposition energy", "Annihilation gammas deposition energy [keV]", 100, 0.0, 500.0);
	annihil_gamma_energy_deposition_spectrum->GetXaxis()->SetTitle("Energy [keV]");
	annihil_gamma_energy_deposition_spectrum->GetYaxis()->SetTitle("Number of annihilations gammas");




	TH1F *z_profile_inbeam = new TH1F( "Registered annihilations in coincidence - inbeam", "Registered annihilations in coincidence", atoi(std::getenv("PRM_PH_BINS")), atof(std::getenv("PRM_PH_MIN")), atof(std::getenv("PRM_PH_MAX")));
	z_profile_inbeam->GetXaxis()->SetTitle("Phantom depth [mm]");
	z_profile_inbeam->GetYaxis()->SetTitle("Number of registered annihilations in coincidence");

	TH1F *z_profile_interspill = new TH1F( "Registered annihilations in coincidence - interspill", "Registered annihilations in coincidence", atoi(std::getenv("PRM_PH_BINS")), atof(std::getenv("PRM_PH_MIN")), atof(std::getenv("PRM_PH_MAX")));
	z_profile_interspill->GetXaxis()->SetTitle("Phantom depth [mm]");
	z_profile_interspill->GetYaxis()->SetTitle("Number of registered annihilations in coincidence");

	TH1F *z_profile_offbeam = new TH1F( "Registered annihilations in coincidence - offbeam", "Registered annihilations in coincidence", atoi(std::getenv("PRM_PH_BINS")), atof(std::getenv("PRM_PH_MIN")), atof(std::getenv("PRM_PH_MAX")));
	z_profile_offbeam->GetXaxis()->SetTitle("Phantom depth [mm]");
	z_profile_offbeam->GetYaxis()->SetTitle("Number of registered annihilations in coincidence");

	TH1F *z_integrated = new TH1F( "Registered annihilations in coincidence - integrated over time", "Registered annihilations in coincidence", atoi(std::getenv("PRM_PH_BINS")), atof(std::getenv("PRM_PH_MIN")), atof(std::getenv("PRM_PH_MAX")));
	z_integrated->GetXaxis()->SetTitle("Phantom depth [mm]");
	z_integrated->GetYaxis()->SetTitle("Number of registered annihilations in coincidence");




	TH1F *z_original_interspill = new TH1F( "Original place of gammas annihilation - interspill phase", "Original place of gammas annihilation - interspill phase", atoi(std::getenv("PRM_PH_BINS")), atof(std::getenv("PRM_PH_MIN")), atof(std::getenv("PRM_PH_MAX")));
	z_original_interspill->GetXaxis()->SetTitle("Phantom depth [mm]");
	z_original_interspill->GetYaxis()->SetTitle("Number of registered annihilations in coincidence");

	TH1F *z_original_integrated = new TH1F( "Original place of gammas annihilation - integrated over time", "Original place of gammas annihilation - integrated over time", atoi(std::getenv("PRM_PH_BINS")), atof(std::getenv("PRM_PH_MIN")), atof(std::getenv("PRM_PH_MAX")));
	z_original_integrated->GetXaxis()->SetTitle("Phantom depth [mm]");
	z_original_integrated->GetYaxis()->SetTitle("Number of registered annihilations in coincidence");




	TH1F *z_diff_interspill = new TH1F( "Delta z - interspill", "Delta z - interspill", 200, -50.0, 50.0);
	z_diff_interspill->GetXaxis()->SetTitle("Difference [mm]");
	z_diff_interspill->GetYaxis()->SetTitle("Number of registered annihilations in coincidence");

	TH1F *z_diff_integrated = new TH1F( "Delta z - integrated over time", "Delta z - integrated over time", 200, -50.0, 50.0);
	z_diff_integrated->GetXaxis()->SetTitle("Difference [mm]");
	z_diff_integrated->GetYaxis()->SetTitle("Number of registered annihilations in coincidence");


	std::string dualhead_string = "dualhead";
	std::string lso_string = "lso";
	bool sfr_flag = false;
	



	TH1F *z_profile_inbeam_sfr = new TH1F( "Registered annihilations in coincidence - inbeam - SFR recon", "Registered annihilations in coincidence - SFR recon", atoi(std::getenv("PRM_PH_BINS")), atof(std::getenv("PRM_PH_MIN")), atof(std::getenv("PRM_PH_MAX")));
	z_profile_inbeam_sfr->GetXaxis()->SetTitle("Phantom depth [mm]");
	z_profile_inbeam_sfr->GetYaxis()->SetTitle("Number of registered annihilations in coincidence");

	TH1F *z_profile_interspill_sfr = new TH1F( "Registered annihilations in coincidence - interspill - SFR recon", "Registered annihilations in coincidence - SFR recon", atoi(std::getenv("PRM_PH_BINS")), atof(std::getenv("PRM_PH_MIN")), atof(std::getenv("PRM_PH_MAX")));
	z_profile_interspill_sfr->GetXaxis()->SetTitle("Phantom depth [mm]");
	z_profile_interspill_sfr->GetYaxis()->SetTitle("Number of registered annihilations in coincidence");

	TH1F *z_profile_offbeam_sfr = new TH1F( "Registered annihilations in coincidence - offbeam - SFR recon", "Registered annihilations in coincidence - SFR recon", atoi(std::getenv("PRM_PH_BINS")), atof(std::getenv("PRM_PH_MIN")), atof(std::getenv("PRM_PH_MAX")));
	z_profile_offbeam_sfr->GetXaxis()->SetTitle("Phantom depth [mm]");
	z_profile_offbeam_sfr->GetYaxis()->SetTitle("Number of registered annihilations in coincidence");

	TH1F *z_integrated_sfr = new TH1F( "Registered annihilations in coincidence - integrated over time - SFR recon", "Registered annihilations in coincidence - SFR recon", atoi(std::getenv("PRM_PH_BINS")), atof(std::getenv("PRM_PH_MIN")), atof(std::getenv("PRM_PH_MAX")));
	z_integrated_sfr->GetXaxis()->SetTitle("Phantom depth [mm]");
	z_integrated_sfr->GetYaxis()->SetTitle("Number of registered annihilations in coincidence");
	

	TH1F *z_diff_interspill_sfr = new TH1F( "Delta z - interspill - SFR recon", "Delta z - interspill - SFR recon", 200, -50.0, 50.0);
	z_diff_interspill_sfr->GetXaxis()->SetTitle("Difference [mm]");
	z_diff_interspill_sfr->GetYaxis()->SetTitle("Number of registered annihilations in coincidence");

	TH1F *z_diff_integrated_sfr = new TH1F( "Delta z - integrated over time - SFR recon", "Delta z - integrated over time - SFR recon", 200, -50.0, 50.0);
	z_diff_integrated_sfr->GetXaxis()->SetTitle("Difference [mm]");
	z_diff_integrated_sfr->GetYaxis()->SetTitle("Number of registered annihilations in coincidence");

	std::string true_flag_string = "true";

	if (std::getenv("PRM_SFR_FLAG") == true_flag_string and (std::getenv("PRM_RES_MODE") == dualhead_string or std::getenv("PRM_RES_MODE") == lso_string))
	{	
		std::cout << "STRAIGHTFORWARD RECONSTRUCTION:" << std::getenv("PRM_SFR_FLAG") << std::endl;
		sfr_flag = true;
	}

	std::cout << "Main analysis ..." << std::endl;
	std::cout << "Size of the positrons vectors in hits analysis: " << positrons->size() << std::endl;

	this->setTempElements();
	positrons->erase(positrons->begin());

	std::vector<Hit> hitss;
	hitss.clear();
	int compt_number = 0;	
	bool scatter_flag = false;
	float z_smoothed = 0.;
	float z_sfr = 0.;

	Float_t eth_min = atof(std::getenv("PRM_E_MIN"));
	Float_t eth_max = atof(std::getenv("PRM_E_MAX"));

	// LOOP ACROOS ALL ENTRIES
	std::cout << "Number of entries: " << entries << std::endl;
	Int_t nbytes(0);
	
	while (iterr < entries)
	{
		if (iterr%1000000 == 0)
			std::cout << iterr/1000000 << " M events analysed." << std::endl;

		nbytes += HitsChain->GetEntry(iterr);

		
		if (eventID > a_eventID && !positrons->empty())
		{
			
			if (!hitss.empty())
			{				
								
				if (hitss.size() == 1)
				{
					z_one_compton->Fill(a_Z);
					time_one_compton->Fill(a_time/1000000000.);
					iterr++;
					this->one++;
					hitss.clear();
					continue;
				}

				else
				{
					compt_number = this->hitsVectorAnalysis(hitss, annihil_gamma_energy_deposition_spectrum);
					if (compt_number == 1)
					{
						z_one_compton->Fill(a_Z);
						time_one_compton->Fill(a_time/1000000000.);
						this->one++;
					}
					else
					{
						z_two_compton->Fill(a_Z);
						time_two_compton->Fill(a_time/1000000000.);
						this->two++;
						z_smoothed = this->z_range_estimation(hitss);
						if (sfr_flag)
							z_sfr = this->z_straightforward_recon(hitss);
						if (a_time < time_slice)
						{
							inbeam_time_two_compton->Fill(a_time/1000000.);
							this->inbeam_counter++;
							scatter_flag = scatteringFlag (hitss);
							if (scatter_flag)
							{
								inbeam_scattered++;
								
							}
							else
							{	
								inbeam_non_scattered++;
								z_profile_inbeam->Fill(z_smoothed);
								if(sfr_flag)
									z_profile_inbeam_sfr->Fill(z_sfr);
								
							}
//							z_profile_inbeam->Fill(z_smoothed);						
						}	
						else if (a_time < 300000000000.)
						{
							interspill_time_two_compton->Fill(a_time/1000000000.);
							this->interspill_counter++;
							scatter_flag = scatteringFlag (hitss);
							if (scatter_flag)
							{
								interspill_scattered++;

							}
							else
							{
								interspill_non_scattered++;
								z_profile_interspill->Fill(z_smoothed);
								z_integrated->Fill(z_smoothed);
								z_original_interspill->Fill(a_Z);
								z_original_integrated->Fill(a_Z);
								z_diff_interspill->Fill(a_Z-z_smoothed);
								z_diff_integrated->Fill(a_Z-z_smoothed);
								if (sfr_flag)
								{
									z_profile_interspill_sfr->Fill(z_sfr);
									z_integrated_sfr->Fill(z_sfr);
									z_diff_interspill_sfr->Fill(a_Z-z_sfr);
									z_diff_integrated_sfr->Fill(a_Z-z_sfr);
								}

							}
//							z_profile_interspill->Fill(z_smoothed);						
						}
						else
						{
							scatter_flag = scatteringFlag (hitss);
							if (scatter_flag)
							{
								offbeam_scattered++;
							}
							else
							{	
								offbeam_non_scattered++;
								z_profile_offbeam->Fill(z_smoothed);
								z_integrated->Fill(z_smoothed);
								z_original_integrated->Fill(a_Z);
								z_diff_integrated->Fill(a_Z-z_smoothed);
								if (sfr_flag)
								{
									z_profile_offbeam_sfr->Fill(z_sfr);
									z_integrated_sfr->Fill(z_sfr);
									z_diff_integrated_sfr->Fill(a_Z-z_sfr);
								}
							}
//							z_profile_offbeam->Fill(z_smoothed);
						}
					}					
					iterr++;
					hitss.clear();
					continue;
				}
			}

			else 
			{
				time_none_compton->Fill(a_time/1000000000.);
				z_none_compton->Fill(a_Z);
				this->none++;
				hitss.clear();
			}
	
			if (positrons->empty())
			{
				break;
			}
			this->setTempElements();
			positrons->erase(positrons->begin());

		}

		// Looking for the hit 
		if (eventID == a_eventID && parentID == a_trackID && crystal_proc.compare(processName) == 0 && PDGEncoding == 22 && (trackID == a_trackID_gamma1 || trackID == a_trackID_gamma2))		
		{			
			if (edep > eth_min and edep < eth_max)
			{
				hitss.push_back(Hit(runID, eventID, trackID, parentID, time, edep, posX, posY, posZ, nPhantomCompton, nPhantomRayleigh, nCrystalCompton, nCrystalRayleigh));		
			}			
		}//*/
		iterr++;
	}


	std::cout << "None: " << this->none << "; one: " << this->one << "; two: " << this->two << std::endl << std::endl;
	std::cout << "Inbeam coincidences: " << this->inbeam_counter << std::endl << "Interspill coincidences: " << this->interspill_counter << std::endl << "Offbeam coincidences: " << (this->two - this->interspill_counter - this->inbeam_counter) << std::endl << std::endl;
	
	std::cout << "###### TRUE AND SCATTERED FRACTION ######" << std::endl;
	std::cout << "INBEAM" << std::endl;
	std::cout << "True: " << inbeam_non_scattered << std::endl;
	std::cout << "Scattered: " << inbeam_scattered << std::endl << std::endl;

	std::cout << "INTERSPILL" << std::endl;
	std::cout << "True: " << interspill_non_scattered << std::endl;
	std::cout << "Scattered: " << interspill_scattered << std::endl << std::endl;

	std::cout << "OFFBEAM" << std::endl;
	std::cout << "True: " << offbeam_non_scattered << std::endl;
	std::cout << "Scattered: " << offbeam_scattered << std::endl ;
	std::cout << "###### END ######" << std::endl;

	gStyle->SetPalette();

	std::string f_name = "/home/baran/Desktop/root_hits_"+scanner_name+"_analysis_compton_number.root";
	TFile fw(f_name.c_str(),"RECREATE");
	TCanvas* plot = new TCanvas("Hits output","Hits output",2000,1400);
	plot->Divide(2,3);
	plot->cd(1); z_none_compton->Draw();z_none_compton->Write();
	plot->cd(4); time_none_compton->Draw();time_none_compton->Write();
	plot->cd(2); z_one_compton->Draw();z_one_compton->Write();
	plot->cd(5); time_one_compton->Draw();time_one_compton->Write();
	plot->cd(3); z_two_compton->Draw();z_two_compton->Write();
	plot->cd(6); time_two_compton->Draw();time_two_compton->Write();
	plot->Update();

	TImage *img = TImage::Create();
	img->FromPad(plot);
	f_name = "/home/baran/Desktop/root_hits_"+scanner_name+"_analysis_compton_number.png";
	img->WriteImage(f_name.c_str());
	
	fw.Print();
	fw.Close();


	f_name = "/home/baran/Desktop/root_hits_"+scanner_name+"_analysis_phases.root";
	TFile fw2(f_name.c_str(),"RECREATE");
	TCanvas* plot2 = new TCanvas("Hits output 2","Hits output 2",2000,1400);
	plot2->Divide(2,2);
	plot2->cd(1); inbeam_time_two_compton->Draw();inbeam_time_two_compton->Write();
	plot2->cd(2); interspill_time_two_compton->Draw();interspill_time_two_compton->Write();
	plot2->cd(3); annihil_gamma_energy_deposition_spectrum->Draw();annihil_gamma_energy_deposition_spectrum->Write();
	plot2->Update();

	TImage *img2 = TImage::Create();
	img2->FromPad(plot2);
	f_name = "/home/baran/Desktop/root_hits_"+scanner_name+"_analysis_phases.png";
	img2->WriteImage(f_name.c_str());

	fw2.Print();
	fw2.Close();




	f_name = "/home/baran/Desktop/root_hits_"+scanner_name+"_analysis_z_range.root";
	TFile fw3(f_name.c_str(),"RECREATE");
	TCanvas* plot3 = new TCanvas("Hits output 3","Hits output 3",2000,1400);
	plot3->Divide(2,2);
	plot3->cd(1); z_profile_inbeam->Draw();z_profile_inbeam->Write();
	plot3->cd(2); z_profile_interspill->Draw();z_profile_interspill->Write();
	plot3->cd(3); z_profile_offbeam->Draw();z_profile_offbeam->Write();
//	plot3->cd(4); annihil_gamma_energy_deposition_spectrum->Draw();annihil_gamma_energy_deposition_spectrum->Write();
	plot3->Update();

	TImage *img3 = TImage::Create();
	img3->FromPad(plot3);
	f_name = "/home/baran/Desktop/root_hits_"+scanner_name+"_analysis_z_range.png";
	img3->WriteImage(f_name.c_str());

	fw3.Print();
	fw3.Close();



	if (sfr_flag)
	{
		f_name = "/home/baran/Desktop/root_hits_"+scanner_name+"_analysis_z_range.root";
		TFile fw3_sfr(f_name.c_str(),"UPDATE");
		TCanvas* plot3_sfr = new TCanvas("Hits output 2","Hits output 2",2000,1400);
		plot3_sfr->Divide(2,2);
		plot3_sfr->cd(1); z_profile_inbeam_sfr->Draw();z_profile_inbeam_sfr->Write();
		plot3_sfr->cd(2); z_profile_interspill_sfr->Draw();z_profile_interspill_sfr->Write();
		plot3_sfr->cd(3); z_profile_offbeam_sfr->Draw();z_profile_offbeam_sfr->Write();
		plot3_sfr->Update();
	
		TImage *img3_sfr = TImage::Create();
		img3_sfr->FromPad(plot3_sfr);
		f_name = "/home/baran/Desktop/root_hits_"+scanner_name+"_analysis_z_range_sfr.png";
		img3_sfr->WriteImage(f_name.c_str());
	
		fw3_sfr.Print();
		fw3_sfr.Close();

	}


	if (mode == 9)
	{
		f_name = "/home/baran/Desktop/root_IEEE_MIC_"+scanner_name+".root";
		TFile fw4(f_name.c_str(),"UPDATE");
	
		gStyle->SetPalette();
	
		TCanvas* plot4 = new TCanvas("Hits output 4","Hits output 4",2000,1400);
		plot4->Divide(2,3);
		if (sfr_flag)
		{		
			plot4->Divide(2,5);
		} 
		plot4->cd(1); z_profile_interspill->Draw();z_profile_interspill->Write();
		plot4->cd(2); z_integrated->Draw();z_integrated->Write();
		plot4->cd(3); z_original_interspill->Draw();z_original_interspill->Write();
		plot4->cd(4); z_original_integrated->Draw();z_original_integrated->Write();
		plot4->cd(5); z_diff_interspill->Draw();z_diff_interspill->Write();
		plot4->cd(6); z_diff_integrated->Draw();z_diff_integrated->Write();
		
		if (sfr_flag)
		{		
			plot4->cd(7); z_profile_interspill_sfr->Draw();z_profile_interspill_sfr->Write();		
			plot4->cd(8); z_integrated_sfr->Draw();z_integrated_sfr->Write();
			plot4->cd(9); z_diff_interspill_sfr->Draw();z_diff_interspill_sfr->Write();
			plot4->cd(10); z_diff_integrated_sfr->Draw();z_diff_integrated_sfr->Write();
		} 

		plot4->Update();
	
		TImage *img4 = TImage::Create();
		img4->FromPad(plot4);
		f_name = "/home/baran/Desktop/root_IEEE_MIC_"+scanner_name+"_pseudo_reconstruction.png";
		img4->WriteImage(f_name.c_str());
	
		fw4.Close();

		std::ofstream txt_output1;
		std::string txt_file = "/home/baran/Desktop/root_IEEE_MIC_"+scanner_name+"_z_profile_interspill.txt";
		txt_output1.open (txt_file);

		std::ofstream txt_output2;
		txt_file = "/home/baran/Desktop/root_IEEE_MIC_"+scanner_name+"_z_integrated.txt";
		txt_output2.open (txt_file);

		std::ofstream txt_output3;
		txt_file = "/home/baran/Desktop/root_IEEE_MIC_"+scanner_name+"_z_original_interspill.txt";
		txt_output3.open (txt_file);

		std::ofstream txt_output4;
		txt_file = "/home/baran/Desktop/root_IEEE_MIC_"+scanner_name+"_z_original_integrated.txt";
		txt_output4.open (txt_file);

		for (int k=0; k<atoi(std::getenv("PRM_PH_BINS")); k++)
		{
			txt_output1 << z_profile_interspill->GetBinContent(k) << std::endl;
			txt_output2 << z_integrated->GetBinContent(k) << std::endl;
			txt_output3 << z_original_interspill->GetBinContent(k) << std::endl;
			txt_output4 << z_original_integrated->GetBinContent(k) << std::endl;
		}

		txt_output1.close();
		txt_output2.close();
		txt_output3.close();
		txt_output4.close();
		
		if (sfr_flag)
		{
			std::ofstream txt_output1_sfr;
			txt_file = "/home/baran/Desktop/root_IEEE_MIC_"+scanner_name+"_z_profile_interspill_sfr.txt";
			txt_output1_sfr.open (txt_file);
	
			std::ofstream txt_output2_sfr;
			txt_file = "/home/baran/Desktop/root_IEEE_MIC_"+scanner_name+"_z_integrated_sfr.txt";
			txt_output2_sfr.open (txt_file);
	
			for (int k=0; k<atoi(std::getenv("PRM_PH_BINS")); k++)
			{
				txt_output1_sfr << z_profile_interspill_sfr->GetBinContent(k) << std::endl;
				txt_output2_sfr << z_integrated_sfr->GetBinContent(k) << std::endl;
			}

			txt_output1_sfr.close();
			txt_output2_sfr.close();
		}
		

	}



}






void HitsRoot::spectra_energy_analysis()
{
		
	TH1F *gamma_energy = new TH1F( "Gamma deposited energy", "Gamma energy", 500, 0, 0.5);
	gamma_energy->GetXaxis()->SetTitle("Energy [MeV]");
	gamma_energy->GetYaxis()->SetTitle("Number of gammas");

	TH1F *electron_energy = new TH1F( "Electron deposited energy", "Electron energy", 500, 0, 1);
	electron_energy->GetXaxis()->SetTitle("Energy [MeV]");
	electron_energy->GetYaxis()->SetTitle("Number of electrons (e-)");

	TH1F *positron_energy = new TH1F( "Positron deposited energy", "Positron energy", 500, 0, 1);
	positron_energy->GetXaxis()->SetTitle("Energy [MeV]");
	positron_energy->GetYaxis()->SetTitle("Number of positrons (e+)");

	TH1F *proton_energy = new TH1F( "Proton deposited energy", "Proton energy", 500, 0, 3);
	proton_energy->GetXaxis()->SetTitle("Energy [MeV]");
	proton_energy->GetYaxis()->SetTitle("Number of protons (p+)");

	TH1F *neutron_energy = new TH1F( "Neutron deposited energy", "Neutron energy", 500, 0, 0.1);
	neutron_energy->GetXaxis()->SetTitle("Energy [MeV]");
	neutron_energy->GetYaxis()->SetTitle("Number of neutrons (n)");

	Int_t i(0);
	Int_t nbytes(0);
	
	Int_t gamma(0);
	Int_t electron(0);
	Int_t proton(0);
	Int_t neutron(0);
	Int_t positron(0);

	Float_t max_gamma(0);
	Float_t max_electron(0);
	Float_t max_proton(0);
	Float_t max_neutron(0);
	Float_t max_positron(0);

	std::cout << "Number of entries: " << entries << std::endl;

	std::cout << "-------SETUP-------" << std::endl;
	std::cout << "Gammas number: " << gamma << std::endl;
	std::cout << "Electrons number: " << electron << std::endl;
	std::cout << "Positrons number: " << positron << std::endl;
	std::cout << "Protons number: " << proton << std::endl;
	std::cout << "Neutrons number: " << neutron << std::endl;
	std::cout << "--------END--------" << std::endl;

	while(i < entries)
	{
		if (i%1000000 == 0)
			std::cout << i/1000000 << " M events analysed." << std::endl;
	
		nbytes += HitsChain->GetEntry( i );

		if (PDGEncoding == 22)
		{
			gamma++;
			gamma_energy->Fill(edep);
			if (edep > max_gamma)
				max_gamma = edep;
		}

		else if (PDGEncoding == 11)
		{
			electron++;
			electron_energy->Fill(edep);
			if (edep > max_electron)
				max_electron = edep;
		}

		else if (PDGEncoding == -11)
		{
			positron++;
			positron_energy->Fill(edep);
			if (edep > max_positron)
				max_positron = edep;
		}

		else if (PDGEncoding == 2212)
		{
			proton++;
			proton_energy->Fill(edep);
			if (edep > max_proton)
				max_proton = edep;
		}

		else if (PDGEncoding == 2112)
		{
			neutron++;
			neutron_energy->Fill(edep);
			if (edep > max_neutron)
				max_neutron = edep;
		}


	i++;

	}

	Int_t digits = (2);	
	Int_t digits_energy = (4);
	
	std::cout << "-------RESULTS-------" << std::endl;
	std::cout << "Gammas number: " << gamma << " (" << std::setprecision(digits) << (gamma/float(entries)*100.) << "%)" << std::endl;
	std::cout << "Gammas max energy: " << std::setprecision(digits_energy) << max_gamma << " MeV" << std::endl;
	std::cout << "Electrons number: " << electron << " (" << std::setprecision(digits) << (electron/float(entries)*100.) << "%)"  << std::endl;
	std::cout << "Electrons max energy: " << std::setprecision(digits_energy) << max_electron << " MeV" << std::endl;
	std::cout << "Positrons number: " << positron << " (" << std::setprecision(digits) << (positron/float(entries)*100.) << "%)" << std::endl;
	std::cout << "Positrons max energy: " << std::setprecision(digits_energy) << max_positron << " MeV" << std::endl;
	std::cout << "Protons number: " << proton << " (" << std::setprecision(digits) << (proton/float(entries)*100.) << "%)" << std::endl;
	std::cout << "Protons max energy: " << std::setprecision(digits_energy) << max_proton << " MeV" << std::endl;
	std::cout << "Neutrons number: " << neutron << " (" << std::setprecision(digits) << (neutron/float(entries)*100.) << "%)" << std::endl;
	std::cout << "Neutrons max energy: " << std::setprecision(digits_energy) << max_neutron << " MeV" << std::endl;
	std::cout << "--------END--------" << std::endl;

	std::string f_name = "/home/baran/Desktop/root_hits_"+scanner_name+"_energy_spectra.root";
	TFile fw(f_name.c_str(),"RECREATE");
	TCanvas* plot = new TCanvas("Hits output","Hits output",2000,1400);
	plot->Divide(3,2);
	plot->cd(1); gamma_energy->Draw();gamma_energy->Write();
	plot->cd(2); electron_energy->Draw();electron_energy->Write();
	plot->cd(3); positron_energy->Draw();positron_energy->Write();
	plot->cd(4); proton_energy->Draw();proton_energy->Write();
	plot->cd(5); neutron_energy->Draw();neutron_energy->Write();
	plot->Update();

	TImage *img = TImage::Create();
	img->FromPad(plot);
	f_name = "/home/baran/Desktop/root_hits_"+scanner_name+"_energy_spectra.png";
	img->WriteImage(f_name.c_str());

	fw.Print();
	fw.Close();



}











void HitsRoot::rate_analysis()
{
		
	TH1F *energy_deposition = new TH1F( "Deposited energy", "Deposited energy", 5000, 0, 5.);
	energy_deposition->GetXaxis()->SetTitle("Energy [MeV]");
	energy_deposition->GetYaxis()->SetTitle("Number of particles");

	TH1F *energy_during = new TH1F( "Deposited energy during irradiation", "Deposited energy during irradiation", 500, 0, 5.);
	energy_during->GetXaxis()->SetTitle("Energy [MeV]");
	energy_during->GetYaxis()->SetTitle("Number of particles");

	TH1F *energy_after = new TH1F( "Deposited energy after irradiation", "Deposited energy after irradiation", 500, 0, 5.);
	energy_after->GetXaxis()->SetTitle("Energy [MeV]");
	energy_after->GetYaxis()->SetTitle("Number of particles");

	TH1F *time_during = new TH1F( "Time hit during irradiation", "Time hit during irradiation", 100, 0, 1);
	time_during->GetXaxis()->SetTitle("Time [ns]");
	time_during->GetYaxis()->SetTitle("Number of particles");

	TH1F *time_after = new TH1F( "Time hit after irradiation", "Time hit after irradiation", 1000, 0, 10000);
	time_after->GetXaxis()->SetTitle("Time [s]");
	time_after->GetYaxis()->SetTitle("Number of particles");

	Int_t i(0);
	Int_t nbytes(0);
	
	Int_t energy_during_count(0);
	Int_t energy_after_count(0);
	Int_t energy_during_count_10keV(0);
	Int_t energy_after_count_10keV(0);


	std::cout << "Number of entries: " << entries << std::endl;

	std::cout << "-------SETUP-------" << std::endl;
	std::cout << "During irradiation: " << energy_during_count << std::endl;
	std::cout << "After irradiation: " << energy_after_count << std::endl;
	std::cout << "During irradiation above 10keV: " << energy_during_count_10keV << std::endl;
	std::cout << "After irradiation above 10keV: " << energy_after_count_10keV << std::endl;
	std::cout << "--------END--------" << std::endl;

	while(i < entries)
	{
		if (i%1000000 == 0)
			std::cout << i/1000000 << " M events analysed." << std::endl;
	
		nbytes += HitsChain->GetEntry( i );

		energy_deposition->Fill(edep);

		if (time <= 1000000.)
		{
			energy_during_count++;
			energy_during->Fill(edep);
			time_during->Fill(time);
			if (edep>=0.01)
				energy_during_count_10keV++;
		}

		else if (time > 1000000.)
		{
			energy_after_count++;
			energy_after->Fill(edep);
			time_after->Fill(time/1000000000.);
			if (edep>=0.01)
				energy_after_count_10keV++;
		}

	i++;

	}

	Int_t digits = (2);	
	Int_t digits_energy = (4);
	
	std::cout << "-------RESULTS-------" << std::endl;
	std::cout << "During irradiation: " << energy_during_count << std::endl;
	std::cout << "After irradiation: " << energy_after_count << std::endl;
	std::cout << "During irradiation above 10keV: " << energy_during_count_10keV << std::endl;
	std::cout << "After irradiation above 10keV: " << energy_after_count_10keV << std::endl;
	std::cout << "--------END--------" << std::endl;

	std::string f_name = "/home/baran/Desktop/root_hits_"+scanner_name+"_rate_analysis.root";
	TFile fw(f_name.c_str(),"RECREATE");
	TCanvas* plot = new TCanvas("Hits output","Hits output",2000,1400);
	plot->Divide(3,2);
	plot->cd(1); energy_deposition->Draw();energy_deposition->Write();
	plot->cd(2); energy_during->Draw();energy_during->Write();
	plot->cd(3); time_during->Draw();time_during->Write();
	plot->cd(4); energy_after->Draw();energy_after->Write();
	plot->cd(5); time_after->Draw();time_after->Write();
	plot->Update();

//	TImage *img = TImage::Create();
//	img->FromPad(plot);
//	f_name = "/home/baran/Desktop/root_hits_"+scanner_name+"_rate_analysis.png";
//	img->WriteImage(f_name.c_str());

	fw.Print();
	fw.Close();



}





void HitsRoot::analysisTIMEPIX()
{

	//
	// HISTOGRAMS INITIALIZATION IF NEEDED
	//


	std::cout << "Method to implment as requested by Antoni and Paulina (HitsRoot)" << std::endl;
/*
	Int_t i(0);
	Int_t nbytes(0);

	while(i < entries)
	{
		nbytes += PhaseSpaceActor->GetEntry( i );

		// Place for yours analysis



		i++;
	}
*/



}









void HitsRoot::closeRootFile()
{
	f->Close();
}
