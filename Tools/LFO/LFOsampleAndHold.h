#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#include <vector>
#include <random>


#include "ILFO.h"
class LFOsampleAndHold : public ILFO
{
public:
	LFOsampleAndHold();
	~LFOsampleAndHold();

	void setFrequency(double frequency);
	void setSamplingrate (double samplerate);
	void setStartPhase(double phase);
	// setSlopeTime in percentage (0...100)
	void setSlopeTime(double slopetime) override;
	void reset();

	int getData(std::vector<double>& data);


private:
	double m_curPhase;
	double m_deltaPhase;
	double m_noiseValue;
	double m_oldnoiseValue;
	void computeDeltaPhase();

	int m_slopeCounter;
	int m_slopeLen;

	std::mt19937 m_mt;
	std::uniform_real_distribution<double> m_dist;

	std::vector<double> m_slopeFunction;

};

