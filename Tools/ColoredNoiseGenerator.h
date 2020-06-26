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
#include <JuceHeader.h>

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

class ColoredNoiseParameter
{
public:
	// Lowpass

	int addParameter(std::vector < std::unique_ptr<RangedAudioParameter>>& paramVector);
};


typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
typedef AudioProcessorValueTreeState::ComboBoxAttachment ComboAttachment;

class ColoredNoiseComponent : public Component
{
public:
	enum class ComponentStyle
	{
		compact,
		horizontal,
		vertical
	};
	ColoredNoiseComponent(AudioProcessorValueTreeState&);

	void paint(Graphics& g) override;
	void resized() override;
	std::function<void()> somethingChanged;
	void setStyle(ComponentStyle style) { m_style = style; };

private:
	AudioProcessorValueTreeState& m_vts;
	ComponentStyle m_style;

	Label m_NoiseLevelLabel;
	Slider m_NoiseLevelSlider;
	std::unique_ptr<SliderAttachment> m_NoiseLevelAttachment;

	Label m_NoiseGraininessLabel;
	Slider m_NoiseGraininessSlider;
	std::unique_ptr<SliderAttachment> m_NoiseGraininessAttachment;

	Label m_NoiseColorLowLabel;
	Slider m_NoiseColorLowSlider;
	std::unique_ptr<SliderAttachment> m_NoiseColorLowAttachment;

	Label m_NoiseColorHighLabel;
	Slider m_NoiseColorHighSlider;
	std::unique_ptr<SliderAttachment> m_NoiseColorHighAttachment;

};




#endif
