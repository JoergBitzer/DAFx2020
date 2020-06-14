#include "LFOsin.h"

LFOsin::LFOsin()
{
	m_curPhase = 0.0;
	m_freq = 1.0;
	m_fs = 1.0;
	m_deltaPhase = 0.0;
	m_startPhase = 0.0;
	m_phaseshift = 0.0;
}

LFOsin::~LFOsin()
{
}

void LFOsin::setFrequency(double frequency)
{
	m_freq = frequency;
	computeDeltaPhase();
}

void LFOsin::setSamplingrate(double samplerate)
{
	m_fs = samplerate;
	computeDeltaPhase();
}

void LFOsin::setStartPhase(double phase)
{
	m_startPhase = phase;
	reset();
}

void LFOsin::setPhase(double phase)
{
	m_phaseshift = phase;
}

void LFOsin::reset()
{
	m_curPhase = m_startPhase;
}

int LFOsin::getData(std::vector<double>& data)
{
	for (unsigned int kk = 0; kk < data.size(); kk++)
	{
		double out = 0.5 * (cos(m_curPhase + m_phaseshift) + 1.0); // normalized between 0..1
		data.at(kk) = out;
		m_curPhase += m_deltaPhase;
		if (m_curPhase >= 2.0 * M_PI)
			m_curPhase -= 2.0 * M_PI;
	}
	return 0;
}

void LFOsin::computeDeltaPhase()
{
	m_deltaPhase = 2.0 * M_PI * m_freq / m_fs;
}
