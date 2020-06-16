/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "LFO/LFO.h"
#include "SimpleChorus.h"
#include "PresetHandler.h"

//==============================================================================
/**
*/
class SimpleChorusAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    SimpleChorusAudioProcessor();
    ~SimpleChorusAudioProcessor();

    //==============================================================================
    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override;

   #ifndef JucePlugin_PreferredChannelConfigurations
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;
   #endif

    void processBlock (AudioBuffer<float>&, MidiBuffer&) override;

    //==============================================================================
    AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override;

    //==============================================================================
    const String getName() const override;

    bool acceptsMidi() const override;
    bool producesMidi() const override;
    bool isMidiEffect() const override;
    double getTailLengthSeconds() const override;

    //==============================================================================
    int getNumPrograms() override;
    int getCurrentProgram() override;
    void setCurrentProgram (int index) override;
    const String getProgramName (int index) override;
    void changeProgramName (int index, const String& newName) override;

    //==============================================================================
    void getStateInformation (MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;
	void numBusesChanged() override;
private:
	int m_nrofbusses;
    //==============================================================================
		//Parameterhandling
	SimpleChorus m_chorus;
	std::vector<double> m_dataLeft;
	std::vector<double> m_dataRight;
	std::vector<double> m_dataOutLeft;
	std::vector<double> m_dataOutRight;

	std::unique_ptr<AudioProcessorValueTreeState> m_parameterVTS;
	std::vector < std::unique_ptr<RangedAudioParameter>> m_paramVector;

	LFOParameter m_lfoparams;
	ChorusParameter m_chorusparams;

	std::atomic <float>* m_rate;
	float m_oldrate;
	std::atomic <float>* m_waveform;
	float m_oldwaveform;

	std::atomic <float>* m_delay;
	float m_olddelay;
	std::atomic <float>* m_width;
	float m_oldwidth;
	std::atomic <float>* m_directout;
	float m_olddirectout;
	std::atomic <float>* m_forward;
	float m_oldforward;
	std::atomic <float>* m_feedback;
	float m_oldfeedback;
	std::atomic <float>* m_phase;
	float m_oldphase;

	std::atomic <float>* m_lowForward;
	float m_oldlowForward;
	std::atomic <float>* m_highForward;
	float m_oldhighForward;

	std::atomic <float>* m_lowFeedback;
	float m_oldlowFeedback;
	std::atomic <float>* m_highFeedback;
	float m_oldhighFeedback;

	PresetHandler m_presets;
	CriticalSection objectLock;

	JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleChorusAudioProcessor)
};
/*
bool isBusesLayoutSupported(const BusesLayout& layouts) const override
{
	// the sidechain can take any layout, the main bus needs to be the same on the input and output
	return layouts.getMainInputChannelSet() == layouts.getMainOutputChannelSet();
	// && ! layouts.getMainInputChannelSet().isDisabled(); <- remove this
}*/