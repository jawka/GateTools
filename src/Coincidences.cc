#include "Coincidences.hh"


Coincidences::Coincidences(std::string dir_path, std::string scanner_mode, int analysis_mode)
{
	entries = 0;
	iterr = 0;
	barrel_flag = true;
	scanner_name = scanner_mode;
	res_mode = std::getenv("PRM_RES_MODE");
	file_name_for_number = std::getenv("PRM_COINC_FILE");
	old_file_name = dir_path + std::getenv("PRM_COINC_FILE");
	new_file_name = dir_path + "extended_sensitivity_results.root";

	rsector_barrel = 12; // in fact half of rsector
	dualhead_crystal = 12; // in fact 13; 12 due to c++ numbering
	dualhead_module = 3; // in fact one more; as it is due to c++ numbering
		
	std::string bb ("b");
	char a = res_mode.at(0);
	std::string aa;
	aa.push_back(a);


	if (bb.compare(aa) != 0)
	{
		barrel_flag = false;
		//std::cout << "Dualhead mode" << std::endl;
	}
	if (res_mode.compare("dualhead_3x4") == 0)
	{
		dualhead_module = 3;
	}
	else if (res_mode.compare("dualhead") == 0 || res_mode.compare("dualhead2") == 0 || res_mode.compare("dualhead3") == 0)
	{
		dualhead_module = 4;
	}
	else
	{
		dualhead_module = 5;
	}
	if (default_file_name.compare(old_file_name) != 0)
	{

		for (unsigned int i=0; i < file_name_for_number.size(); i++)
		{
			if (isdigit(file_name_for_number[i]))
			{
				file_no += file_name_for_number[i];
			}    
		}
		new_file_name = dir_path + "extended_sensitivity_results"+file_no+".root";
	}

	if (atoi(std::getenv("PRM_MODE")) == 12)
	{
		std::cout << "New_file_name: " << new_file_name << std::endl;
		init();
	}
}



Coincidences::~Coincidences()
{
	Reset();
}



void Coincidences::Reset()
{
	entries = 0;
	iterr = 0;
}


