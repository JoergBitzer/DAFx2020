#include "LFOsaw.h"

LFOsaw::LFOsaw()
{
	m_curPhase = 0.0;
	m_freq = 1.0;
	m_fs = 1.0;
	m_deltaPhase = 0.0;
	m_startPhase = 0.0;
}

LFOsaw::~LFOsaw()
{
}

void LFOsaw::setFrequency(double frequency)
{
	m_freq = frequency;
	computeDeltaPhase();
}

void LFOsaw::setSamplingrate(double samplerate)
{
	m_fs = samplerate;
	computeDeltaPhase();
}

void LFOsaw::setStartPhase(double phase)
{	
	m_startPhase = phase;
	reset();
}

void LFOsaw::reset()
{
	m_curPhase = m_startPhase;
}

int LFOsaw::getData(std::vector<double>& data)
{
	for (unsigned int idx = 0; idx < data.size(); idx++)
	{
		double out = m_curPhase / (2.0* M_PI);
		
		data[idx] = out;
		m_curPhase += m_deltaPhase;

		if (m_curPhase >= 2.0 * M_PI)
			m_curPhase -= 2.0 * M_PI;
	}
	return 0;
}

void LFOsaw::computeDeltaPhase()
{
	m_deltaPhase = 2.0 * M_PI * m_freq / m_fs;
}
