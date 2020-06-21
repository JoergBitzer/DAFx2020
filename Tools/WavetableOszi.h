/*****************************************************************//**
 * \file   WavetableOszi.h
 * \brief  Wavetable oscillator without aliasing below 16kHz
 * 
 * 
 * \author Bitzer
 * \date   April 2020
***********************************************************************/
#pragma once
#define _USE_MATH_DEFINES

#include <cmath>
#include <vector>
#include "FFT.h"
/**
 * \class  WavetableOsci class
 */
class WavetableOszi
{
public:
	/**
	 * \brief the constructor without arguments (samplerate is often not known )
	*/
	WavetableOszi();
	~WavetableOszi();

	/**
	 * method to add a new waveform (always appended at the end)
	 * 
	 * \param waveform is a vector containing the actual waveform (should hae 512 entries)
	 * \return 0 if accepted and -1 if not 
	 */
	int addF0Wavetable(const std::vector<double>& waveform);
	/**
	 * method to get data of a given length (size of vector)
	 * 
	 * \param data vector as reference
	 * \return 
	 */
	int getData(std::vector<double>& data);
	void setSamplerate(double samplerate); /**< set the samplerate for the internal tablef0*/
	void setFrequency(double freq); /**< set the frequency for the generated data*/
	void setWaveform(int newwaveformnumber); /**< set the current generated waveform */
	void setWaveformXFade(double xfadefactor); /**< set a factor to mix two waveforms range 0...1*/
	void setSecondWaveform(int waveformnumber); /**< set the second waveform*/
	void resetPhase(); /**< reset the phase to start at the beginning of the internal waveform representation*/
	void changeFrequencyRelative(double change_semitones);
	void setPortamentoTime(double portaTime_ms);

	void setLevel(double level);

	void setTuneCoarse(float semitones);
	void setTuneFine(float cents);
private:
	double m_fs; ///< sample rate
	double m_freq; ///< frequency
	double m_f0Table; ///< the internal fundamental frequency of the table
	double m_fsmin;  ///< an assumed minimal sampling rate

	int m_nrOfWaveforms; ///< how many waveforms are available
	int m_nrOfWavetables; ///< the number of wavetables necessary for aliasing free generation
	int m_lenOfWavetable; ///< the internal length of the wavetable set to 512
	int m_curWaveform; ///< the current waveform (unfortunately no list available)
	int m_curWavetable; ///< the used wavetable for a given frequency
	int m_interpolationOrder; ///< for order of the used interpolation algorithm (1 for linear interpolation ) 
	/**
	 * memory for the wavetables 3 dimensions: first the waveforms, second the wavetables and thirs the actual data
	 */
	std::vector <std::vector<std::vector<double>>> m_wavetable;

	double m_incStep; ///< the internal stepsize to walk through the wavetabel to generate a given frequency
	double m_curPos; ///< the current position in the wavetable

	void computeFullWavetable(); ///< helper function to generate the wavetables with reduced harmonics
	spectrum m_fft; ///< a fft class to compute the wavetables
	std::vector<double> m_real;
	std::vector<double> m_imag;
	std::vector<double> m_newwave;
	int m_semitonespertable; ///< controls the number of necessary wavetables

	double m_waveformXFadeFactor; ///< the internal crossfade factor between two waveforms
	int m_secondWaveform;
	/**
	 * Internal helper function to set the wavetable
	 * 
	 * \param newwaveform : the new waveform to be copied in the internal memory
	 * \param nrWaveform : defines the waveform it belongs to
	 * \param nrWavetable : defines the wavetable it should go
	 */
	void copyNewWavetable(std::vector<double> newwaveform, int nrWaveform, int nrWavetable);
	void computeIncStep(); ///< internal helper for the computation of the inc step
	double m_incStepModifier;
	int m_portamentoCounter;
	int m_portamentoTime_samples;
	double m_portamentoFactor;

	double m_tunecoarse;
	double m_tunefine;
	double m_tunefinal;
	void computeTuning();

	double m_level;
};

