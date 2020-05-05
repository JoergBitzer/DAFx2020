#include "WavetableOszi.h"

WavetableOszi::WavetableOszi()
	:m_lenOfWavetable(512),
	m_fs(0.0),
	m_curPos(0.0),
	m_curWaveform(0),
	m_curWavetable(0),
	m_fsmin(44100),
	m_nrOfWaveforms(0),
	m_nrOfWavetables(0),
	m_interpolationOrder(1),
	m_incStep(0.0),
	m_freq(1.0),
	m_fft(m_lenOfWavetable),
	m_waveformXFadeFactor(0.0),
	m_secondWaveform(0),
	m_incStepModifier(1.0)
{
	m_f0Table = m_fsmin / m_lenOfWavetable;
	double maxAliasingFreeMidiNote = 108.0;
	double a0 = 440.0;
	double midia0 = 69.0;
	//double maxFreq = pow(2.0, (maxAliasingFreeMidiNote - midia0) / 12.0) * a0;
	int neededOctaves = 1 + static_cast<int> (log(m_fsmin / (2.0 * m_f0Table)) / log(2.0));
	m_semitonespertable = 6;
	m_nrOfWavetables = neededOctaves * 12 / m_semitonespertable;
	
	// aliasing free wavetables
	m_real.resize(m_lenOfWavetable / 2 + 1);
	m_imag.resize(m_lenOfWavetable / 2 + 1);
	m_newwave.resize(m_lenOfWavetable);
	computeIncStep();
}

WavetableOszi::~WavetableOszi()
{
}

int WavetableOszi::addF0Wavetable(const std::vector<double>& waveform)
{
	if (waveform.size() != 512)
		return -1;

	m_nrOfWaveforms++;
	m_wavetable.resize(m_nrOfWaveforms);
	m_wavetable[m_nrOfWaveforms - 1].resize(m_nrOfWavetables);
	m_wavetable[m_nrOfWaveforms - 1][0].resize(m_lenOfWavetable + 2*m_interpolationOrder);

	copyNewWavetable(waveform, m_nrOfWaveforms - 1, 0);
	computeFullWavetable();

	return 0;
}

int WavetableOszi::getData(std::vector<double>& data)
{
	double* pDataLow = &m_wavetable[m_curWaveform][m_curWavetable][0];
	pDataLow += m_interpolationOrder;
	double* pDataSecond = &m_wavetable[m_secondWaveform][m_curWavetable][0];
	pDataSecond += m_interpolationOrder;
	for (auto kk = 0; kk < data.size(); kk++)
	{
		int pos = int(m_curPos);
		double fracpos = m_curPos - pos;

		double out = (1.0 - fracpos) * *(pDataLow + pos) + fracpos * *(pDataLow + pos + 1);
		double out2 = (1.0 - fracpos) * *(pDataSecond + pos) + fracpos * *(pDataSecond + pos + 1);

		data[kk] = (1.0-m_waveformXFadeFactor) * out + m_waveformXFadeFactor*out2;

		if (m_portamentoCounter > 0)
		{
			--m_portamentoCounter;
			m_incStep *= m_portamentoFactor;
		}

		m_curPos += (m_incStep*m_incStepModifier);
		if (m_curPos >= m_lenOfWavetable)
			m_curPos -= m_lenOfWavetable;

	}
	return 0;
}

void WavetableOszi::setSamplerate(double samplerate)
{
	m_fs = samplerate;
	m_f0Table = m_fs / m_lenOfWavetable;
	computeIncStep();
}

void WavetableOszi::setFrequency(double freq)
{
	m_freq = freq;
	if (m_portamentoTime_samples <= 0)
	{
		computeIncStep();
		m_portamentoCounter = -1;
	}
	else
	{
		m_portamentoCounter = m_portamentoTime_samples;
		double targetInc = m_freq / m_f0Table;
		m_portamentoFactor = exp(log(targetInc / m_incStep) / m_portamentoTime_samples);
	}
	m_curWavetable = static_cast<int> (log(freq / m_f0Table) / log(pow(2.0, double(m_semitonespertable) / 12.0)));
	if (m_curWavetable < 0)
		m_curWavetable = 0 ;
}

