#pragma once

#define _USE_MATH_DEFINES
#include <cmath>
#ifndef M_PI
	#define M_PI 3.1415
#endif
#include <vector>
#include "LFO/LFO.h"
#include "FirstOrderFilter.h"

class SimpleChorus
{
public:
    SimpleChorus();
    void setSamplerate(double samplerate);
    void setMaxBlockSize(int maxSize);
    int processData(std::vector<double>& dataInLeft, std::vector<double>& dataInRight,
        std::vector<double>& dataOutLeft, std::vector<double>& dataOutRight);

    void setNrofChns(int nrofchns) { m_nrofchns = nrofchns; };
    void setFeedback(double feedback) { m_feedback = feedback; };
    void setForward(double forward) { m_forward = forward; };
    void setBlend(double blend) { m_blend = blend; };
    void setLFOMinDelay(double mindelay) { m_lfoMinDelay = mindelay; setMinDelay(); };
    void setLFOMaxDelay(double maxdelay) { m_lfoMaxDelay = maxdelay; setMaxDelay(); };
    void setDelay(double delay) { m_nominalDelay = delay; computeSampleDelay(); };
    void setWidth(double width) {
        m_width = width; computeSampleDelay();
    };
    void setRate(float rate_Hz);
	void setWaveform(LFO::LFOFunctions waveform)
	{
		m_lfoLeft.setLFOFunction(waveform); 
		m_lfoRight.setLFOFunction(waveform);
	};
	void setPhase(double phase_degree)
	{
		m_phase = phase_degree / 180.0*M_PI; 
		m_lfoRight.setPhase(m_phase);
	};
	void setLowCutForward(double cutoff)
	{
		m_lowLeftForward.setCutoff(exp(cutoff));
		m_lowRightForward.setCutoff(exp(cutoff));
	}
	void setHighCutForward(double cutoff)
	{
		m_highLeftForward.setCutoff(exp(cutoff));
		m_highRightForward.setCutoff(exp(cutoff));
	}
	void setLowCutFeedback(double cutoff)
	{
		m_lowLeftFeedback.setCutoff(exp(cutoff));
		m_lowRightFeedback.setCutoff(exp(cutoff));
	}
	void setHighCutFeedback(double cutoff)
	{
		m_highLeftFeedback.setCutoff(exp(cutoff));
		m_highRightFeedback.setCutoff(exp(cutoff));
	}

private:
    double m_fs;
    const double m_absolutMaxDelay_ms = 250;
    int m_absolutDelay_samples;
    std::vector<double> m_delayMemoryLeft;
    std::vector<double> m_delayMemoryRight;

    double m_nominalDelay;
    double m_nominalDelay_samples;
    double m_width;

    int m_writeCounter;
    
    LFO m_lfoLeft;
    LFO m_lfoRight;
    std::vector<double> m_lfoDataLeft;
    std::vector<double> m_lfoDataRight;
    double m_lfoRate;
    double m_lfoMinDelay;
    double m_lfoMaxDelay;
	double m_phase;

    int m_nrofchns;
    double m_feedback;
    double m_forward;
    double m_blend;
    double m_lastDelayValueLeft;
    double m_lastDelayValueRight;
    
    void initChorus();
    void setMinDelay();
    void setMaxDelay();
    void computeSampleDelay();

	FirstOrderFilter m_lowLeftForward;
	FirstOrderFilter m_highLeftForward;
	FirstOrderFilter m_lowRightForward;
	FirstOrderFilter m_highRightForward;

	FirstOrderFilter m_lowLeftFeedback;
	FirstOrderFilter m_highLeftFeedback;
	FirstOrderFilter m_lowRightFeedback;
	FirstOrderFilter m_highRightFeedback;


};

#ifdef USE_JUCE
#include <JuceHeader.h>
#include "PresetHandler.h"
// several IDs to allow more than one instance
#define MAX_LFO_INSTANCES 4

const struct
{
	const std::string ID = "Delay";
	std::string name = "Delay";
	std::string unitName = "ms";
	float minValue = (0.5);
	float maxValue = (50.f);
	float defaultValue = (6.f);
}paramChorusDelay;
const struct
{
	const std::string ID = "Width";
	std::string name = "Delay Width ";
	std::string unitName = "%";
	float minValue = (0.f);
	float maxValue = (100.f);
	float defaultValue = (50.f);
}paramChorusWidth;

