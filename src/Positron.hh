#ifndef POSITRON_HH
#define POSITRON_HH

#include <iostream>

/*
@JakubBaran

Class for storing Hit within the crystal.

*/
class Positron
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
	int gamma_track1;
	int gamma_track2;
	
	Positron(int _run_id, int _event_id, int _track_id, int _parent_id, float _time, float _x, float _y, float _z, int _gamma_track1, int _gamma_track2)
			:run_id (_run_id), event_id(_event_id), track_id(_track_id), parent_id(_parent_id), time(_time), x(_x), y(_y), z(_z), gamma_track1(_gamma_track1), gamma_track2 (_gamma_track2){}

	void print_Positron();
};

#endif // POSITRON_HH
