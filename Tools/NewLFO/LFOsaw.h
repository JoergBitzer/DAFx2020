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
	void setSamplingrate (double samplerate);
	void setStartPhase(double phase);
	void reset();
	void setPulseWidth(double pulsewidth_percent);

	void setMaxCycleTime(double maxCycle_percent);
	void setEmptyCyclePosValue(double value) override;

	int getData(std::vector<double>& data);
	void setSyncedPhase(bool phaseIsSynced)
	{
		m_syncedInnerPhase = phaseIsSynced;
	};

private:
	double m_curPhase;
	double m_InnerCurPhase;
	double m_InnerCycleDeltaPhase;
	double m_deltaPhase;
	double m_maxCyclePhase;
	double m_PulseWidth;
	bool m_syncedInnerPhase;
	void computeDeltaPhase();
	double m_zeroCyclePosValue;
	double m_last;
	bool m_useLast;

};

