#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>

#include "ILFO.h"
class LFOsin : public ILFO
{
public:
	LFOsin();
	~LFOsin();

	void setFrequency(double frequency);
	void setSamplingrate (double samplerate);
	void setStartPhase(double phase);
	void setPhase(double phase);
	void reset();

	int getData(std::vector<double>& data);

private:
	double m_curPhase;
	double m_deltaPhase;
	void computeDeltaPhase();
	double m_phaseshift;

};

