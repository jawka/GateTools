// C++ Libraries
#include <cstdlib>
#include <map>
#include <boost/program_options.hpp>
#include <sstream>
#include <string>
#include <cstring>
#include <stdlib.h>

// Libraries
#include "PhaseSpace.hh"
#include "PhaseSpaceOutgoing.hh"
#include "HitsRoot.hh"
#include "Dose.hh"
#include "Plot.hh"

#define SET_PRM_ENV_VAR(option, variable, default_value) { \
	if (vm.count(option)) { \
		std::stringstream ss; \
		ss << vm[option].as<std::string>(); \
		setenv(variable, ss.str().c_str(), 1); \
	} \
	else { \
		setenv(variable, default_value, 1); \
	} \
	}


namespace po = boost::program_options;


int main(int argc, char *argv[])
{



	std::stringstream modes_desciption;
	modes_desciption << "Available analysis modes (default = 9): " << std::endl;
	modes_desciption << "No.  Description" << std::endl;
	modes_desciption << "1    Annihilation gammas analysis within plastic to establish system efficiency." << std::endl;
	modes_desciption << "2    Prompt gammas analysis based on phase space actor." << std::endl;
	modes_desciption << "3    #TODO Prompt gammas analysis based on phase space outgoing actor and root output." << std::endl;
	modes_desciption << "4    Time analysis of the produced generic ions and annihilations." << std::endl;
	modes_desciption << "5    VHEE electrons analysis." << std::endl;
	modes_desciption << "6    Outgoing particles spectra analysis." << std::endl;
	modes_desciption << "7    Energy deposition within plastic spectrum analysis." << std::endl;
	modes_desciption << "8    #TODO Kinetic energy vs deposited energy analysis." << std::endl;
	modes_desciption << "9    Full analysis." << std::endl;
	modes_desciption << "10   Original and reconstructed data plots." << std::endl;


	po::options_description desc("Help to the prm(proton range monitoring) program\n\nArguments");

	desc.add_options()
	("help", "help message")
	("dir_path", po::value<std::string>(), "Path to the directory where the simulation results are stored. MANDATORY")
	("scanner_name", po::value<std::string>(), "Name of the scanner, the result files will start from this expression and will be stored in Desktop (default: test_scanner_name)")
	("ps_files_no", po::value<std::string>(), "Number of the phaseSpace secondaries actors (default: 1)")
	("mode", po::value<std::string>(), modes_desciption.str().c_str())
	("e_min", po::value<std::string>(), "Fixed minimum energy threshold [MeV] (default: 0.15 MeV); For a photoelectric absorbtion use 0.35")
	("e_max", po::value<std::string>(), "Fixed maximum energy threshold [MeV] (default: 0.4 MeV); For a photoelectric absorbtion use 0.85")
	("edep_process", po::value<std::string>(), "Process causing (default: compt). For photoelectric absorbtion use phot")
	("phantom_min", po::value<std::string>(), "Beginning Z(float) position of the phantom in the world coordinates [mm] (default: -200.).")
	("phantom_max", po::value<std::string>(), "End Z(float) position of the phantom  in the world coordinates [mm] (default: 200.).")
	("phantom_bins_no", po::value<std::string>(), "Number of bins in results histogram corresponding to the phantom binning (default: 200)")
	("res_blur_mode", po::value<std::string>(), "Resolurion blurring depends from the mode. There is four different modes: barrel, dualhead, lso, random (default: barrel)")
	("res_blur_x", po::value<std::string>(), "Works only with the random blurring_mode, otwerwise default value is set. Resolution blurring in x direction (sigma) [mm] (default for random blurring_mode: 3.0)")
	("res_blur_y", po::value<std::string>(), "Works only with the random blurring_mode, otwerwise default value is set. Resolution blurring in y direction (sigma) [mm] (default for random blurring_mode: 3.0)")
	("res_blur_z", po::value<std::string>(), "Works only with the random blurring_mode, otwerwise default value is set. Resolution blurring in z direction (sigma) [mm] (default for random blurring_mode: 3.0)")
	("res_blur_t", po::value<std::string>(), "Works only with the random blurring_mode, otwerwise default value is set. Time resolution blurring (sigma) [ns] (default for random blurring_mode: 0.1)")
	("beta_range", po::value<std::string>(), "Approximate range of the beta+ activity in mm wrt world coordinates [mm] (default for 10^8 primary protons: 30.). For 10^8 primary protons -80. mm should be used due to phantom trasnaltion wrt the isocenter.")

	("y_surface", po::value<std::string>(), "y surface coordinate parallel to the head surfaces, which indicate y coordinate where the annihilation occured. Works only for res_blur_mode == dualhead or res_blur_mode == lso.  Needed for the straightforward reconstruction [mm]. (default: 0.)")

	("sfr_flag", po::value<std::string>(), "Flag for the straightforward reconstruction. Applicable only for the dualhead or lso modes (default: false; options: true or false)");

	po::variables_map vm;
	po::store(po::parse_command_line(argc, argv, desc), vm);
	po::notify(vm);

	if (argc == 1 or vm.count("help") or vm.count("h")) 
	{
		std::cout << desc << std::endl;
		return 1;
	}

	SET_PRM_ENV_VAR("scanner_name", "PRM_SCANNER_NAME", "test_scanner_name");
	SET_PRM_ENV_VAR("ps_files_no", "PRM_PS_FILES", "1");
	SET_PRM_ENV_VAR("mode", "PRM_MODE", "9");
	SET_PRM_ENV_VAR("e_min", "PRM_E_MIN", "0.15");
	SET_PRM_ENV_VAR("e_max", "PRM_E_MAX", "0.4");
	SET_PRM_ENV_VAR("edep_process", "PRM_EDEP_PROCESS", "compt");
	SET_PRM_ENV_VAR("phantom_min", "PRM_PH_MIN", "-150.");
	SET_PRM_ENV_VAR("phantom_max", "PRM_PH_MAX", "350.");
	SET_PRM_ENV_VAR("phantom_bins_no", "PRM_PH_BINS", "250");
	SET_PRM_ENV_VAR("res_blur_mode", "PRM_RES_MODE", "barrel");
	SET_PRM_ENV_VAR("res_blur_x", "PRM_RES_X", "3");
	SET_PRM_ENV_VAR("res_blur_y", "PRM_RES_Y", "3");
	SET_PRM_ENV_VAR("res_blur_z", "PRM_RES_Z", "3");
	SET_PRM_ENV_VAR("res_blur_t", "PRM_RES_T", "0.1");
	SET_PRM_ENV_VAR("beta_range", "PRM_BETA_RANGE", "25.");
	SET_PRM_ENV_VAR("y_surface", "PRM_ANNIHIL_Y", "0.");
	SET_PRM_ENV_VAR("sfr_flag", "PRM_SFR_FLAG", "false");


	if (!(vm.count("dir_path"))) 
	{
		std::cout << "You need to specify the directory to the results from the the GATE simulation output. See help for further information." << std::endl;
		return 1;
	}
	
	try
	{
		//atof(getenv("GOJA_COMPTON_E_TH"));
		gROOT->Reset();

		std::stringstream sss;
		sss << vm["dir_path"].as<std::string>();
		setenv("PRM_DIR_PATH", sss.str().c_str(), 1);
		
		std::string dir_path = std::getenv("PRM_DIR_PATH");
		std::string scanner_name = std::getenv("PRM_SCANNER_NAME");
		int phaseSpace_root_files = atoi(std::getenv("PRM_PS_FILES"));
		int mode = atoi(std::getenv("PRM_MODE"));
		
		bool multiple_root_files_flag = false;
		if (phaseSpace_root_files > 1)
			multiple_root_files_flag = true;
		TRandom *r = new TRandom3();

/*

		MODE:
		No.	Description
		
		1	Full annihilation gammas analysis
		2	Prompt gammas analysis (from phase space actors)
#TODO		3	Prompt gammas analysis (from phase space outgoing and root output)
		4	Time analysis of the produced generic ions and annihilations
		5	VHEE electrons analysis
		6	Outgoing particles spectra analysis
		7	Energy deposition within plastic spectrum analysis
#TODO MONIKA	8	Kinetic energy vs deposited energy analysis
		9	Complex analysis and plots
		10	IEEE MIC and complex analysis plots
		11	Dummies

*/ 
		gROOT->Reset();
		
		PhaseSpace ps(dir_path, scanner_name, mode, multiple_root_files_flag);
		PhaseSpaceOutgoing pso(dir_path, scanner_name);
		HitsRoot hr(dir_path, scanner_name, mode);
		Dose ds(dir_path, scanner_name, true);
		Plot plt(scanner_name);

		if (mode == 1)
		{
			std::cout << "Mode 1" << std::endl;			
			if (!multiple_root_files_flag)
			{
				ps.analysisAnnihilationGammas();
				ps.closeRootFile();
			}
			else
			{
				for (int p = 0; p<phaseSpace_root_files; p++)
				{
				ps.init(p);
				ps.analysisAnnihilationGammas();
				ps.closeRootFile();
				}					
			}

			hr.setPositrons(ps.getPositrons());				
			hr.analysis();
			hr.closeRootFile();
		}

		else if (mode == 2)
		{
			std::cout << "Mode 2" << std::endl;			
			if (!multiple_root_files_flag)
			{
				ps.analysisPromptGammas();
				ps.closeRootFile();
			}
			else
			{
				for (int p = 0; p<phaseSpace_root_files; p++)
				{
				ps.init(p);
				ps.analysisPromptGammas();
				ps.closeRootFile();
				}					
			}
			pso.analysisPromptGammas();
			pso.closeRootFile();
		}
// TODO
/*		else if (mode == 3)
		{
			std::cout << "Mode 3" << std::endl;			
			pso.analysisPromptGammasComparison();
		}
*/
		else if (mode == 4)
		{
			std::cout << "Mode 4" << std::endl;			
			if (!multiple_root_files_flag)
			{
				ps.analysisGenericIonsAndAnnihilation();
				ps.closeRootFile();
			}
			else
			{
				for (int p = 0; p<phaseSpace_root_files; p++)
				{
					ps.init(p);
					ps.analysisGenericIonsAndAnnihilation();
					ps.closeRootFile();
				}					
			}
		}

		else if (mode == 5)
		{
			std::cout << "Mode 5" << std::endl;			
			if (!multiple_root_files_flag)
			{
				ps.analysisVHEEGammas();
				ps.closeRootFile();
			}
			else
			{
				for (int p = 0; p<phaseSpace_root_files; p++)
				{
				ps.init(p);
				ps.analysisVHEEGammas();
				ps.closeRootFile();
				}					
			}
			pso.analysisVHEEGammas();
			pso.closeRootFile();
		}

		else if (mode == 6)
		{
			std::cout << "Mode 6" << std::endl;			
			pso.outgoing_spectra_energy_analysis();
			pso.closeRootFile();	
		}

		else if (mode == 7)
		{
			std::cout << "Mode 7" << std::endl;			
			hr.spectra_energy_analysis();
			hr.closeRootFile();
		}

// TODO
/*		else if (mode == 8)
		{
			std::cout << "Mode 8" << std::endl;			
			ps.analysisVHEEGammas();
			pso.analysisVHEEGammas();
		}
*/
		else if (mode == 9)
		{
			std::cout << "Mode 9" << std::endl;			
			std::cout << "Dose" << std::endl;
			ds.readRAW();
			std::cout << "PhasSpace actor" << std::endl;
			if (!multiple_root_files_flag)
			{
				ps.analysisAnnihilationGammas();
				ps.closeRootFile();
			}
			else
			{
				for (int p = 0; p<phaseSpace_root_files; p++)
				{
					ps.init(p);
					std::cout << p << std::endl << std::endl;
					ps.analysisAnnihilationGammas();
					ps.closeRootFile();
				}					
			}

			std::cout << "Crystals" << std::endl;
			hr.setPositrons(ps.getPositrons());				
			hr.analysis();
			hr.closeRootFile();
//			std::cout << "Plots" << std::endl;
//			plt.plot_IEEE();
		}


		else if (mode == 10)
		{
			std::cout << "Mode 10" << std::endl;			
			std::cout << "Plots" << std::endl;
			plt.plot_complex_profile_analysis();
			plt.plot_IEEE_interspill();
			plt.plot_IEEE_integrated();
		}

		else
		{
			std::cout << "Random value from gaussian (mean = 0. mm, sigma = 1e-9 * 0.23 mm): " << r->Gaus(0.,1e-9 * 0.23) << std::endl;
			std::cout << 1e-9 * 0.23 << std::endl;
			ds.readRAW();
		}
		if (mode > 0 and mode <10)
		{
			ps.closeRootFile();
			pso.closeRootFile();
			hr.closeRootFile();
		}

	}
	catch(const std::logic_error& e )
	{
		std::cerr << e.what() << std::endl;
	}
	catch(...)
	{
		std::cerr << "Undefined exception" << std::endl;
	}	
	return 0;
}
