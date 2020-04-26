/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include "../JuceLibraryCode/JuceHeader.h"
#include "PluginProcessor.h"

//==============================================================================
/**
*/
class GuibasicsAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    GuibasicsAudioProcessorEditor (GuibasicsAudioProcessor&);
    ~GuibasicsAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    GuibasicsAudioProcessor& m_processor;
    Slider m_slider;
    Slider m_secondslider;
    TextButton m_button;
    ComboBox m_combobox;
    void changeColorCombo();
    void changeColorSlider();
    void changeButton();
    float m_red;
    float m_green;
    float m_blue;

    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GuibasicsAudioProcessorEditor)
};
