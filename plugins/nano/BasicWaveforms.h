#pragma once
#define _USE_MATH_DEFINES

#include <cmath>
#include <vector>

class BasicWaveforms
{
public:
	enum waveforms
	{
		rect = 0,
		saw,
		tri,
		nrofwaveforms
	};

	BasicWaveforms(int lenWavetable, double maxFreq, double tablef0);
	void getWavetable(std::vector<double>& wavetable, waveforms waveform);

private:
	double m_maxFreq;
	double m_tablef0;
	int m_len;
	int m_maxHarm;
	void computeRect();
	void computeSaw();
	void computeTriangle();
	std::vector<double> m_waveform;
};

