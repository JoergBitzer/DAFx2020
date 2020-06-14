/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"
#include "LFO/LFO.h"
#include "SimpleChorus.h"
#include "PresetHandler.h"

//==============================================================================
/**
*/
class SimpleChorusAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    SimpleChorusAudioProcessorEditor (SimpleChorusAudioProcessor&, AudioProcessorValueTreeState&, PresetHandler &);
    ~SimpleChorusAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SimpleChorusAudioProcessor& m_processor;
	AudioProcessorValueTreeState& m_vts;
	PresetComponent m_presetGUI;

	LFOParameterComponent m_lfo1params;
	ChorusParameterComponent m_chorusparams;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleChorusAudioProcessorEditor)
};
