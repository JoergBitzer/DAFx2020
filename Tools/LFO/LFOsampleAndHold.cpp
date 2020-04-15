#include "LFOsampleAndHold.h"

LFOsampleAndHold::LFOsampleAndHold()
{
	m_curPhase = 0.0;
	m_freq = 1.0;
	m_fs = 1.0;
	m_deltaPhase = 0.0;
	m_startPhase = 0.0;

	std::random_device rd;
	m_mt = std::mt19937(rd());
	m_dist = std::uniform_real_distribution<double>(0.0, 1.0);
	m_noiseValue = m_dist(m_mt);
	m_oldnoiseValue = m_dist(m_mt);

}

LFOsampleAndHold::~LFOsampleAndHold()
{
}

void LFOsampleAndHold::setFrequency(double frequency)
{
	m_freq = frequency;
	computeDeltaPhase();
}

void LFOsampleAndHold::setSamplingrate(double samplerate)
{
	m_fs = samplerate;
	computeDeltaPhase();
}

void LFOsampleAndHold::setStartPhase(double phase)
{
	m_startPhase = phase;
	reset();
}

void LFOsampleAndHold::setSlopeTime(double slopetime)
{
	m_slopeCounter = 0;
	double help = slopetime * 0.01 * m_fs / m_freq;

	m_slopeLen = (int) slopetime*0.01 * m_fs / m_freq;
	m_slopeFunction.resize(m_slopeLen);
	// Hann Fenster 0.5-0.5*cos(x) 0 <= x <= pi
	for (unsigned int kk = 0; kk < m_slopeFunction.size(); kk++)
	{
		m_slopeFunction[kk] = 0.5 - 0.5 * cos((double(kk) + 1.0) / (m_slopeLen + 1) * M_PI);
	}

}

void LFOsampleAndHold::reset()
{
	m_curPhase = m_startPhase;
}

int LFOsampleAndHold::getData(std::vector<double>& data)
{
	for (unsigned int kk = 0; kk < data.size(); kk++)
	{
		m_curPhase += m_deltaPhase;
		if (m_slopeCounter < m_slopeLen)
		{
			data.at(kk) = (m_noiseValue - m_oldnoiseValue) * m_slopeFunction.at(m_slopeCounter) + m_oldnoiseValue;
			m_slopeCounter++;
		}
		else
			data.at(kk) = m_noiseValue;

		if (m_curPhase >= 2.0 * M_PI)
		{
			m_curPhase -= 2.0 * M_PI;
			m_oldnoiseValue = m_noiseValue;
			m_noiseValue = m_dist(m_mt);
			m_slopeCounter = 0;
		}
	}
	return 0;
}

void LFOsampleAndHold::computeDeltaPhase()
{
	m_deltaPhase = 2.0 * M_PI * m_freq / m_fs;
}
