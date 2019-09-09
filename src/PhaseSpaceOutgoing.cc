#include "PhaseSpaceOutgoing.hh"


PhaseSpaceOutgoing::PhaseSpaceOutgoing(std::string pso_string, std::string scanner_mode)
{

	temp_event = -1;
	annihilations = 0;
	positron_no = 0;
	gammas_no = 0;
	annihil_gammas_no = 0;
	file_name = pso_string + file_name_extension;
	scanner_name = scanner_mode;
	if (atoi(std::getenv("PRM_MODE")) != 10 && atoi(std::getenv("PRM_MODE")) != 12 && atoi(std::getenv("PRM_MODE")) != 13)
		init();

}



PhaseSpaceOutgoing::~PhaseSpaceOutgoing()
{
	temp_event = -1;
	annihilations = 0;
	positron_no = 0;
}









// Main analysis function
void PhaseSpaceOutgoing::init()
{

	f = new TFile(file_name.c_str(), "READ");

	if( f->IsZombie() )
	{
		std::cerr << "Cannot open the file '" << file_name  << "'" << std::endl;
		exit( EXIT_FAILURE );
	}
	
	PhaseSpaceActor = (TTree*)gDirectory->Get("PhaseSpace");

	//
	// SET BRANCH ADDRESSES - TTREE PhaseSpace
	//

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
//	PhaseSpaceActor->SetBranchAddress("PDGCode",&PDGCode);
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


}
	




	// ANALYSIS OF THE PROMPT GAMMAS
void PhaseSpaceOutgoing::analysisPromptGammas()
{
	//
	// HISTOGRAMS INITIALIZATION
	//
	TH2F *xy = new TH2F( "xy", "XY histogram", 100, -50.0, 50.0, 100, -50.0, 50.0);
	xy->GetXaxis()->SetTitle("X [mm]");
	xy->GetYaxis()->SetTitle("Y [mm]");

	TH1F *z = new TH1F( "z", "Z range", atoi(std::getenv("PRM_PH_BINS")), atof(std::getenv("PRM_PH_MIN")), atof(std::getenv("PRM_PH_MAX")));
	z->GetXaxis()->SetTitle("Z [mm]");
	z->GetYaxis()->SetTitle("Number of particles");

	//
	// MAIN ANALYSIS (LOOP)
	//			
	TH1F *prod_time_curve = new TH1F( "prod_time", "Prompt gamma leaving phantom time", 1200, 0, 12);
	prod_time_curve->GetXaxis()->SetTitle("Time [ns] - 0.01 nanoseconds intervals");
	prod_time_curve->GetYaxis()->SetTitle("Number of prompt gammas");

	TH1F *gamma_energy = new TH1F( "Gamma energy", "Gamma energy", 5000, 0, 10);
	gamma_energy->GetXaxis()->SetTitle("Energy [MeV]");
	gamma_energy->GetYaxis()->SetTitle("Number of prompt gammas");	
	
	Int_t i(0);
	Int_t nbytes(0);

	while(i < entries)
	{
		if (i%10000000 == 0)
			std::cout << i/10000000 << " E+07 events analysed." << std::endl;

		nbytes += PhaseSpaceActor->GetEntry( i );

		//GAMMAS
		if (gamma.compare(ParticleName) == 0)
//		if (PDGCode == 22)	
		{			
			if (Ekine < 10. and Ekine > 1. and protonInelastic.compare(CreatorProcess) != 0)
			{
				gammas_no++;
				prod_time_curve->Fill(TProd+TOut);
				gamma_energy->Fill(Ekine);	
			}
		}
		i++;
	}

	std::string f_name = "/home/baran/Desktop/root_phaseSpaceOutgoing_"+scanner_name+"_promptGammas.root";
	TFile fw(f_name.c_str(),"RECREATE");

	std::cout << "All outgoing prompt gammas no: " << gammas_no << std::endl;

	gStyle->SetPalette();

	TCanvas* plot = new TCanvas("PhaseSpaceOutgoingActor","PhaseSpaceOutgoingActor",2000,1400);
	plot->Divide(1,2);
	plot->cd(1); prod_time_curve->Draw();prod_time_curve->Write();
	plot->cd(2); gamma_energy->Draw();gamma_energy->Write();
	gPad->SetLogy();
	plot->Update();

	TImage *img = TImage::Create();
	img->FromPad(plot);
	f_name = "/home/baran/Desktop/root_phaseSpaceOutgoing_"+scanner_name+"_promptGammas.png";
	img->WriteImage(f_name.c_str());

	fw.Print();
	fw.Close();

}






	// ANALYSIS OF THE VHEE
