#include "LFOrect.h"

LFOrect::LFOrect()
{
	m_curPhase = 0.0;
	m_freq = 1.0;
	m_fs = 1.0;
	m_deltaPhase = 0.0;
	m_startPhase = 0.0;
}

LFOrect::~LFOrect()
{
}

void LFOrect::setFrequency(double frequency)
{
	m_freq = frequency;
	computeDeltaPhase();
}

void LFOrect::setSamplingrate(double samplerate)
{
	m_fs = samplerate;
	computeDeltaPhase();
}

void LFOrect::setStartPhase(double phase)
{
	m_startPhase = phase;
	reset();
}

void LFOrect::reset()
{
	m_curPhase = m_startPhase;
}

int LFOrect::getData(std::vector<double>& data)
{
	for (unsigned int kk = 0; kk < data.size(); kk++)
	{
		double out;
		if (m_curPhase < M_PI)
			out = 1.0;
		else
			out = 0.0;
//*/
/*		double out = cos(m_curPhase);
		if (out >= 0.0)
			out = 1.0;
		else
			out = 0.0;
//*/
		data.at(kk) = out;
		m_curPhase += m_deltaPhase;
		if (m_curPhase >= 2.0 * M_PI)
			m_curPhase -= 2.0 * M_PI;
	}
	return 0;
}

void LFOrect::computeDeltaPhase()
{
	m_deltaPhase = 2.0 * M_PI * m_freq / m_fs;
}
