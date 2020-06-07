/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"

typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

//==============================================================================
/**
*/
class NanoAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    NanoAudioProcessorEditor (NanoAudioProcessor&, AudioProcessorValueTreeState&);
    ~NanoAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    NanoAudioProcessor& m_processor;
    AudioProcessorValueTreeState& m_vts;

    MidiKeyboardComponent m_MidiKeyboard;
    
    Slider m_cutoffSlider;
    std::unique_ptr<SliderAttachment> m_cutoffAttachment;
    
    Slider m_resoSlider;
    std::unique_ptr<SliderAttachment> m_resoAttachment;


    // Image m_backgroundImage;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NanoAudioProcessorEditor)
};