void PhaseSpaceOutgoing::analysisVHEEGammas()
{
	
	//
	// MAIN ANALYSIS (LOOP)
	//
			
	TH1F *gamma_energy = new TH1F( "Gamma energy", "Gamma energy", 200, 0, 5);
	gamma_energy->GetXaxis()->SetTitle("Energy [MeV]");
	gamma_energy->GetYaxis()->SetTitle("Number of gammas");

	TH2F *z_time = new TH2F( "Z position versus production time", "Z position versus production time", 1000, 0, 10, atoi(std::getenv("PRM_PH_BINS")), atof(std::getenv("PRM_PH_MIN")), atof(std::getenv("PRM_PH_MAX")));
	z_time->GetXaxis()->SetTitle("Production Time [ns]");
	z_time->GetYaxis()->SetTitle("Z position [mm]");

	TH1F *prod_time_curve = new TH1F( "prod_time", "Positron production time", 1000, 0, 10);
	prod_time_curve->GetXaxis()->SetTitle("Time [ns] - 0.01 nanoseconds intervals");
	prod_time_curve->GetYaxis()->SetTitle("Number of annihilation gammas");
	
	TH1F *time_curve = new TH1F( "Global", "Global outgoing global time", 1000, 0, 10);
	time_curve->GetXaxis()->SetTitle("Time [ns] - 0.01 nanoseconds intervals");
	time_curve->GetYaxis()->SetTitle("Number of annihilation gammas");

	Int_t i(0);
	Int_t nbytes(0);

	while(i < entries)
	{
		if (i%10000000 == 0)
			std::cout << i/10000000 << " E+07 events analysed." << std::endl;

		nbytes += PhaseSpaceActor->GetEntry( i );

		//GAMMAS
		if (gamma.compare(ParticleName) == 0)
//		if (PDGCode == 22)	
		{			
			if (annihil_process.compare(CreatorProcess) == 0)
			{
				gammas_no++;
				prod_time_curve->Fill(TProd);
				gamma_energy->Fill(Ekine);	
				z_time->Fill(Time, EmissionPointZ);
				prod_time_curve->Fill(Time);
				
			}
		}
		i++;
	}

	std::string f_name = "/home/baran/Desktop/root_phaseSpaceOutgoing_"+scanner_name+"_VHEE_annihilGammas.root";
	TFile fw(f_name.c_str(),"RECREATE");

	std::cout << "All outgoing prompt gammas no: " << gammas_no << std::endl;

	gStyle->SetPalette();

	TCanvas* plot = new TCanvas("PhaseSpaceOutgoingActor","PhaseSpaceOutgoingActor",2000,1400);
	plot->Divide(2,2);
	plot->cd(1); prod_time_curve->Draw();prod_time_curve->Write();
	plot->cd(2); gamma_energy->Draw();gamma_energy->Write();
	gPad->SetLogy();
	plot->cd(3); gamma_energy->Draw();gamma_energy->Write();
	plot->cd(4); z_time->Draw("COLZ1");z_time->Write();
	plot->Update();

	TImage *img = TImage::Create();
	img->FromPad(plot);
	f_name = "/home/baran/Desktop/root_phaseSpaceOutgoing_"+scanner_name+"_VHEE_annihilGammas.png";
	img->WriteImage(f_name.c_str());

	fw.Print();
	fw.Close();

}




