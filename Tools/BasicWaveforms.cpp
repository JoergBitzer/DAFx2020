#include "BasicWaveforms.h"

BasicWaveforms::BasicWaveforms(int lenWavetable, double maxFreq, double tablef0)
	:m_len(lenWavetable),
	m_maxFreq(maxFreq),
	m_tablef0(tablef0),
	m_maxHarm(1),
	m_dutyCycle(0.1)
{
	m_maxHarm = static_cast <int> (m_maxFreq / m_tablef0);
	m_waveform.resize(m_len);
}

void BasicWaveforms::getWavetable(std::vector<double>& wavetable, waveforms waveform)
{
	std::fill(m_waveform.begin(), m_waveform.end(), 0.0);

	switch (waveform)
	{
	case waveforms::rect:
		computeRect();
		break;
	case waveforms::saw:
		computeSaw();
		break;
	case waveforms::tri:
		computeTriangle();

	case waveforms::rectPulse:
		computeRectPulse();
	}
	std::copy(m_waveform.begin(), m_waveform.end(), wavetable.begin());
}
void BasicWaveforms::setDutyCycle(double durtyCicle)
{
	m_dutyCycle = durtyCicle;
}

void BasicWaveforms::computeRect()
{
	for (auto hh = 1; hh < m_maxHarm; hh += 2)
	{
		double Lanczossigma = sin(M_PI * double(hh - 1) / m_maxHarm + 0.000000001) / (M_PI * double(hh - 1) / m_maxHarm + 0.000000001);
		for (auto kk = 0; kk < m_len; kk++)
		{
			m_waveform[kk] += Lanczossigma * 4.0 / (M_PI * hh) * sin(2.0 * M_PI * hh * kk / m_len);
		}
	}
}

void BasicWaveforms::computeSaw()
{
	for (auto hh = 1; hh < m_maxHarm; hh += 1)
	{
		double Lanczossigma = sin(M_PI * double(hh - 1) / m_maxHarm + 0.000000001) / (M_PI * double(hh - 1) / m_maxHarm + 0.000000001);
		for (auto kk = 0; kk < m_len; kk++)
		{
			m_waveform[kk] += Lanczossigma * 2.0 / (M_PI * hh) * pow(-1.0,(hh-1)) * sin(2.0 * M_PI * hh * kk / m_len);
		}
	}
}

void BasicWaveforms::computeTriangle()
{
	for (auto hh = 1; hh < m_maxHarm; hh += 2)
	{
		double Lanczossigma = sin(M_PI * double(hh - 1) / m_maxHarm + 0.000000001) / (M_PI * double(hh - 1) / m_maxHarm + 0.000000001);
		for (auto kk = 0; kk < m_len; kk++)
		{
			m_waveform[kk] += Lanczossigma * 8.0 / (M_PI*M_PI * hh *hh) * pow(-1.0,0.5*double(hh-1)) * sin(2.0 * M_PI * hh * kk / m_len);
		}
	}
}

/**
 * This method computes the basic waveform for a periodic pulse. The duty cicle
 * can be changed by a setter function.
 *
 * author:	Arno Schiller
 * date:	27-04-2020
 *
 * source:	 https://lpsa.swarthmore.edu/Fourier/Series/ExFS.html
 */
void BasicWaveforms::computeRectPulse()
{
	for (auto hh = 0; hh < m_maxHarm; hh += 1)
	{
		double Lanczossigma;
		double a_n;
		if (hh == 0)
		{
			a_n = m_dutyCycle;
			Lanczossigma = 1;
		}
		else
		{
			a_n = 2 / (hh * M_PI) * sin(m_dutyCycle * hh * M_PI); // 2A/(n pi) sin(0.4n pi)
			Lanczossigma = sin(M_PI * double(hh - 1) / m_maxHarm + 0.000000001) / (M_PI * double(hh - 1) / m_maxHarm + 0.000000001);
		}
		for (auto kk = 0; kk < m_len; kk++)
		{
			m_waveform[kk] += Lanczossigma * a_n * cos(2 * M_PI * hh * kk / m_len);
		}
	}
}
