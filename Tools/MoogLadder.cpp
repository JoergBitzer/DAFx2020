#include "MoogLadder.h"

MoogLadder::MoogLadder()
	:m_fs(44100.f),
	m_cutoff(1000.f),
	m_resonance(0.f)
{
	reset();
	changeParameter();
}

MoogLadder::~MoogLadder()
{
}

int MoogLadder::processData(std::vector<double>& in, std::vector<double>& out)
{
	for (auto kk = 0U; kk < in.size(); ++kk)
	{
		double curIn = in[kk];
		double u = m_alpha_0 * (curIn - m_resonance * (m_state1 * m_beta1 +
			m_state2 * m_beta2 + m_state3 * m_beta3 + m_state4 * m_beta4));

		double v1 = m_alpha * (u - m_state1);
		double y1 = v1 + m_state1;

		double v2 = m_alpha * (y1 - m_state2);
		double y2 = v2 + m_state2;

		double v3 = m_alpha * (y2 - m_state3);
		double y3 = v3 + m_state3;

		double v4 = m_alpha * (y3 - m_state4);
		double y4 = v4 + m_state4;

		out[kk] = y4;

		m_state1 = v1 + y1;
		m_state2 = v2 + y2;
		m_state3 = v3 + y3;
		m_state4 = v4 + y4;
	}
	return 0;
}

void MoogLadder::reset()
{
	m_state1 = 0.0;
	m_state2 = 0.0;
	m_state3 = 0.0;
	m_state4 = 0.0;
}

void MoogLadder::changeParameter()
{
	double g = tan(M_PI * m_cutoff / m_fs);
	m_alpha = g / (1.0 + g);
	m_alpha_0 = 1.0 / (1.0 + m_resonance * m_alpha * m_alpha * m_alpha * m_alpha);
	m_beta4 = 1.0 / (1.0 + g);
	m_beta3 = m_alpha / (1.0 + g);
	m_beta2 = m_alpha * m_alpha / (1.0 + g);
	m_beta1 = m_alpha * m_alpha * m_alpha / (1.0 + g);
}
#ifdef JUCE

int MoogLadderParameter::addParameter(std::vector<std::unique_ptr<RangedAudioParameter>>& paramVector)
{
	paramVector.push_back(std::make_unique<AudioParameterFloat>(paramCutoff.ID,
		paramCutoff.name,
		NormalisableRange<float>(paramCutoff.minValue, paramCutoff.maxValue),
		paramCutoff.defaultValue,
		paramCutoff.unitName,
		AudioProcessorParameter::genericParameter,
		[](float value, int MaxLen) { value = int(exp(value) * 10)*0.1;  return String(value, MaxLen); },
		[](const String& text) {return text.getFloatValue(); }));
	
	paramVector.push_back(std::make_unique<AudioParameterFloat>(paramResonance.ID,
		paramResonance.name,
		NormalisableRange<float>(paramResonance.minValue, paramResonance.maxValue),
		paramResonance.defaultValue,
		paramResonance.unitName,
		AudioProcessorParameter::genericParameter,
		[](float value, int MaxLen) {return String(value, MaxLen); },
		[](const String& text)	{ return text.getFloatValue();}));
	
	
	return 0;
}
#endif
