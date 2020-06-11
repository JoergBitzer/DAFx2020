/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "PresetHandler.h"

//==============================================================================
/**
*/
typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

class NanoAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    NanoAudioProcessorEditor (NanoAudioProcessor&, AudioProcessorValueTreeState&, PresetHandler& );
    ~NanoAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;
    Image backgroundImage;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    NanoAudioProcessor& m_processor;
	AudioProcessorValueTreeState& m_paramVTS;
    MidiKeyboardComponent m_keyboard;

	Slider m_cutoffSlider;
	std::unique_ptr<SliderAttachment> m_cutoffAttachment;

	Slider m_resonanceSlider;
	std::unique_ptr<SliderAttachment> m_resonanceAttachment;

	PresetComponent m_presetGUI;

//    ImageComponent backgroundImage;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NanoAudioProcessorEditor)
};


// REsizing ansehen
// https://forum.juce.com/t/resizing-scaling-issues-in-ableton-vst3-with-hi-dpi-resolution-on-windows/35096