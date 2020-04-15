#pragma once

#define _USE_MATH_DEFINES
#include <cmath>

#include <vector>

#include "ILFO.h"

class LFOsaw : public ILFO
{
public:
	LFOsaw();
	~LFOsaw();

	void setFrequency(double frequency);
	void setSamplingrate(double samplerate);
	void setStartPhase(double phase);
	void reset();

	int getData(std::vector<double>& data);

private:
	double m_curPhase;
	double m_deltaPhase;
	void computeDeltaPhase();


};

