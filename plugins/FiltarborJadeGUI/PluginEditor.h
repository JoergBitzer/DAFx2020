/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include "JadeLookAndFeel.h"
typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;

//==============================================================================
/**
*/
class FilarborAudioProcessorEditor  : public AudioProcessorEditor, private Slider::Listener
{
public:
    FilarborAudioProcessorEditor (FilarborAudioProcessor&, AudioProcessorValueTreeState& );
    ~FilarborAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    JadeLookAndFeel m_jadeLAF;

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    FilarborAudioProcessor& m_processor;
    AudioProcessorValueTreeState& m_paramVTS;

    // Lowpass
    Label m_cutoffLpLabel;
    Slider m_cutoffLpSlider;
    std::unique_ptr<SliderAttachment> m_cutoffLpAttachment;

    Label m_orderLpLabel;
    Slider m_orderLpSlider;
    std::unique_ptr<SliderAttachment> m_orderLpAttachment;
    
    // Highpas
    Label m_cutoffHpLabel;
    Slider m_cutoffHpSlider;
    std::unique_ptr<SliderAttachment> m_cutoffHpAttachment;

    Label m_orderHpLabel;
    Slider m_orderHpSlider;
    std::unique_ptr<SliderAttachment> m_orderHpAttachment;

    void sliderValueChanged(Slider* slider) override;
 

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilarborAudioProcessorEditor)
};
