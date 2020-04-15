#include "LFO.h"

LFO::LFO()
	:m_freq(1.0),
	m_fs(1.0),
	m_max(1.0),
	m_min(1.0),
	m_mean(0.0),
	m_amplitude(1.0),
	m_maxmin(2.0)
{
	m_lfoBasis = &m_lfosin;
}

LFO::~LFO()
{
}

int LFO::getData(std::vector<double>& data)
{
	m_lfoBasis->getData(data);

	for (unsigned int kk = 0; kk < data.size(); kk++)
	{
		data[kk] *= m_maxmin;
		data[kk] += m_min;
	}
	return 0;
}

void LFO::setMin(double newMin)
{
	m_min = newMin;
	m_maxmin = m_max - m_min;
	m_amplitude = m_maxmin * 0.5;
	m_mean = (m_max + m_min) * 0.5;
}

void LFO::setMax(double newMax)
{
	m_max = newMax;
	m_maxmin = m_max - m_min;
	m_amplitude = m_maxmin * 0.5;
	m_mean = (m_max + m_min) * 0.5;
}

void LFO::setMean(double newMean)
{
	m_mean = newMean;
	m_max = m_mean + m_amplitude;
	m_min = m_mean - m_amplitude;
	m_maxmin = m_max - m_min;
}

void LFO::setAmplitude(double newAmplitude)
{
	m_amplitude = newAmplitude;
	m_max = m_mean + m_amplitude;
	m_min = m_mean - m_amplitude;
	m_maxmin = m_max - m_min;
}

void LFO::setFrequency(double frequency)
{
	m_freq = frequency;
	m_lfoBasis->setFrequency(m_freq);
}

void LFO::setSamplerate(double samplerate)
{
	m_fs = samplerate;
	m_lfoBasis->setSamplingrate(m_fs);
}

void LFO::setStartphase(double startPhase)
{
	m_startphase = startPhase;
	m_lfoBasis->setStartPhase(m_startphase);
}

void LFO::setSlopeTime(double slopetime)
{
	m_slopeTime = slopetime;
	m_lfoBasis->setSlopeTime(m_slopeTime);
}

void LFO::setLFOFunction(LFOFunctions newFunc)
{
	switch (newFunc)
	{
	case LFOFunctions::sinus:
		m_lfoBasis = &m_lfosin;
		break;
	case LFOFunctions::rect:
		m_lfoBasis = &m_lforect;
		break;
	case LFOFunctions::sampleAndHold:
		m_lfoBasis = &m_sampleandhold;
		break;
	}
	reset();
}

void LFO::reset()
{
	m_lfoBasis->setFrequency(m_freq);
	m_lfoBasis->setSamplingrate(m_fs);
	m_lfoBasis->setStartPhase(m_startphase);
	m_lfoBasis->setSlopeTime(m_slopeTime);
	m_lfoBasis->reset();

}
