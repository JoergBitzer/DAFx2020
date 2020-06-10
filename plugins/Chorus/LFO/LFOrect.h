#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>

#include "ILFO.h"
class LFOrect : public ILFO
{
public:
	LFOrect();
	~LFOrect();

	void setFrequency(double frequency);
	void setSamplingrate (double samplerate);
	void setStartPhase(double phase);
	void reset();

	int getData(std::vector<double>& data);

private:
	double m_curPhase;
	double m_deltaPhase;
	void computeDeltaPhase();

};