void WavetableOszi::computeIncStep()
{
	m_incStep = m_freq / m_f0Table;
}
void WavetableOszi::setWaveform(int newwaveformnumber)
{
	if (newwaveformnumber < m_nrOfWaveforms)
		m_curWaveform = newwaveformnumber;
}

void WavetableOszi::setWaveformXFade(double xfadefactor)
{
	if (xfadefactor >= 0.0 && xfadefactor <= 1.0)
		m_waveformXFadeFactor = xfadefactor;
}
void WavetableOszi::setSecondWaveform(int waveformnumber)
{
	if (waveformnumber < m_nrOfWaveforms)
		m_secondWaveform = waveformnumber;

}

void WavetableOszi::resetPhase()
{
	m_curPos = 0.0;
}

void WavetableOszi::changeFrequencyRelative(double change_semitones)
{
	m_incStepModifier = pow(2.0, change_semitones / 12.0);
}

void WavetableOszi::setPortamentoTime(double portaTime_ms)
{
	m_portamentoTime_samples = static_cast<int>(portaTime_ms * 0.001 * m_fs);
}

void WavetableOszi::computeFullWavetable()
{
	double* pData = m_wavetable[m_nrOfWaveforms-1][0].data();
	pData += m_interpolationOrder;
	double* pReal = m_real.data();
	double* pImag = m_imag.data();

	m_fft.fft(pData, pReal, pImag);
	double NewFreq = 16000.0 / (m_fsmin / 2.0);

	for (auto tt = 1; tt < m_nrOfWavetables; tt++)
	{
		NewFreq = NewFreq * pow(2.0, double(-m_semitonespertable) / 12.0);
		int maxIndex = static_cast<int>(NewFreq * m_real.size());
		for (auto ff = maxIndex; ff < m_real.size(); ff++)
		{
			m_real[ff] = 0.0;
			m_imag[ff] = 0.0;
		}
		for (auto ff = 1; ff < maxIndex; ff++)
		{
			double LZsigma = sin(M_PI * double(ff-1) / maxIndex + 0.000000000001) / (M_PI * double(ff-1) / maxIndex + 0.000000000001);
			m_real[ff] *= LZsigma;
			m_imag[ff] *= LZsigma;
		}
		pData = m_newwave.data();
		m_fft.ifft(pReal, pImag, pData);
		for (auto ff = 1; ff < maxIndex; ff++)
		{
			double LZsigma = sin(M_PI * double(ff-1) / maxIndex + 0.000000000001) / (M_PI * double(ff-1) / maxIndex + 0.000000000001);
			m_real[ff] /= LZsigma;
			m_imag[ff] /= LZsigma;
		}
		m_wavetable[m_nrOfWaveforms - 1][tt].resize(m_lenOfWavetable + 2 * m_interpolationOrder);

		copyNewWavetable(m_newwave, m_nrOfWaveforms - 1, tt);
	}
}

void WavetableOszi::copyNewWavetable(std::vector<double> newwaveform,int nrWaveform, int nrWavetable)
{
	std::copy(newwaveform.begin(), newwaveform.end(), m_wavetable[m_nrOfWaveforms - 1][nrWavetable].begin() + m_interpolationOrder);
	for (auto kk = 0; kk < m_interpolationOrder; kk++)
	{
		m_wavetable[nrWaveform][nrWavetable][kk] = newwaveform[newwaveform.size() - m_interpolationOrder + kk];
		m_wavetable[nrWaveform][nrWavetable][m_lenOfWavetable + m_interpolationOrder + kk] = newwaveform[kk];
	}
}
