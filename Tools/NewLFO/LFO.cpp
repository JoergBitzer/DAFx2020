#include "LFO.h"
#include "DSPhelper.h"
#include <cassert>
LFO::LFO()
	:m_freq(1.0),
	m_fs(1.0),
	m_max(1.0),
	m_min(1.0),
	m_mean(0.0),
	m_amplitude(1.0),
	m_maxmin(2.0),
	m_formFactor(1.0),
	m_invert(1.0),
	m_withdelay(false),
	m_withSmoothing(false),
	m_pulseWidth(100.0),
	m_ReadCounter(0),
	m_WriteCounter(0),
	m_maxCycle(100.0),
	m_delay_ms(0.0),
	m_delay_samples(1),
	m_smoothcut(500.0),
	m_startphase(0.0),
	m_state1(0.0),
	m_state2(0.0),
	m_polarity(Polarity::bipolar)
{
	m_lfoBasis = &m_lfosin;
	m_b.resize(3);
	m_a.resize(3);
	m_b[0] = 0.0;
	m_b[1] = 0.0;
	m_b[2] = 1.6221;
	m_a[0] = 1.0;
	m_a[1] = 2.206;
	m_a[2] = 1.6221;
	m_smoothcut = 500.0;
	LowpassLowpassTransform(m_b, m_a, m_d, m_c, m_smoothcut);
	m_b[0] = 1.0;
	m_b[1] = 0.0;
	m_b[2] = 0.0;
	m_a[0] = 1.0;
	m_a[1] = 0.0;
	m_a[2] = 0.0;

}

LFO::~LFO()
{
}

int LFO::getData(std::vector<double>& data)
{
	m_lfoBasis->getData(data);

	for (unsigned int kk = 0; kk < data.size(); kk++)
	{
		
		data[kk] *= m_invert;
		if (m_polarity == Polarity::bipolar)
		{
			if (data[kk] < 0.0)
				data[kk] = -pow(-data[kk], m_formFactor);
			else
				data[kk] = pow(data[kk], m_formFactor);

			data[kk] += 1.0;
			data[kk] *= 0.5;
		}
		else
		{
			data[kk] += 1.0;
			data[kk] *= 0.5;
			data[kk] = pow(data[kk], m_formFactor);
		}

		data[kk] *= m_maxmin;
		data[kk] += m_min;
	}
	if (m_withdelay == true)
	{
		for (unsigned int kk = 0; kk < data.size(); kk++)
		{
			m_delayline[m_WriteCounter++] = data[kk];
			data[kk] = m_delayline[m_ReadCounter++];
			if (m_WriteCounter >= m_delay_samples)
				m_WriteCounter = 0;
			if (m_ReadCounter >= m_delay_samples)
				m_ReadCounter = 0;
			
		}
	}
	if (m_withSmoothing == true)
	{
		for (unsigned int kk = 0; kk < data.size(); kk++)
		{
			// filter structur is Df2 
			double instate = data[kk] - m_state1 * m_a[1] - m_state2 * m_a[2];
			data[kk] = instate*m_b[0] + m_state1 * m_b[1] + m_state2 * m_b[2];
			m_state2 = m_state1;
			m_state1 = instate;
		}
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
	assert(frequency >= 0.0);
	assert(frequency <= m_fs*0.5);
	m_freq = frequency;
	m_lfoBasis->setFrequency(m_freq);
}

void LFO::setSamplerate(double samplerate)
{
	bilinearTransform(m_d, m_c, m_b, m_a, samplerate, m_smoothcut);
	m_fs = samplerate;
	m_lfoBasis->setSamplingrate(m_fs);
	computeDelay();
}

void LFO::setStartphase(double startPhase)
{
	m_startphase = startPhase;
	m_lfoBasis->setStartPhase(m_startphase);
}


void LFO::setFormFactor(double formfak)
{
	m_formFactor = formfak;
}

void LFO::invertForm()
{
	m_invert *= -1.f;
}

void LFO::setPulseWidth(double pulsewidth_percent)
{
	m_lfoBasis->setPulseWidth(pulsewidth_percent);
}

void LFO::setMaxCycleTime(double maxCycle_percenz)
{
	m_lfoBasis->setMaxCycleTime(maxCycle_percenz);
}

void LFO::setLFOFunction(LFO::LFOFunctions newFunc)
{
	switch (newFunc)
	{
	case LFO::LFOFunctions::sinus:
		m_lfoBasis = &m_lfosin;
		break;
	case LFO::LFOFunctions::saw:
		m_lfoBasis = &m_lfosaw;
		break;
	case LFO::LFOFunctions::triangle:
		m_lfoBasis = &m_lfotri;
		break;


/*	case LFOFunctions::rect:
		m_lfoBasis = &m_lforect;
		break;
	case LFOFunctions::sampleAndHold:
		m_lfoBasis = &m_sampleandhold;
		break;
		//*/
	}
	reset();
}
void LFO::setDelay(double delay_ms) 
{
	if (delay_ms > 0.0)
	{ 
		m_withdelay = true; 
		m_delay_ms = delay_ms; 
		computeDelay();
	}
	else 
		m_withdelay = false;
};


void LFO::reset()
{
	m_lfoBasis->setFrequency(m_freq);
	m_lfoBasis->setSamplingrate(m_fs);
	m_lfoBasis->setStartPhase(m_startphase);
	m_lfoBasis->setPulseWidth(m_pulseWidth);
	m_lfoBasis->setMaxCycleTime(m_maxCycle);
	m_lfoBasis->reset();
	m_ReadCounter = 1;
	m_WriteCounter = 0;
	std::fill(m_delayline.begin(), m_delayline.end(), 0.0);
	m_state1 = m_state2 = 0.0;
}

void LFO::setSmoothing(bool withSmoothing)
{
	m_withSmoothing = withSmoothing;
	m_state1 = 0.0;
	m_state2 = 0.0;
}

void LFO::setEmptyCyclePos(EmptyCyclePos pos)
{
	switch (pos) {
	case EmptyCyclePos::min:
		m_lfoBasis->setEmptyCyclePosValue(-1.0);
		break;
	case EmptyCyclePos::max:
		m_lfoBasis->setEmptyCyclePosValue(1.0);
		break;
	case EmptyCyclePos::zero:
		m_lfoBasis->setEmptyCyclePosValue(0.0);
		break;
	case EmptyCyclePos::last:
		m_lfoBasis->setEmptyCyclePosValue(std::nan(""));
		break;
	}

}