#ifdef USE_JUCE
#include <JuceHeader.h>
// several IDs to allow more than one instance
#define MAX_OSC_INSTANCES 4
const struct
{
	const std::string ID[MAX_OSC_INSTANCES] = { "Osc1Waveform1", "Osc2Waveform1", "Osc3Waveform1", "Osc4Waveform1" };
	std::string name = "Waveform1";
	std::string unitName = "";
	const char* Choices[4] = { "sawtooth","rectangular","triangle"};
	int defaultIndex = 0;
}paramOscWaveform1;
const struct
{
	const std::string ID[MAX_OSC_INSTANCES] = { "Osc1Waveform2", "Osc2Waveform2", "Osc3Waveform2", "Osc4Waveform2" };
	std::string name = "Waveform2";
	std::string unitName = "";
	const char* Choices[4] = { "sawtooth","rectangular","triangle" };
	int defaultIndex = 1;
}paramOscWaveform2;


const struct
{
	const std::string ID[MAX_OSC_INSTANCES] = { "Osc1CoarseTuning", "Osc2CoarseTuning", "Osc3CoarseTuning", "Osc4CoarseTuning" };
	std::string name = "Tune (Coarse)";
	std::string unitName = "semitones";
	float minValue = -36.f;
	float maxValue = +36.f;
	float defaultValue = 0.f;
}paramOscTuneCoarse;

const struct
{
	const std::string ID[MAX_OSC_INSTANCES] = { "Osc1FineTuning", "Osc2FineTuning", "Osc3FineTuning", "Osc4FineTuning" };
	std::string name = "Tune (Fine)";
	std::string unitName = "cents";
	float minValue = -50.f;
	float maxValue = +50.f;
	float defaultValue = 0.f;
}paramOscTuneFine;

const struct
{
	const std::string ID[MAX_OSC_INSTANCES] = { "Osc1XFade", "Osc2XFade", "Osc3XFade", "Osc4XFade" };
	std::string name = "X Fade";
	std::string unitName = "%";
	float minValue = 0.f;
	float maxValue = 100.f;
	float defaultValue = 0.f;
}paramOscXFade;

const struct
{
	const std::string ID[MAX_OSC_INSTANCES] = { "Osc1Level", "Osc2Level", "Osc3Level", "Osc4Level" };
	std::string name = "Level";
	std::string unitName = "dB";
	float minValue = -90.f;
	float maxValue = 10.f;
	float defaultValue = 0.f;
}paramOscLevel;

const struct
{
	const std::string ID[MAX_OSC_INSTANCES] = { "Osc1ModDepth", "Osc2ModDepth", "Osc3ModDepth", "Osc4ModDepth" };
	std::string name = "Modulation Depth";
	std::string unitName = "%";
	float minValue = -100.f;
	float maxValue = 100.f;
	float defaultValue = 0.f;
}paramOscModDepth;

class OscParameter
{
public:
	int addParameter(std::vector < std::unique_ptr<RangedAudioParameter>>& paramVector, int instance);
};
typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
typedef AudioProcessorValueTreeState::ComboBoxAttachment ComboAttachment;

class OscParameterComponent : public Component
{
public:
	OscParameterComponent(AudioProcessorValueTreeState&, int index, const String& lfoName);

	void paint(Graphics& g) override;
	void resized() override;
	std::function<void()> somethingChanged;

private:
	AudioProcessorValueTreeState& m_vts;
	int m_index;
 	String m_name;

// comboboxen
	Label m_osc1waveform1Label;
	ComboBox m_osc1waveform1Combo;
	std::unique_ptr<ComboAttachment> m_osc1waveform1Attachment;
	
	Label m_osc1waveform2Label;
	ComboBox m_osc1waveform2Combo;
	std::unique_ptr<ComboAttachment> m_osc1waveform2Attachment;

	Label m_osc1xfadeLabel;
	Slider m_osc1xfadeSlider;
	std::unique_ptr<SliderAttachment> m_osc1xfadeAttachment;
// Tuning
	Label m_osc1tunecoarseLabel;
	Slider m_osc1tunecoarseSlider;
	std::unique_ptr<SliderAttachment> m_osc1tunecoarseAttachment;
	
	Label m_osc1tunefineLabel;
	Slider m_osc1tunefineSlider;
	std::unique_ptr <SliderAttachment> m_osc1tunefineAttachment;
// Level
	Label m_osc1levelLabel;
	Slider m_osc1levelSlider;
	std::unique_ptr <SliderAttachment> m_osc1levelAttachment;



// ModDepth fehlt
};

/*
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
	LFOParameterComponent(AudioProcessorValueTreeState&, int index, const String& lfoName);
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
	AudioProcessorValueTreeState& m_vts;
	LFOComponentStyle m_style;
};
//*/
#endif
