#pragma once
#include <vector>
#include "ILFO.h"
#include "LFOsin.h"
#include "LFOrect.h"
#include "LFOtriangle.h"
#include "LFOsampleAndHold.h"

class LFO
{
public:
	enum LFOFunctions
	{
		sinus,
		rect,
		tri,
		sampleAndHold,
		nroflfofunctions
	};
	LFO();
	~LFO();
	int getData(std::vector<double>& data);
	void setMin(double newMin);
	void setMax(double newMax);
	void setMean(double newMean);
	void setAmplitude(double newAmplitude);
	void setFrequency(double frequency);
	void setSamplerate(double samplerate);
	void setStartphase(double startPhase);
	void setSlopeTime(double slopetime);
	void setLFOFunction(LFOFunctions newFunc);
	void reset();

protected:
	ILFO* m_lfoBasis;
	LFOsin	m_lfosin;
	LFOrect m_lforect;
	LFOtriangle m_lfotri;
	LFOsampleAndHold m_sampleandhold;
	double m_max;
	double m_min;
	double m_maxmin;
	double m_mean;
	double m_amplitude;
	double m_fs;
	double m_freq;
	double m_startphase;
	double m_slopeTime;
};

