#include "Hit.hh"

void Hit::print_Hit()
{
	std::cout << "Run ID: " << run_id << std::endl;
	std::cout << "Event ID: " << event_id << std::endl;
	std::cout << "Track ID: " << track_id << std::endl;
	std::cout << "Parent ID: " << parent_id << std::endl;
	std::cout << "Time: " << time << std::endl;
	std::cout << "Deposited energy: " << e_dep << std::endl;
	std::cout << "Gamma X position : " << x  << " mm" << std::endl;
	std::cout << "Gamma Y position: " << y  << " mm" << std::endl;
	std::cout << "Gamma Z position: " << z  << " mm" << std::endl;
	std::cout << "Number of Compton scatterings within the phantom: " << nPhantomCompton << std::endl;
	std::cout << "Number of Rayleigh scatterings within the phantom" << nPhantomRayleigh << std::endl << std::endl;
	std::cout << "Number of Compton scatterings within the plastic: " << nCrystalCompton << std::endl;
	std::cout << "Number of Rayleigh scatterings within the plastic" << nCrystalRayleigh << std::endl << std::endl;
}