// TODO: Perform the full analysis of the outgoing particles
void PhaseSpaceOutgoing::complexAnalysisVHEEGammas()
{
	// MAIN ANALYSIS (LOOP)
			
	TH1F *gamma_energy = new TH1F( "Gamma energy", "Gamma energy", 200, 0, 20);
	gamma_energy->GetXaxis()->SetTitle("Energy [MeV]");
	gamma_energy->GetYaxis()->SetTitle("Number of gammas");

	TH1F *annihil_gamma_energy = new TH1F( "Anniliation gamma energy", "Anniliation gamma energy", 200, 0, 20);
	annihil_gamma_energy->GetXaxis()->SetTitle("Energy [MeV]");
	annihil_gamma_energy->GetYaxis()->SetTitle("Number of annihilation gammas");

	TH1F *electron_energy = new TH1F( "Electron energy", "Electron energy", 400, 0, 200);
	electron_energy->GetXaxis()->SetTitle("Energy [MeV]");
	electron_energy->GetYaxis()->SetTitle("Number of electrons (e-)");

	TH1F *positron_energy = new TH1F( "Positron energy", "Positron energy", 400, 0, 200);
	positron_energy->GetXaxis()->SetTitle("Energy [MeV]");
	positron_energy->GetYaxis()->SetTitle("Number of positrons (e+)");

	TH1F *neutron_energy = new TH1F( "Neutron energy", "Neutron energy", 400, 0, 200);
	neutron_energy->GetXaxis()->SetTitle("Energy [MeV]");
	neutron_energy->GetYaxis()->SetTitle("Number of neutrons (n)");

	Int_t i(0);
	Int_t nbytes(0);
	
	Int_t gammas(0);
	Int_t annihil_gammas(0);
	Int_t electrons(0);
	Int_t positrons(0);
	Int_t neutrons(0);
	Int_t rest(0);

	std::cout << "-------SETUP-------" << std::endl;
	std::cout << "Gammas number: " << gammas << std::endl;
	std::cout << "Annihilation gammas number: " << annihil_gammas << std::endl;
	std::cout << "Electrons number: " << electrons << std::endl;
	std::cout << "Positrons number: " << positrons << std::endl;
	std::cout << "Neutrons number: " << neutrons << std::endl;
	std::cout << "The rest: " << rest << std::endl;
	std::cout << "--------END--------" << std::endl;

	Float_t phantom_boundary_min (atof(std::getenv("PRM_PH_MIN")));
	Float_t phantom_boundary_max (atof(std::getenv("PRM_PH_MAX")));

	while(i < entries)
	{
		if (i%10000000 == 0)
			std::cout << i/10000000 << " E+07 events analysed." << std::endl;
	
		nbytes += PhaseSpaceActor->GetEntry( i );

		//GAMMAS
		if (gamma.compare(ParticleName) == 0 and Z < phantom_boundary_max and Z > phantom_boundary_min)
		{			
			gamma_energy->Fill(Ekine);
			gammas++;
			//ANNIHILATION GAMMAS
			if (annihil_process.compare(CreatorProcess) == 0)
			{
				annihil_gamma_energy->Fill(Ekine);
				annihil_gammas++;
			}
		}
		//ELECTRONS
		else if(electron.compare(ParticleName) == 0 and Z < phantom_boundary_max and Z > phantom_boundary_min)
		{
			electron_energy->Fill(Ekine);
			electrons++;
		}
		//POSITRONS
		else if(positron.compare(ParticleName) == 0 and Z < phantom_boundary_max and Z > phantom_boundary_min)
		{
			positron_energy->Fill(Ekine);
			positrons++;
		}
		//NEUTRONS
		else if(neutron.compare(ParticleName) == 0 and Z < phantom_boundary_max and Z > phantom_boundary_min)
		{
			neutron_energy->Fill(Ekine);
			neutrons++;
		}
		//THE REST
		else if(Z < phantom_boundary_max and Z > phantom_boundary_min)
		{
			rest++;
		}
		i++;
	}

	entries = rest + neutrons + positrons + electrons + gammas;

	std::cout << "------RESULTS------" << std::endl;
	std::cout << "Gammas number: " << gammas << " - " << (float) 100*(float)gammas/(float)entries << "%" << " of all outgoing particles" << std::endl;
	std::cout << "Annihilation gammas number: " << annihil_gammas << " - " << (float) 100*(float)annihil_gammas/(float)gammas << "%" << " of all outgoing gammas" << std::endl;
	std::cout << "Electrons number: " << electrons << " - " << (float) 100*(float)electrons/(float)entries << "%" << " of all outgoing particles" << std::endl;
	std::cout << "Positrons number: " << positrons << " - " << (float) 100*(float)positrons/(float)entries << "%" << " of all outgoing particles" << std::endl;
	std::cout << "Neutrons number: " << neutrons << " - " << (float) 100*(float)neutrons/(float)entries << "%" << " of all outgoing particles" << std::endl;
	std::cout << "The rest: " << rest << " - " << (float) 100*(float)rest/(float)entries << "%" << " of all outgoing particles" << std::endl;
	std::cout << "--------END--------" << std::endl;

	std::string f_name = "/home/baran/Desktop/root_phaseSpaceOutgoing_"+scanner_name+"_VHEE_complex.root";
	TFile fw(f_name.c_str(),"RECREATE");

	gStyle->SetPalette();

	TCanvas* plot = new TCanvas("PhaseSpaceOutgoingActor","PhaseSpaceOutgoingActor",2000,1400);
	plot->Divide(3,2);
	plot->cd(1); gamma_energy->Draw();gamma_energy->Write();
	plot->cd(2); annihil_gamma_energy->Draw();annihil_gamma_energy->Write();
	plot->cd(3); electron_energy->Draw();electron_energy->Write();
	plot->cd(4); positron_energy->Draw();positron_energy->Write();
	plot->cd(5); neutron_energy->Draw();neutron_energy->Write();
	plot->Update();

	TImage *img = TImage::Create();
	img->FromPad(plot);
	f_name = "/home/baran/Desktop/root_phaseSpaceOutgoing_"+scanner_name+"_VHEE_complex.png";
	img->WriteImage(f_name.c_str());

	fw.Print();
	fw.Close();

}







