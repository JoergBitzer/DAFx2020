#include "LFOsaw.h"

LFOsaw::LFOsaw()
{
	m_curPhase = 0.0;
	m_freq = 1.0;
	m_fs = 1.0;
	m_deltaPhase = 0.0;
	m_startPhase = 0.0;
	m_maxCyclePhase = 2.0 * M_PI;
	m_InnerCurPhase = 0.0;
	m_InnerCycleDeltaPhase = 0.0;
	m_PulseWidth = 2.0 * M_PI;
	m_syncedInnerPhase = false;
	m_maxCyclePhase = 2.0 * M_PI;
	m_last = 0.0;
	m_zeroCyclePosValue = -2.0;

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
	m_curPhase = 0.0;
	m_last = -2.0;
}

void LFOsaw::setPulseWidth(double pulsewidth_percent)
{
	m_PulseWidth = 0.01*pulsewidth_percent * 2.0 * M_PI;
	computeDeltaPhase();
}

void LFOsaw::setMaxCycleTime(double maxCycle_percent)
{
	m_maxCyclePhase = 0.01 * maxCycle_percent * 2.0 * M_PI;
	computeDeltaPhase();
}

void LFOsaw::setEmptyCyclePosValue(double value)
{
	if (isnan(value))
	{
		m_useLast = true;
		m_zeroCyclePosValue = 0.0;
	}
	else
	{
		m_useLast = false;
		m_zeroCyclePosValue = value;
	}
}

int LFOsaw::getData(std::vector<double>& data)
{
	for (unsigned int kk = 0; kk < data.size(); kk++)
	{
		double out = (m_InnerCurPhase + m_startPhase - M_PI)/(M_PI);
		if (out > 1.0)
			out -= 2.0;
		if (m_curPhase >= m_PulseWidth)
		{
			if (m_useLast == true)
				data[kk] = m_last;
			else
				data[kk] = m_zeroCyclePosValue;

		}
		else
		{
			m_InnerCurPhase += m_InnerCycleDeltaPhase;
			if (m_syncedInnerPhase == false)
			{
				if (m_InnerCurPhase >= 2.0 * M_PI)
					m_InnerCurPhase -= 2.0 * M_PI;
			}
			data.at(kk) = out;
			m_last = out;
		}
		
		m_curPhase += m_deltaPhase;
		if (m_curPhase >= 2.0 * M_PI)
		{
			m_curPhase -= 2.0 * M_PI;
			if (m_syncedInnerPhase == true)
				m_InnerCurPhase = 0.0;

		}
	}
	return 0;
}

void LFOsaw::computeDeltaPhase()
{
	m_deltaPhase = 2.0 * M_PI * m_freq / m_fs;
	m_InnerCycleDeltaPhase = m_deltaPhase*(2.0*M_PI/m_PulseWidth)*(m_maxCyclePhase/(2.0*M_PI));
}
