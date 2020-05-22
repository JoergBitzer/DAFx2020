
// Was fehlt noch?
// smoothing of edges // Bessel Filter 2-4 .Ordnung. Grenzfrequenz 1kHz




#pragma once
#include <vector>
#include "ILFO.h"
#include "LFOsin.h"
#include "LFOsaw.h"
#include "LFOtri.h"
class LFO
{
public:

	enum class LFOFunctions
	{
		sinus,
		triangle,
		saw,
		nroflfofunctions
	};
	enum class EmptyCyclePos
	{
		zero,
		max,
		min,
		last
	};
	LFO();
	~LFO();

	int getData(std::vector<double>& data);
	void setMin(double newMin);
	void setMax(double newMax);
	void setMean(double newMean);
	void setAmplitude(double newAmplitude);
	void setFrequency(double frequency_Hz);
	void setSamplerate(double samplerate_Hz);
	void setStartphase(double startPhase_rad); 
	void setFormFactor(double formfak);
	void invertForm();
	void setPulseWidth(double pulsewidth_percent);

	void setMaxCycleTime(double maxCycle_percenz);
	void setLFOFunction(LFOFunctions newFunc);
	void reset();
	void setDelay(double delay_ms);
	void setSmoothing(bool withSmoothing = false);
	void setEmptyCyclePos(EmptyCyclePos pos);
	void setSyncedPhase(bool phaseIsSynced)
	{
		m_lfoBasis->setSyncedPhase(phaseIsSynced);
	};

protected:
	ILFO* m_lfoBasis;
	LFOsin	m_lfosin;
	LFOsaw m_lfosaw;
	LFOtri m_lfotri;
	//LFOrect m_lforect;
	//LFOsampleAndHold m_sampleandhold;
	double m_max;
	double m_min;
	double m_maxmin;
	double m_mean;
	double m_amplitude;
	double m_fs;
	double m_freq;
	double m_startphase;
	double m_formFactor;
	double m_pulseWidth;
	double m_maxCycle;
	double m_invert;

	int m_ReadCounter;
	int m_WriteCounter;
	int m_delay_samples;
	std::vector <double> m_delayline;
	double m_delay_ms;
	bool m_withdelay;
	void computeDelay() {
		m_delay_samples = static_cast<int>(0.001 * m_delay_ms * m_fs + 0.5); 
		m_delayline.resize(m_delay_samples);
		m_WriteCounter = 0;
		m_ReadCounter = 1;
	};

	bool m_withSmoothing;
	double m_smoothcut;
	double m_state1, m_state2;
	std::vector<double> m_b, m_a, m_d, m_c;


};

