#pragma once
#define _USE_MATH_DEFINES
#include <cmath>

#ifndef M_PI
	#define M_PI 3.141592653589793
#endif

#include <random>
#include <vector>

#include "WhiteNoiseGenerator.h"
#include "FreeOrderLowHighpassFilter.h"
#include "GeneralIR.h"

class ColoredNoiseGenerator : public WhiteNoiseGenerator
{
public:
	ColoredNoiseGenerator();
	~ColoredNoiseGenerator();
	void setSamplerate(double fs)
	{
		m_fs = fs; computeCoeffs(FreeOrderLowHighpassFilter::FilterType::highpass);
		computeCoeffs(FreeOrderLowHighpassFilter::FilterType::lowpass);
	}
	void setHighpassOrder(double hporder)
	{
		m_hporder = hporder; computeCoeffs(FreeOrderLowHighpassFilter::FilterType::highpass);
	}
	void setLowpassOrder(double lporder)
	{
		m_lporder = lporder; computeCoeffs(FreeOrderLowHighpassFilter::FilterType::lowpass);
	}
	void setHighpassCutoff(double hpcut)
	{
		m_hpcut = hpcut; computeCoeffs(FreeOrderLowHighpassFilter::FilterType::highpass);
	}
	void setLowpassCutoff(double lpcut)
	{
		m_lpcut = lpcut; computeCoeffs(FreeOrderLowHighpassFilter::FilterType::lowpass);
	}

	virtual int getData(std::vector<double>& data) override; 

	inline double getOneSample()
	{
		return getOneWhiteSample();

	}

protected:
	FreeOrderLowHighpassFilter m_filter;
	GeneralIR m_lowpass;
	GeneralIR m_highpass;
	int m_filterorder;
	int m_nrofcoefficients;
	std::vector<double> m_b;
	std::vector<double> m_a;

	double m_fs;
	double m_hporder, m_lporder, m_hpcut, m_lpcut;
	void computeCoeffs(FreeOrderLowHighpassFilter::FilterType type);
	
};

#ifdef USE_JUCE

const struct
{
	const std::string ID = { "NoiseColorLow" };
	std::string name = "Color Low";
	std::string unitName = "";
	float minValue = 0.25f;
	float maxValue = 3.f;
	float defaultValue = 0.25f;
}paramNoiseColorLow;

const struct
{
	const std::string ID = { "NoiseColorHigh" };
	std::string name = "ColorHigh";
	std::string unitName = "";
	float minValue = 0.25f;
	float maxValue = 3.f;
	float defaultValue = 0.25f;
}paramNoiseColorHigh;




#endif