const struct
{
	const std::string ID = "DirectOut";
	std::string name = "Direct Out";
	std::string unitName = "";
	float minValue = (0.f);
	float maxValue = (1.5f);
	float defaultValue = (1.f);
}paramChorusDirectOut;

const struct
{
	const std::string ID = "Forward";
	std::string name = "Forward Gain";
	std::string unitName = "";
	float minValue = (0.f);
	float maxValue = (1.5f);
	float defaultValue = (0.707f);
}paramChorusForward;

const struct
{
	const std::string ID = "Feedback";
	std::string name = "Feedback Gain";
	std::string unitName = "";
	float minValue = (-0.999f);
	float maxValue = (0.999f);
	float defaultValue = (0.f);
}paramChorusFeedback;

const struct
{
	const std::string ID = "Phase";
	std::string name = "Phase L/R";
	std::string unitName = "";
	float minValue = (0.f);
	float maxValue = (180.f);
	float defaultValue = (90.f);
}paramChorusPhase;

const struct
{
	const std::string ID = "FFLow";
	std::string name = "Forward Lowpass";
	std::string unitName = "Hz";
	float minValue = log(100.f);
	float maxValue = log(18000.f);
	float defaultValue = log(4000.f);
}paramChorusFFLow;

const struct
{
	const std::string ID = "FFHigh";
	std::string name = "Forward Highass";
	std::string unitName = "Hz";
	float minValue = log(10.f);
	float maxValue = log(6000.f);
	float defaultValue = log(1500.f);
}paramChorusFFHigh;
const struct
{
	const std::string ID = "FBLow";
	std::string name = "Feedback Lowpass";
	std::string unitName = "Hz";
	float minValue = log(100.f);
	float maxValue = log(18000.f);
	float defaultValue = log(4000.f);
}paramChorusFBLow;

const struct
{
	const std::string ID = "FBHigh";
	std::string name = "Feedback Highass";
	std::string unitName = "Hz";
	float minValue = log(10.f);
	float maxValue = log(6000.f);
	float defaultValue = log(1500.f);
}paramChorusFBHigh;


class ChorusParameter
{
public:
	int addParameter(std::vector < std::unique_ptr<RangedAudioParameter>>& paramVector);
};

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
typedef AudioProcessorValueTreeState::ComboBoxAttachment ComboAttachment;
class ChorusParameterComponent : public Component
{
public:
	ChorusParameterComponent(AudioProcessorValueTreeState&);

	void paint(Graphics& g) override;
	void resized() override;
	std::function<void()> somethingChanged;


private:
	AudioProcessorValueTreeState& m_vts;

	Label m_delayLabel;
	Slider m_delaySlider;
	std::unique_ptr<SliderAttachment> m_delayAttachment;

	Label m_widthLabel;
	Slider m_widthSlider;
	std::unique_ptr<SliderAttachment> m_widthAttachment;

	Label m_forwardLabel;
	Slider m_forwardSlider;
	std::unique_ptr<SliderAttachment> m_forwardAttachment;

	Label m_ffLowLabel;
	Slider m_ffLowSlider;
	std::unique_ptr<SliderAttachment> m_ffLowAttachment;

	Label m_ffHighLabel;
	Slider m_ffHighSlider;
	std::unique_ptr<SliderAttachment> m_ffHighAttachment;

	Label m_feedbackLabel;
	Slider m_feedbackSlider;
	std::unique_ptr<SliderAttachment> m_feedbackAttachment;

	Label m_fbLowLabel;
	Slider m_fbLowSlider;
	std::unique_ptr<SliderAttachment> m_fbLowAttachment;

	Label m_fbHighLabel;
	Slider m_fbHighSlider;
	std::unique_ptr<SliderAttachment> m_fbHighAttachment;

	Label m_directLabel;
	Slider m_directSlider;
	std::unique_ptr<SliderAttachment> m_directAttachment;

	Label m_phaseLRLabel;
	Slider m_phaseLRSlider;
	std::unique_ptr<SliderAttachment> m_phaseLRAttachment;

};
#endif