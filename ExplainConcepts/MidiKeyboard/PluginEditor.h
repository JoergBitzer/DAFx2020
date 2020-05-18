/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class NanoAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    NanoAudioProcessorEditor (NanoAudioProcessor&);
    ~NanoAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

    void processMidiBuffer(MidiBuffer& buffer, int startSample, int numSamples, bool addEvents)
    {
        m_MidiState.processNextMidiBuffer(buffer, startSample, numSamples, addEvents);
    };

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    NanoAudioProcessor& m_processor;
    MidiKeyboardState m_MidiState;
    MidiKeyboardComponent m_MidiKeyboard;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NanoAudioProcessorEditor)
};
