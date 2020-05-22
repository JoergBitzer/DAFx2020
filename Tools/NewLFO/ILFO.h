#pragma once
#include <vector>
class ILFO
{
public:
	virtual void setFrequency(double frequency) = 0;
	virtual void setSamplingrate(double samplerate) = 0;
	virtual void setStartPhase(double phase) = 0;
	virtual void  setPulseWidth(double pulsewidth) = 0;
	virtual	void setMaxCycleTime(double maxCycle_percent) = 0;
	virtual void reset() = 0;
	virtual int getData(std::vector<double>& data) = 0;
	virtual void setEmptyCyclePosValue(double value) = 0;
	virtual void setSyncedPhase(bool phaseIsSynced) = 0;

protected:
	double m_freq;
	double m_fs;
	double m_startPhase;
};

