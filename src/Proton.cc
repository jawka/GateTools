#include "Proton.hh"

void Proton::print_Proton()
{
	std::cout << "Run ID: " << run_id << std::endl;
	std::cout << "Event ID: " << event_id << std::endl;
	std::cout << "Track ID: " << track_id << std::endl;
	std::cout << "Parent ID: " << parent_id << std::endl;
	std::cout << "Time: " << time << std::endl;
	std::cout << "Proton entrance X coordinate: " << x  << " mm" << std::endl;
	std::cout << "Proton entrance Y coordinate: " << y  << " mm" << std::endl;
	std::cout << "Proton entrance Z coordinate: " << z  << " mm" << std::endl;
	std::cout << "Proton direction X coordinate: " << dx  << " mm" << std::endl;
	std::cout << "Proton direction Y coordinate: " << dy  << " mm" << std::endl;
	std::cout << "Proton direction Z coordinate: " << dz  << " mm" << std::endl;
}

void Proton::set_edep(float _edep)
{
	edep = _edep;
}
