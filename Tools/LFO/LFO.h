#pragma once
#include <vector>
#include "ILFO.h"
#include "LFOsin.h"
#include "LFOrect.h"
#include "LFOtriangle.h"
#include "LFOsaw.h"
#include "LFOsampleAndHold.h"

class LFO
{
public:
	enum LFOFunctions
	{
		sinus,
		tri,
		rect,
		saw,
		sampleAndHold,
		nroflfofunctions
	};
	LFO();
	~LFO();
	int getData(std::vector<double>& data);
	void setMin(double newMin);
	void setMax(double newMax);
	void setMean(double newMean);
	void setAmplitude(double newAmplitude);
	void setFrequency(double frequency);
	void setSamplerate(double samplerate);
	void setStartphase(double startPhase);
	void setPhase(double phase);
	void setSlopeTime(double slopetime);
	void setLFOFunction(LFOFunctions newFunc);
	void reset();

protected:
	ILFO* m_lfoBasis;
	LFOsin	m_lfosin;
	LFOrect m_lforect;
	LFOtriangle m_lfotri;
	LFOsaw m_lfosaw;
	LFOsampleAndHold m_sampleandhold;
	double m_max;
	double m_min;
	double m_maxmin;
	double m_mean;
	double m_amplitude;
	double m_fs;
	double m_freq;
	double m_startphase;
	double m_slopeTime;
	double m_phase;
};


//#define USE_JUCE 1
#ifdef USE_JUCE
#include <JuceHeader.h>
// several IDs to allow more than one instance
#define MAX_LFO_INSTANCES 4
const struct
{
	const std::string ID[MAX_LFO_INSTANCES] = { "LFO1Waveform", "LFO2Waveform", "LFO3Waveform", "LFO4Waveform" };
	std::string name = "Waveform";
	std::string unitName = "";
	const char* Choices[4] = { "sinus","triangle","rectangular","sawtooth"};
	int defaultIndex = 0;
}paramLFOWaveform;

const struct
{
	const std::string ID[MAX_LFO_INSTANCES] = { "LFO1Rate", "LFO2Rate", "LFO3Rate", "LFO4Rate" };
	std::string name = "Rate";
	std::string unitName = "Hz";
	float minValue = log(0.05f);
	float maxValue = log(40.f);
	float defaultValue = log(1.f);
}paramLFORate;

const struct
{
	const std::string ID[MAX_LFO_INSTANCES] = { "LFO1Level", "LFO2Level", "LFO3Level", "LFO4Level" };
	std::string name = "Level";
	std::string unitName = "dB";
	float minValue = -90.f;
	float maxValue = 0.f;
	float defaultValue = -90.f;
}paramLFOLevel;



class LFOParameter
{
public:
	int addParameter(std::vector < std::unique_ptr<RangedAudioParameter>>& paramVector, int instance);
};

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
typedef AudioProcessorValueTreeState::ComboBoxAttachment ComboAttachment;
class LFOParameterComponent : public Component
{
	enum LFOComponentStyle
	{
		full,
		compact, // no labels
		longvertical, // all in a row
		longhorizontal
	};
public:
	LFOParameterComponent(AudioProcessorValueTreeState&,int index, const String& lfoName);
	void paint(Graphics& g) override;
	void resized() override;
	void setStyle(LFOComponentStyle style);
	std::function<void()> somethingChanged;

private:
	int m_index;
	String m_name;
	Label m_lfowaveformLabel;
	ComboBox m_lfowaveformCombo;
	std::unique_ptr<ComboAttachment> m_lfowaveformAttachment;
	Label m_lforateLabel;
	Slider m_lforateSlider;
	std::unique_ptr<SliderAttachment> m_lforateAttachment;
	Label m_lfolevelLabel;
	Slider m_lfolevelSlider;
	std::unique_ptr<SliderAttachment> m_lfolevelAttachment;


	AudioProcessorValueTreeState& m_vts;
	LFOComponentStyle m_style;


};
#endif