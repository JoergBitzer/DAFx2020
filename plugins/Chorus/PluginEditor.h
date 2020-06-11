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

//==============================================================================
/**
*/
class SimpleChorusAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    SimpleChorusAudioProcessorEditor (SimpleChorusAudioProcessor&, AudioProcessorValueTreeState&);
    ~SimpleChorusAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    SimpleChorusAudioProcessor& m_processor;
	AudioProcessorValueTreeState& m_vts;

	LFOParameterComponent m_lfo1params;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SimpleChorusAudioProcessorEditor)
};
