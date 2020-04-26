/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#pragma once

#include <vector>
#include <JuceHeader.h>

#include "FreeOrderLowHighpassFilter.h"
#include "GeneralIR.h"

//==============================================================================
/**
*/
class FilarborAudioProcessor  : public AudioProcessor
{
public:
    //==============================================================================
    FilarborAudioProcessor();
    ~FilarborAudioProcessor();

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

    void setCutoffLowpass(float cutoff)
    { m_cutoffLowpass = cutoff; computeLowpass(); };

    void setOrderLowpass(float order)
    {
        m_orderLowpass = order;
        computeLowpass();
    }

    void setCutoffHighpass(float cuttoff)
    {
        m_cutoffHighpass = cuttoff; computeHighpass(); 
    }

    void setOrderHighpass(float order)
    {
        m_orderHighpass = order;
        computeHighpass();
    }

private:
    std::vector<double> m_data;

    int m_filterorder;
    int m_nrofcoefficients;
    std::vector<double> m_b;
    std::vector<double> m_a;
    
    double m_fs;
    int m_chns;
    double m_cutoffLowpass;
    double m_orderLowpass;

    double m_cutoffHighpass;
    double m_orderHighpass;

    FreeOrderLowHighpassFilter m_filterdesigner;
    std::vector<GeneralIR> m_lowpassFilter;
    std::vector<GeneralIR> m_highpassFilter;
    void computeLowpass();
    void computeHighpass();
    CriticalSection m_protect;

    //Parameterhandling
    std::unique_ptr<AudioProcessorValueTreeState> m_parameterVTS;
    std::vector < std::unique_ptr<RangedAudioParameter>> m_paramVector;

    //==============================================================================
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FilarborAudioProcessor)
};
