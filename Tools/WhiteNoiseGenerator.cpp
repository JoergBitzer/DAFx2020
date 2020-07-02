#include "WhiteNoiseGenerator.h"

WhiteNoiseGenerator::WhiteNoiseGenerator()
{
	m_amplitude = 1.0;
	m_grainFactor = 0.98;
	std::random_device rd;
	m_mt = std::mt19937(rd());
	m_dist = std::uniform_real_distribution<double>(-M_PI*0.5, M_PI*0.5);
	m_pureDistribution = false;
}

WhiteNoiseGenerator::~WhiteNoiseGenerator()
{
}

int WhiteNoiseGenerator::getData(std::vector<double>& data)
{
	for (unsigned int kk = 0; kk < data.size(); kk++)
	{
		data.at(kk) = getOneWhiteSample();
	}
	return 0;
}

int WhiteNoiseGenerator::getUniformData(std::vector<double>& data, double min, double max)
{
	std::uniform_real_distribution<double> dist(min, max);


	for (unsigned int kk = 0; kk < data.size(); kk++)
	{
		data.at(kk) = dist(m_mt);
	}
	return 0;
}

void WhiteNoiseGenerator::setGraininess(double grainFak)
{
	if ((grainFak >= 0.0) & (grainFak <= 1.0))
	{
		m_grainFactor = 0.99998 * (0.8 + (grainFak * 0.2));
	}
	m_normalize = 1.0 / tan(m_grainFactor * M_PI * 0.5);
}

#ifdef USE_JUCE
int WhiteNoiseParameter::addParameter(std::vector<std::unique_ptr<RangedAudioParameter>>& paramVector)
{

	paramVector.push_back(std::make_unique<AudioParameterFloat>(paramNoiseLevel.ID,
		paramNoiseLevel.name,
		NormalisableRange<float>(paramNoiseLevel.minValue, paramNoiseLevel.maxValue),
		paramNoiseLevel.defaultValue,
		paramNoiseLevel.unitName,
		AudioProcessorParameter::genericParameter,
		[](float value, int MaxLen) { value = int((value) * 10) * 0.1;  return String(value, MaxLen); },
		[](const String& text) {return text.getFloatValue(); }));

	paramVector.push_back(std::make_unique<AudioParameterFloat>(paramNoiseGraininess.ID,
		paramNoiseGraininess.name,
		NormalisableRange<float>(paramNoiseGraininess.minValue, paramNoiseGraininess.maxValue),
		paramNoiseGraininess.defaultValue,
		paramNoiseGraininess.unitName,
		AudioProcessorParameter::genericParameter,
		[](float value, int MaxLen) { value = int(log(value) * 100) * 0.01;  return String(value, MaxLen); },
		[](const String& text) {return text.getFloatValue(); }));


	return 0;
}
#endif
