/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include "PluginProcessor.h"
#include <PresetHandler.h>
#include <JadeLookAndFeel.h>

#include "WavetableOszi.h"
#include "Envelope.h"
//==============================================================================
/**
*/
class JadeMiniSynthAudioProcessorEditor  : public AudioProcessorEditor
{
public:
    JadeMiniSynthAudioProcessorEditor(JadeMiniSynthAudioProcessor& , AudioProcessorValueTreeState&, PresetHandler&);
    ~JadeMiniSynthAudioProcessorEditor();

    //==============================================================================
    void paint (Graphics&) override;
    void resized() override;

private:

    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    JadeLookAndFeel m_jade;
    // This reference is provided as a quick way for your editor to
    // access the processor object that created it.
    AudioProcessorValueTreeState& m_vts;
    PresetComponent m_presetGUI;
    MidiKeyboardComponent m_keyboard;
        
    JadeMiniSynthAudioProcessor& m_processor;

    // GUI Elements
    // Osci
    OscParameterComponent m_osc1;
    EnvelopeParameterComponent m_env1;


    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JadeMiniSynthAudioProcessorEditor)
};
