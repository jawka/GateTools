#ifndef PROTON_HH
#define PROTON_HH

#include <iostream>

/*
@JakubBaran

Class for storing incoming protons

*/
class Proton
{

public:

	int run_id;
	int event_id;
	int track_id;
	int parent_id;
	float time;
	float x;
	float y;
	float z;
	float dx;
	float dy;
	float dz;
	float edep;
	
	Proton(int _run_id, int _event_id, int _track_id, int _parent_id, float _time, float _x, float _y, float _z, float _dx, float _dy, float _dz)
			:run_id (_run_id), event_id(_event_id), track_id(_track_id), parent_id(_parent_id), time(_time), x(_x), y(_y), z(_z), dx(_dx), dy(_dy), dz(_dz){}

	void print_Proton();
	void set_edep(float _edep);
};

#endif // PROTON_HH
