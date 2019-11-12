#ifndef GAMMA_HH
#define GAMMA_HH

#include <iostream>

/*
@JakubBaran

Class for storing Hit within the crystal.

*/
class Gamma
{

public:
	int run_id;
	int event_id;
	int track_id;
	int parent_id;
	double time;
	float e_kin;
	float x;
	float y;
	float z;

	//Constructor
	Gamma(int _run_id, int _event_id, int _track_id, int _parent_id, double _time, float _e_kin, float _x, float _y, float _z)
			:run_id(_run_id), event_id(_event_id), track_id(_track_id), parent_id(_parent_id), time(_time), e_kin(_e_kin), x(_x), y(_y), z(_z){}
	// Method to print out the gamma properties
	void print_Gamma();
};

#endif // GAMMA_HH
