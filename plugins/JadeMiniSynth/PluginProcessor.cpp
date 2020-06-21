/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JadeMiniSynthAudioProcessor::JadeMiniSynthAudioProcessor()
#ifndef JucePlugin_PreferredChannelConfigurations
     : AudioProcessor (BusesProperties()
                     #if ! JucePlugin_IsMidiEffect
                      #if ! JucePlugin_IsSynth
                       .withInput  ("Input",  AudioChannelSet::stereo(), true)
                      #endif
                       .withOutput ("Output", AudioChannelSet::stereo(), true)
                     #endif
                       )
#endif
{
    // Add Parameters (direct or by calling the addparameter functions)
    m_oscParam.addParameter(m_paramVector, 0);
    m_envParam.addParameter(m_paramVector, 0);
    m_moogParam.addParameter(m_paramVector);


    // Build VTS and PresetHandler
    m_parameterVTS = std::make_unique<AudioProcessorValueTreeState>(*this, nullptr, Identifier("SimpleChorusVTS"),
        AudioProcessorValueTreeState::ParameterLayout(m_paramVector.begin(), m_paramVector.end()));
    m_parameterVTS->state.setProperty("version", JucePlugin_VersionString, nullptr);

    m_presethandler.setAudioValueTreeState(m_parameterVTS.get());
    m_presethandler.loadAllUserPresets();

    for (auto kk = 0; kk < kNumberOfVoices; ++kk)
        //for (auto kk = 0; kk < 1; ++kk)
    {
        m_synth.addVoice(new JadeMiniVoice());
    }
    m_sound = new JadeMiniSound();
    //m_synth.addSound(*m_pSound);
    m_synth.addSound(m_sound);

    m_sound->m_osc1wave1 = m_parameterVTS->getRawParameterValue(paramOscWaveform1.ID[0]);
    m_sound->m_osc1wave2 = m_parameterVTS->getRawParameterValue(paramOscWaveform2.ID[0]);
    m_sound->m_osc1moddepth = m_parameterVTS->getRawParameterValue(paramOscModDepth.ID[0]);
    m_sound->m_osc1xfade = m_parameterVTS->getRawParameterValue(paramOscXFade.ID[0]);
    m_sound->m_osc1level = m_parameterVTS->getRawParameterValue(paramOscLevel.ID[0]);
    m_sound->m_osc1tunecoarse = m_parameterVTS->getRawParameterValue(paramOscTuneCoarse.ID[0]);
    m_sound->m_osc1tunefine = m_parameterVTS->getRawParameterValue(paramOscTuneFine.ID[0]);

    // ENvelope
    m_sound->m_env1Delay = m_parameterVTS->getRawParameterValue(paramEnvDelay.ID[0]);
    m_sound->m_env1Attack = m_parameterVTS->getRawParameterValue(paramEnvAttack.ID[0]);
    m_sound->m_env1Hold = m_parameterVTS->getRawParameterValue(paramEnvHold.ID[0]);
    m_sound->m_env1Decay = m_parameterVTS->getRawParameterValue(paramEnvDecay.ID[0]);
    m_sound->m_env1Sustain = m_parameterVTS->getRawParameterValue(paramEnvSustain.ID[0]);
    m_sound->m_env1Release = m_parameterVTS->getRawParameterValue(paramEnvRelease.ID[0]);

    // Filter
    m_sound->m_cutoff = m_parameterVTS->getRawParameterValue(paramCutoff.ID);
    m_sound->m_reso = m_parameterVTS->getRawParameterValue(paramResonance.ID);
    m_sound->m_modKeyboard = m_parameterVTS->getRawParameterValue(paramModKeyboard.ID);
    m_sound->m_modEnvelope = m_parameterVTS->getRawParameterValue(paramModEnvelope.ID);
    m_sound->m_modLfo = m_parameterVTS->getRawParameterValue(paramModLfo.ID);

}

JadeMiniSynthAudioProcessor::~JadeMiniSynthAudioProcessor()
{
}

//==============================================================================
const String JadeMiniSynthAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool JadeMiniSynthAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool JadeMiniSynthAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool JadeMiniSynthAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double JadeMiniSynthAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int JadeMiniSynthAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int JadeMiniSynthAudioProcessor::getCurrentProgram()
{
    return 0;
}

void JadeMiniSynthAudioProcessor::setCurrentProgram (int index)
{
}

const String JadeMiniSynthAudioProcessor::getProgramName (int index)
{
    if (index == 0)
        return "Init";

    return {};
}

void JadeMiniSynthAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void JadeMiniSynthAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    m_synth.prepareToPlay(sampleRate, samplesPerBlock);

}

void JadeMiniSynthAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool JadeMiniSynthAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
  #if JucePlugin_IsMidiEffect
    ignoreUnused (layouts);
    return true;
  #else
    // This is the place where you check if the layout is supported.
    // In this template code we only support mono or stereo.
    if (layouts.getMainOutputChannelSet() != AudioChannelSet::mono()
     && layouts.getMainOutputChannelSet() != AudioChannelSet::stereo())
        return false;

    // This checks if the input layout matches the output layout
   #if ! JucePlugin_IsSynth
    if (layouts.getMainOutputChannelSet() != layouts.getMainInputChannelSet())
        return false;
   #endif

    return true;
  #endif
}
#endif

void JadeMiniSynthAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedLock Sp(objectLock);
    int nrofsamples = buffer.getNumSamples();
    m_keyboardState.processNextMidiBuffer(midiMessages, 0, nrofsamples, true);

    auto bypass = getBypassParameter();


    if (buffer.getNumChannels() == 0)
        return;

    if (getBusCount(false) < 1)
        return;

    ScopedNoDenormals noDenormals;
    m_synth.renderNextBlock(buffer, midiMessages, 0, nrofsamples);
}

//==============================================================================
bool JadeMiniSynthAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* JadeMiniSynthAudioProcessor::createEditor()
{
    return new JadeMiniSynthAudioProcessorEditor (*this, *m_parameterVTS, m_presethandler);
}

//==============================================================================
void JadeMiniSynthAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
    auto state = m_parameterVTS->copyState();
    std::unique_ptr<XmlElement> xml(state.createXml());
    copyXmlToBinary(*xml, destData);

}

void JadeMiniSynthAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
    std::unique_ptr<XmlElement> xmlState(getXmlFromBinary(data, sizeInBytes));

    if (xmlState.get() != nullptr)
        if (xmlState->hasTagName(m_parameterVTS->state.getType()))
            m_parameterVTS->replaceState(ValueTree::fromXml(*xmlState));
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new JadeMiniSynthAudioProcessor();
}
