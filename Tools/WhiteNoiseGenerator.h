#pragma once
#define _USE_MATH_DEFINES
#include <cmath>

#ifndef M_PI
	#define M_PI 3.141592653589793
#endif

#include <random>
#include <vector>



class WhiteNoiseGenerator
{
public:
	WhiteNoiseGenerator();
	~WhiteNoiseGenerator();

	virtual int getData(std::vector<double>& data);
	int getUniformData(std::vector<double>& data, double min = 0.0, double max = 1.0);

	inline double getOneWhiteSample()
	{
		double noise = m_dist(m_mt);
		if (m_pureDistribution)
			return noise;
		else
			return (m_normalize * m_amplitude * tan(noise * m_grainFactor));
	}

	void setAmplitude(double amplitude) {
		m_amplitude = amplitude;
	};
	void setLogAmplitude(double amplitude_dB) {
		setAmplitude(pow(10.0, amplitude_dB / 20.0));
	};
	void setGraininess(double grainFak);
	void setSeed(int seedVal = 0)
	{
		m_mt.seed(seedVal);
	}
protected:
	std::mt19937 m_mt;
	std::uniform_real_distribution<double> m_dist;

	double m_amplitude;
	double m_grainFactor;
	double m_normalize;
	bool m_pureDistribution;
};

#ifdef USE_JUCE
#include <JuceHeader.h>
const struct
{
	const std::string ID = { "WNLevel_dB" };
	std::string name = "Level";
	std::string unitName = "dB";
	float minValue = -90.f;
	float maxValue = 10.f;
	float defaultValue = -90.f;
}paramNoiseLevel;

const struct
{
	const std::string ID = { "WNGraininess" };
	std::string name = "Graininess";
	std::string unitName = "";
	float minValue = exp(0.2f);
	float maxValue = exp(1.f);
	float defaultValue = exp(0.2f);
}paramNoiseGraininess;

class WhiteNoiseParameter
{
public:
	// Lowpass

	int addParameter(std::vector < std::unique_ptr <RangedAudioParameter>>& paramVector);
};




#endif
