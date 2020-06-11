#pragma once
#define _USE_MATH_DEFINES

#include <cmath>
#include <random>
#include <vector>



class WhiteNoiseGenerator
{
public:
	WhiteNoiseGenerator();
	~WhiteNoiseGenerator();

	int getData(std::vector<double>& data);
	void setAmplitude(double amplitude) {
		m_amplitude = amplitude;
	};
	void setLogAmplitude(double amplitude_dB) {
		setAmplitude(pow(10.0, amplitude_dB / 20.0));
	};
	void setGraininess(double grainFak);

protected:
	std::mt19937 m_mt;
	std::uniform_real_distribution<double> m_dist;

	double m_amplitude;
	double m_grainFactor;
	double m_normalize;
};

