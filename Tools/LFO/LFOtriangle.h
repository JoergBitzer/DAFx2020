/*--------------------------------------------------------------------
USAGE: This class generates triangle signal of a given frequency
Author: Jörg Bitzer, changed by Geraldine Sutter
License: BSD 3 clause (Open Source License)

---------------------------------------------------------------------*/



#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <random>


#include "ILFO.h"
class LFOtriangle : public ILFO
{
public:
	LFOtriangle();
	~LFOtriangle();

	void setFrequency(double frequency);
	void setSamplingrate (double samplerate);
	void setStartPhase(double phase);
	void setPhase(double phase);
	void reset();
	//setwidth is setting the width of the signal
	void setPulseWidth(double newwidth);

	int getData(std::vector<double>& data);


private:
	double m_curPhase;
	double m_deltaPhase;
	double m_noiseValue;
	double m_oldnoiseValue;
	//this is the width of the triangle
	double m_width;
	void computeDeltaPhase();
	double m_phaseshift;
//	double m_oldphaseshift;
	double m_inc;

};

