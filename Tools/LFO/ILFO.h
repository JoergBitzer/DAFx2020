#pragma once
#include <vector>
class ILFO
{
public:
	virtual void setFrequency(double frequency) = 0;
	virtual void setSamplingrate(double samplerate) = 0;
	virtual void setStartPhase(double phase) = 0;
	virtual void setSlopeTime(double slopetime) {};
	virtual void reset() = 0;

	virtual int getData(std::vector<double>& data) = 0;

protected:
	double m_freq;
	double m_fs;
	double m_startPhase;
};

