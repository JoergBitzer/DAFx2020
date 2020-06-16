#include "LFOtriangle.h"

LFOtriangle::LFOtriangle()
{
	m_curPhase = 0.0;
	m_freq = 1.0;
	m_fs = 1.0;
	m_deltaPhase = 0.0;
	m_startPhase = 0.0;
	m_width = 2.0 * M_PI;
	m_phaseshift = 0.0;
	m_oldphaseshift = 0.0;

}

LFOtriangle::~LFOtriangle()
{
}

void LFOtriangle::setFrequency(double frequency)
{
	m_freq = frequency;
	computeDeltaPhase();
}

void LFOtriangle::setSamplingrate(double samplerate)
{
	m_fs = samplerate;
	computeDeltaPhase();
	reset();
}

void LFOtriangle::setStartPhase(double phase)
{
	m_startPhase = phase;
	reset();
}

void LFOtriangle::setPhase(double phase)
{
	m_phaseshift = phase;
	m_curPhase += m_phaseshift;
	m_curPhase -= m_oldphaseshift;
	m_oldphaseshift = m_phaseshift;
}


void LFOtriangle::reset()
{

	m_curPhase = m_startPhase + m_phaseshift;
	m_oldphaseshift = m_phaseshift;
}

void LFOtriangle::setPulseWidth(double newwidth)
{
	m_width = newwidth*2.0*M_PI;

}

int LFOtriangle::getData(std::vector<double>& data)
{
	for (unsigned int kk = 0; kk < data.size(); kk++)
	{
		m_curPhase += m_deltaPhase;

		if (m_curPhase >= M_PI)
		{
			m_curPhase = M_PI - (m_curPhase - M_PI);
			m_deltaPhase = -m_deltaPhase;
		}
		if (m_curPhase < 0.0)
		{
			m_curPhase = -m_curPhase;
			m_deltaPhase = -m_deltaPhase;
		}

		data.at(kk) = m_curPhase / (m_width / 2);

/*		if (curPhase > m_width)
		{
			//no more values except '0', since the triangle is then complete
			//data.at(kk) = 0;
			if (curPhase >= 2.0*M_PI)
				curPhase -= m_width;
			else
			{
				data.at(kk) = 0;
			}
		}
		if (curPhase <= m_width / 2)
		{
			//this is the rising part of the triangle
			data.at(kk) = m_curPhase / (m_width / 2);
		}
		if (curPhase > m_width / 2 & curPhase <= m_width)
		{
			//by subtracting '(2 * m_curPhase - m_width)', the previous signal is 'reflected'
			data.at(kk) = (curPhase - (2 * curPhase - m_width)) / (m_width / 2);
		}

		m_curPhase += m_deltaPhase;
		

		if (m_curPhase >= 2.0 * M_PI)
		{
			m_curPhase -= 2.0 * M_PI;
			
		}
		//*/
	}
	return 0;
}

void LFOtriangle::computeDeltaPhase()
{
	m_deltaPhase = 2.0 * M_PI * m_freq / m_fs;
}
