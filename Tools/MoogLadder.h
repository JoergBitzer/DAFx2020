#pragma once
#define _USE_MATH_DEFINES 
#include <cmath>
#include <vector>


class MoogLadder
{
public:
	MoogLadder();
	~MoogLadder();
	
	int processData(std::vector<double>& in, std::vector<double>& out);
	void setResonance(double resonance) { m_resonance = resonance; changeParameter(); };
	void setCutoffFrequency(double cutoff) { m_cutoff = cutoff; changeParameter(); };
	void setSamplerate(double samplerate) { m_fs = samplerate; changeParameter(); };
	void reset();

private:
	double m_fs;
	double m_cutoff;
	double m_resonance;

	double m_alpha_0;
	double m_alpha;
	double m_beta1, m_beta2, m_beta3, m_beta4;
	double m_state1, m_state2, m_state3, m_state4;

	void changeParameter();
};

#ifdef JUCE
#include <JuceHeader.h>
const struct
{
	std::string ID = "cutoff";
	std::string name = "Cutoff";
	std::string unitName = "Hz";
	float minValue = log(30.f);
	float maxValue = log(15000.f);
	float defaultValue = log(13000.f);
}paramCutoff;

const struct
{
	std::string ID = "resonance";
	std::string name = "Resonance";
	std::string unitName = "";
	float minValue = 0.0f;
	float maxValue = 3.99f;
	float defaultValue = 2.f;
}paramResonance;

class MoogLadderParameter
{
public:
	// Lowpass

	int addParameter(std::vector < std::unique_ptr<RangedAudioParameter>>& paramVector);
};

#endif