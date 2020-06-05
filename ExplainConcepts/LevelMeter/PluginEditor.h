/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "PoorManLevelMeter.h"

//==============================================================================
/**
*/
class DatenAnzeigeAudioProcessorEditor  : public AudioProcessorEditor, Timer
{
public:
    DatenAnzeigeAudioProcessorEditor (DatenAnzeigeAudioProcessor&);
    ~DatenAnzeigeAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    void timerCallback();
private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    DatenAnzeigeAudioProcessor& m_processor;
    TextButton m_textdisplay;
    PoorManLevelMeter m_level;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DatenAnzeigeAudioProcessorEditor)
};
