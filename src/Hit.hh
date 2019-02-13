#ifndef HIT_HH
#define HIT_HH

#include <iostream>

/*
@JakubBaran

Class for storing Hit within the crystal.

*/
class Hit
{

public:
	int run_id;
	int event_id;
	int track_id;
	int parent_id;
	double time;
	float e_dep;
	float x;
	float y;
	float z;
	int nPhantomCompton;
	int nPhantomRayleigh;
	int nCrystalCompton;
	int nCrystalRayleigh;
	
	Hit(int _run_id, int _event_id, int _track_id, int _parent_id, double _time, float _e_dep, float _x, float _y, float _z, int _nPhantomCompton, int _nPhantomRayleigh, int _nCrystalCompton, int _nCrystalRayleigh)
			:run_id(_run_id), event_id(_event_id), track_id(_track_id), parent_id(_parent_id), time(_time), e_dep(_e_dep), x(_x), y(_y), z(_z), nPhantomCompton(_nPhantomCompton), nPhantomRayleigh(_nPhantomRayleigh), nCrystalCompton(_nCrystalCompton), nCrystalRayleigh(_nCrystalRayleigh) {}

	void print_Hit();
};

#endif // HIT_HH