void PhaseSpaceOutgoing::gammasTimeAnalysisVHEE()
{
	
	//
	// MAIN ANALYSIS (LOOP)
	//
			
	TH1F *gamma_prod_time = new TH1F( "Gammas production time from the incident electron production", "Gammas production time from the incident electron production", 7000, 3, 10);
	gamma_prod_time->GetXaxis()->SetTitle("Time [ns]");
	gamma_prod_time->GetYaxis()->SetTitle("Number of gammas");

	TH1F *annihil_gamma_prod_time = new TH1F( "Annihilation gammas production time from the electron production moment", "Annihilation gammas production time from the production moment", 7000, 3, 10);
	annihil_gamma_prod_time->GetXaxis()->SetTitle("Time [ns]");
	annihil_gamma_prod_time->GetYaxis()->SetTitle("Number of annihilation gammas");

	TH1F *annihil_gamma_middle_prod_time = new TH1F( "Annihilation gammas (from the middle of the phantom) production time from the electron production moment", "Annihilation gammas production time from the production moment", 7000, 3, 10);
	annihil_gamma_middle_prod_time->GetXaxis()->SetTitle("Time [ns]");
	annihil_gamma_middle_prod_time->GetYaxis()->SetTitle("Number of annihilation gammas");

	TH1F *gamma_out_time = new TH1F( "Gammas phantom leaving time from the incident electron production", "Gammas phantom leaving time from the incident electron production", 5000, 1, 6);
	gamma_out_time->GetXaxis()->SetTitle("Time [ns]");
	gamma_out_time->GetYaxis()->SetTitle("Number of gammas");

	TH1F *annihil_gamma_out_time = new TH1F( "Annihilation gammas phantom leaving time from the production moment", "Annihilation gammas phantom leaving time from the production moment", 5000, 1, 6);
	annihil_gamma_out_time->GetXaxis()->SetTitle("Time [ns]");
	annihil_gamma_out_time->GetYaxis()->SetTitle("Number of annihilation gammas");

	TH1F *annihil_gamma_middle_out_time = new TH1F( "Annihilation gammas phantom (from the middle of the phantom) leaving time from the production moment", "Annihilation gammas phantom leaving time from the production moment", 5000, 1, 6);
	annihil_gamma_middle_out_time->GetXaxis()->SetTitle("Time [ns]");
	annihil_gamma_middle_out_time->GetYaxis()->SetTitle("Number of annihilation gammas");

	Int_t i(0);
	Int_t nbytes(0);

	while(i < entries)
	{
		nbytes += PhaseSpaceActor->GetEntry( i );

		if (i%10000000 == 0)
			std::cout << i/10000000 << " E+07 events analysed." << std::endl;

		if (gamma.compare(ParticleName) == 0)				
		{
			gamma_prod_time->Fill(TProd+TOut);
			gamma_out_time->Fill(TOut);	
			gammas_no++;

			if (annihil_process.compare(CreatorProcess) == 0)
			{
				annihil_gamma_prod_time->Fill(TProd+TOut);
				annihil_gamma_out_time->Fill(TOut);
				annihil_gammas_no++;

				if (EmissionPointX <= 2. and EmissionPointY <= 2.)
				{
					annihil_gamma_middle_prod_time->Fill(TProd+TOut);
					annihil_gamma_middle_out_time->Fill(TOut);
					annihil_gammas_middle_no++;
				}
			}
		}
		i++;
	}

	std::string f_name = "/home/baran/Desktop/root_phaseSpaceOutgoing_"+scanner_name+"_VHEE_gammasTimeAnalysis.root";
	TFile fw(f_name.c_str(),"RECREATE");

	std::cout << "All outgoing gammas no: " << gammas_no << std::endl;
	std::cout << "All outgoing annihilation gammas no: " << annihil_gammas_no << std::endl;
	std::cout << "All outgoing annihilation gammas from the middle of the phantom no: " << annihil_gammas_middle_no << std::endl;

	gStyle->SetPalette();

	TCanvas* plot = new TCanvas("PhaseSpaceOutgoingActor","PhaseSpaceOutgoingActor",2000,1400);
	plot->Divide(2,3);
	plot->cd(1); gamma_prod_time->Draw();gamma_prod_time->Write();
	plot->cd(2); gamma_out_time->Draw();gamma_out_time->Write();
	plot->cd(3); annihil_gamma_prod_time->Draw();annihil_gamma_prod_time->Write();
	plot->cd(4); annihil_gamma_out_time->Draw();annihil_gamma_out_time->Write();
	plot->cd(5); annihil_gamma_middle_prod_time->Draw();annihil_gamma_middle_prod_time->Write();
	plot->cd(6); annihil_gamma_middle_out_time->Draw();annihil_gamma_middle_out_time->Write();
	plot->Update();

	TImage *img = TImage::Create();
	img->FromPad(plot);
	f_name = "/home/baran/Desktop/root_phaseSpaceOutgoing_"+scanner_name+"_VHEE_gammasTimeAnalysis.png";
	img->WriteImage(f_name.c_str());

	fw.Print();
	fw.Close();

}