void Coincidences::init()
{

	f = new TFile(old_file_name.c_str(), "READ");

	if( f->IsZombie() )
	{
		std::cerr << "Cannot open the file: '" << old_file_name  << "'" << std::endl;
		exit( EXIT_FAILURE );
	}
	
	CoincidencesChain = (TTree*)f->Get("Coincidences");
	
	//
	// SET BRANCH ADDRESSES - TTREE COINCIDENCES
	//

	CoincidencesChain->SetBranchAddress("axialPos",&axialPos);
 	CoincidencesChain->SetBranchAddress("comptVolName1",&comptVolName1);
 	CoincidencesChain->SetBranchAddress("comptVolName2",&comptVolName2);
 	CoincidencesChain->SetBranchAddress("comptonPhantom1",&comptonPhantom1);
 	CoincidencesChain->SetBranchAddress("comptonPhantom2",&comptonPhantom2);
 	CoincidencesChain->SetBranchAddress("comptonCrystal1",&comptonCrystal1);
 	CoincidencesChain->SetBranchAddress("comptonCrystal2",&comptonCrystal2);
 	CoincidencesChain->SetBranchAddress("crystalID1",&crystalID1);
 	CoincidencesChain->SetBranchAddress("crystalID2",&crystalID2);
	CoincidencesChain->SetBranchAddress("gantryID1",&gantryID1);
  	CoincidencesChain->SetBranchAddress("gantryID2",&gantryID2);
 	CoincidencesChain->SetBranchAddress("energy1",&energy1);
 	CoincidencesChain->SetBranchAddress("energy2",&energy2);   
 	CoincidencesChain->SetBranchAddress("eventID1",&eventID1);
 	CoincidencesChain->SetBranchAddress("eventID2",&eventID2);
 	CoincidencesChain->SetBranchAddress("globalPosX1",&globalPosX1);
 	CoincidencesChain->SetBranchAddress("globalPosX2",&globalPosX2);
 	CoincidencesChain->SetBranchAddress("globalPosY1",&globalPosY1);
 	CoincidencesChain->SetBranchAddress("globalPosY2",&globalPosY2);      
 	CoincidencesChain->SetBranchAddress("globalPosZ1",&globalPosZ1);
 	CoincidencesChain->SetBranchAddress("globalPosZ2",&globalPosZ2);
 	CoincidencesChain->SetBranchAddress("layerID1",&layerID1);
 	CoincidencesChain->SetBranchAddress("layerID2",&layerID2);
 	CoincidencesChain->SetBranchAddress("moduleID1",&moduleID1);
 	CoincidencesChain->SetBranchAddress("moduleID2",&moduleID2);
 	CoincidencesChain->SetBranchAddress("rotationAngle",&rotationAngle);
 	CoincidencesChain->SetBranchAddress("rsectorID1",&rsectorID1);
 	CoincidencesChain->SetBranchAddress("rsectorID2",&rsectorID2);
 	CoincidencesChain->SetBranchAddress("RayleighPhantom1",&RayleighPhantom1);
 	CoincidencesChain->SetBranchAddress("RayleighPhantom2",&RayleighPhantom2);
 	CoincidencesChain->SetBranchAddress("RayleighCrystal1",&RayleighCrystal1);
 	CoincidencesChain->SetBranchAddress("RayleighCrystal2",&RayleighCrystal2);
 	CoincidencesChain->SetBranchAddress("RayleighVolName1",&RayleighVolName1);
 	CoincidencesChain->SetBranchAddress("RayleighVolName2",&RayleighVolName2);
 	CoincidencesChain->SetBranchAddress("runID",&runID);
 	CoincidencesChain->SetBranchAddress("sinogramS",&sinogramS);
 	CoincidencesChain->SetBranchAddress("sinogramTheta",&sinogramTheta);
 	CoincidencesChain->SetBranchAddress("sourceID1",&sourceID1);
 	CoincidencesChain->SetBranchAddress("sourceID2",&sourceID2);
 	CoincidencesChain->SetBranchAddress("sourcePosX1",&sourcePosX1);
 	CoincidencesChain->SetBranchAddress("sourcePosX2",&sourcePosX2);
 	CoincidencesChain->SetBranchAddress("sourcePosY1",&sourcePosY1);
 	CoincidencesChain->SetBranchAddress("sourcePosY2",&sourcePosY2);
 	CoincidencesChain->SetBranchAddress("sourcePosZ1",&sourcePosZ1);
 	CoincidencesChain->SetBranchAddress("sourcePosZ2",&sourcePosZ2);
 	CoincidencesChain->SetBranchAddress("submoduleID1",&submoduleID1);
 	CoincidencesChain->SetBranchAddress("submoduleID2",&submoduleID2);
 	CoincidencesChain->SetBranchAddress("time1",&time1);
	CoincidencesChain->SetBranchAddress("time2",&time2);

	//
	// ENTRIES NUMBER
	//

	entries = CoincidencesChain->GetEntries();
	std::cout << "Number of entries : " << entries << std::endl;
}



