/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>
#include <PresetHandler.h>
#include "JadeMiniSound.h"
#include "JadeMiniSynth.h"
#include "JadeMiniVoice.h"
#include "WavetableOszi.h"
#include "Envelope.h"
#include "MoogLadder.h"

#include "WhiteNoiseGenerator.h"
#include "ColoredNoiseGenerator.h"

//==============================================================================
/**
*/
class JadeMiniSynthAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    JadeMiniSynthAudioProcessor();
    ~JadeMiniSynthAudioProcessor();

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
    MidiKeyboardState m_keyboardState;

private:
    // paramter handling

    std::unique_ptr<AudioProcessorValueTreeState> m_parameterVTS;
    std::vector < std::unique_ptr<RangedAudioParameter>> m_paramVector;

    PresetHandler m_presethandler;
    CriticalSection objectLock;

    // Synth Voice Sound
    static const int kNumberOfVoices = 16;
    JadeMiniSynth m_synth;
    JadeMiniSound* m_sound;

    std::unique_ptr<JadeMiniSound> m_pSound;

    // parameter of the different modules

    OscParameter m_oscParam;
    EnvelopeParameter m_envParam;
    MoogLadderParameter m_moogParam;

    WhiteNoiseParameter m_whitenoiseParam;
    ColoredNoiseParameter m_colorednoiseParam;


    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (JadeMiniSynthAudioProcessor)
};