void PhaseSpaceOutgoing::outgoing_spectra_energy_analysis()
{

	//
	// MAIN ANALYSIS (LOOP)
	//
			
	TH1F *gamma_energy = new TH1F( "Gamma energy", "Gamma energy", 4000, 0, 40);
	gamma_energy->GetXaxis()->SetTitle("Energy [MeV]");
	gamma_energy->GetYaxis()->SetTitle("Number of gammas");

	TH1F *electron_energy = new TH1F( "Electron energy", "Electron energy", 2000, 0, 20);
	electron_energy->GetXaxis()->SetTitle("Energy [MeV]");
	electron_energy->GetYaxis()->SetTitle("Number of electrons (e-)");

	TH1F *positron_energy = new TH1F( "Positron energy", "Positron energy", 2000, 0, 20);
	positron_energy->GetXaxis()->SetTitle("Energy [MeV]");
	positron_energy->GetYaxis()->SetTitle("Number of positrons (e+)");

	TH1F *proton_energy = new TH1F( "Proton energy", "Proton energy", 1500, 0, 150);
	proton_energy->GetXaxis()->SetTitle("Energy [MeV]");
	proton_energy->GetYaxis()->SetTitle("Number of protons (p+)");

	TH1F *neutron_energy = new TH1F( "Neutron energy", "Neutron energy", 1500, 0, 150);
	neutron_energy->GetXaxis()->SetTitle("Energy [MeV]");
	neutron_energy->GetYaxis()->SetTitle("Number of neutrons (n)");

	Int_t i(0);
	Int_t nbytes(0);
	
	Int_t n_gamma(0);
	Int_t n_electron(0);
	Int_t n_proton(0);
	Int_t n_neutron(0);
	Int_t n_positron(0);

	Float_t max_gamma(0);
	Float_t max_electron(0);
	Float_t max_proton(0);
	Float_t max_neutron(0);
	Float_t max_positron(0);

	std::cout << "Number of entries: " << entries << std::endl;

	std::cout << "-------SETUP-------" << std::endl;
	std::cout << "Gammas number: " << n_gamma << std::endl;
	std::cout << "Electrons number: " << n_electron << std::endl;
	std::cout << "Positrons number: " << n_positron << std::endl;
	std::cout << "Protons number: " << n_proton << std::endl;
	std::cout << "Neutrons number: " << n_neutron << std::endl;
	std::cout << "--------END--------" << std::endl;

	while(i < entries)
	{
		if (i%10000000 == 0)
			std::cout << i/10000000 << " E+07 events analysed." << std::endl;
	
		nbytes += PhaseSpaceActor->GetEntry( i );

		if (gamma.compare(ParticleName) == 0 && TrackID != 1)
		{
			n_gamma++;
			gamma_energy->Fill(Ekine);
			if (Ekine > max_gamma)
				max_gamma = Ekine;
		}
		else if (electron.compare(ParticleName) == 0 && TrackID != 1)
		{
			n_electron++;
			electron_energy->Fill(Ekine);
			if (Ekine > max_electron)
				max_electron = Ekine;
		}
		else if (positron.compare(ParticleName) == 0 && TrackID != 1)
		{
			n_positron++;
			positron_energy->Fill(Ekine);
			if (Ekine > max_positron)
				max_positron = Ekine;
		}
		else if (proton.compare(ParticleName) == 0 && TrackID != 1)
		{
			n_proton++;
			proton_energy->Fill(Ekine);
			if (Ekine > max_proton)
				max_proton = Ekine;
		}
		else if (neutron.compare(ParticleName) == 0 && TrackID != 1)
		{
			n_neutron++;
			neutron_energy->Fill(Ekine);
			if (Ekine > max_neutron)
				max_neutron = Ekine;
		}
	i++;
	}

	Int_t digits = (2);	
	Int_t digits_energy = (4);
	
	std::cout << "-------RESULTS-------" << std::endl;
	std::cout << "Gammas number: " << n_gamma << " (" << std::setprecision(digits) << (n_gamma/float(entries)*100.) << "%)" << std::endl;
	std::cout << "Gammas max energy: " << std::setprecision(digits_energy) << max_gamma << " MeV" << std::endl;
	std::cout << "Electrons number: " << n_electron << " (" << std::setprecision(digits) << (n_electron/float(entries)*100.) << "%)"  << std::endl;
	std::cout << "Electrons max energy: " << std::setprecision(digits_energy) << max_electron << " MeV" << std::endl;
	std::cout << "Positrons number: " << n_positron << " (" << std::setprecision(digits) << (n_positron/float(entries)*100.) << "%)" << std::endl;
	std::cout << "Positrons max energy: " << std::setprecision(digits_energy) << max_positron << " MeV" << std::endl;
	std::cout << "Protons number: " << n_proton << " (" << std::setprecision(digits) << (n_proton/float(entries)*100.) << "%)" << std::endl;
	std::cout << "Protons max energy: " << std::setprecision(digits_energy) << max_proton << " MeV" << std::endl;
	std::cout << "Neutrons number: " << n_neutron << " (" << std::setprecision(digits) << (n_neutron/float(entries)*100.) << "%)" << std::endl;
	std::cout << "Neutrons max energy: " << std::setprecision(digits_energy) << max_neutron << " MeV" << std::endl;
	std::cout << "--------END--------" << std::endl;

	std::string f_name = "/home/baran/Desktop/root_phaseSpaceOutgoing_"+scanner_name+"_energy_spectra.root";
	TFile fw(f_name.c_str(),"RECREATE");\

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
	f_name = "/home/baran/Desktop/root_phaseSpaceOutgoing_"+scanner_name+"_energy_spectra.png";
	img->WriteImage(f_name.c_str());

	fw.Print();
	fw.Close();

}









void PhaseSpaceOutgoing::closeRootFile()
{
	f->Close();
}