void Coincidences::extend_sensitivity_FOV()
{
		
	TFile *f_new = new TFile(new_file_name.c_str(), "RECREATE");

	if( f_new->IsZombie() )
	{
		std::cerr << "Cannot open the file: '" << new_file_name  << "'" << std::endl;
		exit( EXIT_FAILURE );
	}

	//CLONING THE TREE WITH ALL ENTRIES
	TTree *newtree = CoincidencesChain->CloneTree();

	Int_t nbytes(0);
	Long64_t print_test = 32368;

	for (Long64_t i=0; i<entries; i++)
	{
//		if (i%10000 == 0 && i!= 0)
//			{std::cout << i/10000 << "0k events analysed." << std::endl;}
		nbytes += CoincidencesChain->GetEntry(i);

	// ORIGINAL
	//Source Position
		osourcePosX1 = sourcePosX1;
		osourcePosY1 = sourcePosY1;
		osourcePosZ1 = sourcePosZ1;
		osourcePosX2 = sourcePosX2;
		osourcePosY2 = sourcePosY2;
		osourcePosZ2 = sourcePosZ2;
	//Global Position
		oglobalPosX1 = globalPosX1;
		oglobalPosY1 = globalPosY1;
		oglobalPosZ1 = globalPosZ1;
		oglobalPosX2 = globalPosX2;
		oglobalPosY2 = globalPosY2;
		oglobalPosZ2 = globalPosZ2;
	//rSectorID
		orsectorID1 = rsectorID1;
		orsectorID2 = rsectorID2;
	//moduleID
		omoduleID1 = moduleID1;
		omoduleID2 = moduleID2;
	//crystalID
		ocrystalID1 = crystalID1;
		ocrystalID2 = crystalID2;
	//layerID
		olayerID1 = layerID1;
		olayerID2 = layerID2;

//		if (i == print_test)
//		{
//			std::cout << "i:" << i << std::endl;
//			std::cout << "FLIP:\trs1 \trs2\tmod1\tmod2\tcr1\tcr2\tlay1\tlay2" << std::endl;
//			std::cout << "0:\t" << rsectorID1 << "\t" << rsectorID2 << "\t" << moduleID1 << "\t" << moduleID2 << "\t" << crystalID1 << "\t" << crystalID2 << "\t" << layerID1 << "\t" << layerID2 << std::endl;
//		}
				

	// FLIP X

	//Source Position
		sourcePosX1 = -osourcePosX1;
//		sourcePosY1 = 
//		sourcePosZ1 = 
		sourcePosX2 = -osourcePosX2;
//		sourcePosY2 = 
//		sourcePosZ2 = 
	//Global Position
		globalPosX1 = -oglobalPosX1;
//		globalPosY1 = 
//		globalPosZ1 = 
		globalPosX2 = -oglobalPosX2;
//		globalPosY2 = 
//		globalPosZ2 = 
	//rSectorID
		if (barrel_flag)	
		{
			if (orsectorID1 != 0)
				{rsectorID1 = 2*rsector_barrel - orsectorID1;}
			if (orsectorID2 != 0)
				{rsectorID2 = 2*rsector_barrel - orsectorID2;}
		}
//		else
//		{
//			rsectorID1 = 
//			rsectorID2 = 
//		}
	//moduleID
//		if (!barrel_flag)
//		{
//			moduleID1 = 
//			moduleID2 =
//		} 
	//crystalID
		if (barrel_flag)	
		{
			crystalID1 = rsector_barrel - ocrystalID1;
			crystalID2 = rsector_barrel - ocrystalID2;
		}
	//layerID
		if (!barrel_flag)	
		{
						
			layerID1 = 200*(olayerID1/100) + 99 - olayerID1;
			layerID2 = 200*(olayerID2/100) + 99 - olayerID2;
		}
	// WRITE FLIP X
		newtree->Fill();
//		if (i == print_test)
//		{
//			std::cout << "X:\t" << rsectorID1 << "\t" << rsectorID2 << "\t" << moduleID1 << "\t" << moduleID2 << "\t" << crystalID1 << "\t" << crystalID2 << "\t" << layerID1 << "\t" << layerID2 << std::endl;
//		}



	// FLIP Y

	//Source Position
		sourcePosX1 = osourcePosX1;
		sourcePosY1 = -osourcePosY1;
//		sourcePosZ1 = 
		sourcePosX2 = osourcePosX2;
		sourcePosY2 = -osourcePosY2;
//		sourcePosZ2 = 
	//Global Position
		globalPosX1 = oglobalPosX1;
		globalPosY1 = -oglobalPosY1;
//		globalPosZ1 = 
		globalPosX2 = oglobalPosX2;
		globalPosY2 = -oglobalPosY2;
//		globalPosZ2 = 
	//rSectorID
		if (barrel_flag)
		{
			if (orsectorID1 == 12)
				{rsectorID1 = 0;}
			else
				{rsectorID1 = 12*(orsectorID1/12) + 12 - orsectorID1 + 12 * (orsectorID1/12);}

			if (orsectorID2 == 12)
				{rsectorID2 = 0;}
			else
				{rsectorID2 = 12*(orsectorID2/12) + 12 - orsectorID2 + 12 * (orsectorID2/12);}
		}
		else
		{
			rsectorID1 = 1-orsectorID1;
			rsectorID2 = 1-orsectorID2;
		}
	//moduleID
//		if (!barrel)
//		{
//			moduleID1 = 
//			moduleID2 = 
//		}
	//crystalID
//		if (barrel)
//		{
//			crystalID1 = rsector_barrel - ocrystalID1;
//			crystalID2 = rsector_barrel - ocrystalID2;
//		}
	//layerID
		if (!barrel_flag)
		{
			layerID1 = olayerID1;
			layerID2 = olayerID2;
		}
	// WRITE FLIP Y
		newtree->Fill();
//		if (i == print_test)
//		{
//			std::cout << "Y:\t" << rsectorID1 << "\t" << rsectorID2 << "\t" << moduleID1 << "\t" << moduleID2 << "\t" << crystalID1 << "\t" << crystalID2 << "\t" << layerID1 << "\t" << layerID2 << std::endl;
//		}




	// FLIP Z

	//Source Position
//		sourcePosX1 = 
		sourcePosY1 = osourcePosY1;
		sourcePosZ1 = -osourcePosZ1;
//		sourcePosX2 = 
		sourcePosY2 = osourcePosY2;
		sourcePosZ2 = -osourcePosZ2;
	//Global Position
//		globalPosX1 = 
		globalPosY1 = oglobalPosY1;
		globalPosZ1 = -oglobalPosZ1;
//		globalPosX2 = 
		globalPosY2 = oglobalPosY2;
		globalPosZ2 = -oglobalPosZ2;
	//rSectorID
		rsectorID1 = orsectorID1;
		rsectorID2 = orsectorID2;
	//moduleID
		if (!barrel_flag)
		{
			moduleID1 = dualhead_module - omoduleID1;
			moduleID2 = dualhead_module - omoduleID2;
		}
	//crystalID
		if (!barrel_flag)
		{
			crystalID1 = dualhead_crystal - ocrystalID1;
			crystalID2 = dualhead_crystal - ocrystalID2;
		}
	//layerID
		if (barrel_flag)
		{
			layerID1 = 200*(olayerID1/100) + 99 - olayerID1;
			layerID2 = 200*(olayerID2/100) + 99 - olayerID2;
		}
	// WRITE FLIP Z
		newtree->Fill();
//		if (i == print_test)
//		{
//			std::cout << "Z:\t" << rsectorID1 << "\t" << rsectorID2 << "\t" << moduleID1 << "\t" << moduleID2 << "\t" << crystalID1 << "\t" << crystalID2 << "\t" << layerID1 << "\t" << layerID2 << std::endl;
//		}




	// FLIP XY

	//Source Position
		sourcePosX1 = -osourcePosX1;
		sourcePosY1 = -osourcePosY1;
		sourcePosZ1 = osourcePosZ1;
		sourcePosX2 = -osourcePosX2;
		sourcePosY2 = -osourcePosY2;
		sourcePosZ2 = osourcePosZ2;
	//Global Position
		globalPosX1 = -oglobalPosX1;
		globalPosY1 = -oglobalPosY1;
		globalPosZ1 = oglobalPosZ1;
		globalPosX2 = -oglobalPosX2;
		globalPosY2 = -oglobalPosY2;
		globalPosZ2 = oglobalPosZ2;
	//rSectorID
		if (barrel_flag)
		{
			if (orsectorID1 < 12)
				{rsectorID1 = orsectorID1+12;}
			else
				{rsectorID1 = orsectorID1-12;}

			if (orsectorID2 < 12)
				{rsectorID2 = orsectorID2+12;}
			else
				{rsectorID2 = orsectorID2-12;}
		}
		else
		{
			rsectorID1 = 1-orsectorID1;
			rsectorID2 = 1-orsectorID2;
		}
	//moduleID
		if (!barrel_flag)
		{
			moduleID1 = omoduleID1;
			moduleID2 = omoduleID2;
		}
	//crystalID
		crystalID1 = ocrystalID1;
		crystalID2 = ocrystalID2;
	//layerID
		if (barrel_flag)
		{
			layerID1 = olayerID1;
			layerID2 = olayerID2;
		}
		if (!barrel_flag)
		{
			layerID1 = 200*(olayerID1/100) + 99 - olayerID1;
			layerID2 = 200*(olayerID2/100) + 99 - olayerID2;
		}
	// WRITE FLIP XY
		newtree->Fill();
//		if (i == print_test)
//		{
//			std::cout << "XY:\t" << rsectorID1 << "\t" << rsectorID2 << "\t" << moduleID1 << "\t" << moduleID2 << "\t" << crystalID1 << "\t" << crystalID2 << "\t" << layerID1 << "\t" << layerID2 << std::endl;
//		}




	// FLIP XZ

	//Source Position
//		sourcePosX1 = 
		sourcePosY1 = osourcePosY1;
		sourcePosZ1 = -osourcePosZ1;
//		sourcePosX2 = 
		sourcePosY2 = osourcePosY2;
		sourcePosZ2 = -osourcePosZ2;
	//Global Position
//		globalPosX1 = 
		globalPosY1 = oglobalPosY1;
		globalPosZ1 = -oglobalPosZ1;
//		globalPosX2 = 
		globalPosY2 = oglobalPosY2;
		globalPosZ2 = -oglobalPosZ2;
	//rSectorID
		if (barrel_flag)
		{	
			if (orsectorID1 != 0)
				{rsectorID1 = 2*rsector_barrel - orsectorID1;}
			if (orsectorID2 != 0)
				{rsectorID2 = 2*rsector_barrel - orsectorID2;}
		}
		else
		{
			rsectorID1 = orsectorID1;
			rsectorID2 = orsectorID2;
		}
	//moduleID
		if (!barrel_flag)
		{
			moduleID1 = dualhead_module - omoduleID1;
			moduleID2 = dualhead_module - omoduleID2;
		}
	//crystalID
		if (barrel_flag)
		{
			crystalID1 = rsector_barrel - ocrystalID1;
			crystalID2 = rsector_barrel - ocrystalID2;
		}
		else
		{
			crystalID1 = dualhead_crystal - ocrystalID1;
			crystalID2 = dualhead_crystal - ocrystalID2;
		}
	//layerID
		if (barrel_flag)
		{
			layerID1 = 200*(olayerID1/100) + 99 - olayerID1;
			layerID2 = 200*(olayerID2/100) + 99 - olayerID2;
		}
	// WRITE FLIP XZ
		newtree->Fill();
//		if (i == print_test)
//		{
//			std::cout << "XZ:\t" << rsectorID1 << "\t" << rsectorID2 << "\t" << moduleID1 << "\t" << moduleID2 << "\t" << crystalID1 << "\t" << crystalID2 << "\t" << layerID1 << "\t" << layerID2 << std::endl;
//		}



	// FLIP YZ

	//Source Position
		sourcePosX1 = osourcePosX1;
		sourcePosY1 = -osourcePosY1;
//		sourcePosZ1 = 
		sourcePosX2 = osourcePosX2;
		sourcePosY2 = -osourcePosY2;
//		sourcePosZ2 = 
	//Global Position
		globalPosX1 = oglobalPosX1;
		globalPosY1 = -oglobalPosY1;
//		globalPosZ1 = 
		globalPosX2 = oglobalPosX2;
		globalPosY2 = -oglobalPosY2;
//		globalPosZ2 = 
	//rSectorID
		if (barrel_flag)
		{
			if (orsectorID1 == 12)
				{rsectorID1 = 0;}
			else
				{rsectorID1 = 12*(orsectorID1/12) + 12 - orsectorID1 + 12 * (orsectorID1/12);}

			if (orsectorID2 == 12)
				{rsectorID2 = 0;}
			else
				{rsectorID2 = 12*(orsectorID2/12) + 12 - orsectorID2 + 12 * (orsectorID2/12);}
		}
		else
		{
			rsectorID1 = 1-orsectorID1;
			rsectorID2 = 1-orsectorID2;
		}
	//moduleID
//		if (!barrel_flag)
//		{
//			moduleID1 = 
//			moduleID2 = 
//		}
	//crystalID
//		if (!barrel_flag)
//		{
//			crystalID1 = 
//			crystalID2 = 
//		}
	//layerID
		if (!barrel_flag)
		{
			layerID1 = olayerID1;
			layerID2 = olayerID2;
		}
	// WRITE FLIP YZ
		newtree->Fill();
//		if (i == print_test)
//		{
//			std::cout << "YZ:\t" << rsectorID1 << "\t" << rsectorID2 << "\t" << moduleID1 << "\t" << moduleID2 << "\t" << crystalID1 << "\t" << crystalID2 << "\t" << layerID1 << "\t" << layerID2 << std::endl;
//		}




	// FLIP XYZ

	//Source Position
		sourcePosX1 = -osourcePosX1;
//		sourcePosY1 = 
//		sourcePosZ1 = 
		sourcePosX2 = -osourcePosX2;
//		sourcePosY2 = 
//		sourcePosZ2 = 
	//Global Position
		globalPosX1 = -oglobalPosX1;
//		globalPosY1 = 
//		globalPosZ1 = 
		globalPosX2 = -oglobalPosX2;
//		globalPosY2 = 
//		globalPosZ2 = 
	//rSectorID
		if (barrel_flag)
		{
			if (orsectorID1 < 12)
				{rsectorID1 = orsectorID1+12;}
			else
				{rsectorID1 = orsectorID1-12;}

			if (orsectorID2 < 12)
				{rsectorID2 = orsectorID2+12;}
			else
				{rsectorID2 = orsectorID2-12;}
		}
//		else
//		{
//			rsectorID1 = 1-orsectorID1;
//			rsectorID2 = 1-orsectorID2;
//		}
	//moduleID
//		if (!barrel_flag)
//		{
//			moduleID1 = 
//			moduleID2 = 
//		}
	//crystalID
		if (barrel_flag)
		{
			crystalID1 = ocrystalID1;
			crystalID2 = ocrystalID2;
		}
	//layerID
		if (!barrel_flag)
		{
			layerID1 = 200*(olayerID1/100) + 99 - olayerID1;
			layerID2 = 200*(olayerID2/100) + 99 - olayerID2;
		}
	// WRITE FLIP XYZ
		newtree->Fill();
//		if (i == print_test)
//		{
//			std::cout << "XYZ:\t" << rsectorID1 << "\t" << rsectorID2 << "\t" << moduleID1 << "\t" << moduleID2 << "\t" << crystalID1 << "\t" << crystalID2 << "\t" << layerID1 << "\t" << layerID2 << std::endl;
//		}











	}

	newtree->AutoSave();
	f_new->Write();
	f_new->Close();

}










void Coincidences::closeRootFile()
{
	f->Close();
}
