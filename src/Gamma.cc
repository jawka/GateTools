#include "Gamma.hh"

void Gamma::print_Gamma()
{
	std::cout << "Run ID: " << run_id << std::endl;
	std::cout << "Event ID: " << event_id << std::endl;
	std::cout << "Track ID: " << track_id << std::endl;
	std::cout << "Parent ID: " << parent_id << std::endl;
	std::cout << "Time: " << time << std::endl;
	std::cout << "Kinetic energy: " << e_kin << std::endl;
	std::cout << "Gamma X position : " << x  << " mm" << std::endl;
	std::cout << "Gamma Y position: " << y  << " mm" << std::endl;
	std::cout << "Gamma Z position: " << z  << " mm" << std::endl;
}


