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
    m_voiceParam.addParameter(m_paramVector);

    m_oscParam.addParameter(m_paramVector, 0);
    m_oscParam.addParameter(m_paramVector, 1);
    m_envParam.addParameter(m_paramVector, 0);
    m_env2Param.addParameter(m_paramVector, 1);

    m_lfo1Param.addParameter(m_paramVector, 0);
    m_lfo2Param.addParameter(m_paramVector, 1);


    m_moogParam.addParameter(m_paramVector);

    m_whitenoiseParam.addParameter(m_paramVector);
    m_colorednoiseParam.addParameter(m_paramVector);

    // Build VTS and PresetHandler
    m_parameterVTS = std::make_unique<AudioProcessorValueTreeState>(*this, nullptr, Identifier("SimpleChorusVTS"),
        AudioProcessorValueTreeState::ParameterLayout(m_paramVector.begin(), m_paramVector.end()));
    m_parameterVTS->state.setProperty("version", JucePlugin_VersionString, nullptr);

    m_presethandler.setAudioValueTreeState(m_parameterVTS.get());
    m_presethandler.loadAllUserPresets();

    m_NumberOfVoices = 8;
    m_oldNumberOfVoices = 8;
    for (auto kk = 0; kk < m_NumberOfVoices; ++kk)
    {
        m_synth.addVoice(new JadeMiniVoice());
    }
    m_sound = new JadeMiniSound();
    //m_synth.addSound(*m_pSound);
    m_synth.addSound(m_sound);

    // Global
    m_sound->m_voiceNrOfVoices = m_parameterVTS->getRawParameterValue(paramGlobalNrOfVoices.ID);
    m_sound->m_voicePortamentoTime = m_parameterVTS->getRawParameterValue(paramGlobalPortamentoTime.ID);
    m_sound->m_voiceTuneA0 = m_parameterVTS->getRawParameterValue(paramGlobalTuneA0.ID);

    // Osc1
    int oscnr = 0;
    m_sound->m_osc1wave1 = m_parameterVTS->getRawParameterValue(paramOscWaveform1.ID[oscnr]);
    m_sound->m_osc1wave2 = m_parameterVTS->getRawParameterValue(paramOscWaveform2.ID[oscnr]);
    m_sound->m_osc1moddepth = m_parameterVTS->getRawParameterValue(paramOscModDepth.ID[oscnr]);
    m_sound->m_osc1xfade = m_parameterVTS->getRawParameterValue(paramOscXFade.ID[oscnr]);
    m_sound->m_osc1level = m_parameterVTS->getRawParameterValue(paramOscLevel.ID[oscnr]);
    m_sound->m_osc1tunecoarse = m_parameterVTS->getRawParameterValue(paramOscTuneCoarse.ID[oscnr]);
    m_sound->m_osc1tunefine = m_parameterVTS->getRawParameterValue(paramOscTuneFine.ID[oscnr]);

    oscnr = 1;
    m_sound->m_osc2wave1 = m_parameterVTS->getRawParameterValue(paramOscWaveform1.ID[oscnr]);
    m_sound->m_osc2wave2 = m_parameterVTS->getRawParameterValue(paramOscWaveform2.ID[oscnr]);
    m_sound->m_osc2moddepth = m_parameterVTS->getRawParameterValue(paramOscModDepth.ID[oscnr]);
    m_sound->m_osc2xfade = m_parameterVTS->getRawParameterValue(paramOscXFade.ID[oscnr]);
    m_sound->m_osc2level = m_parameterVTS->getRawParameterValue(paramOscLevel.ID[oscnr]);
    m_sound->m_osc2tunecoarse = m_parameterVTS->getRawParameterValue(paramOscTuneCoarse.ID[oscnr]);
    m_sound->m_osc2tunefine = m_parameterVTS->getRawParameterValue(paramOscTuneFine.ID[oscnr]);

    // ENvelope 1 (VCA)
    int envnr = 0;
    m_sound->m_env1Delay = m_parameterVTS->getRawParameterValue(paramEnvDelay.ID[envnr]);
    m_sound->m_env1Attack = m_parameterVTS->getRawParameterValue(paramEnvAttack.ID[envnr]);
    m_sound->m_env1Hold = m_parameterVTS->getRawParameterValue(paramEnvHold.ID[envnr]);
    m_sound->m_env1Decay = m_parameterVTS->getRawParameterValue(paramEnvDecay.ID[envnr]);
    m_sound->m_env1Sustain = m_parameterVTS->getRawParameterValue(paramEnvSustain.ID[envnr]);
    m_sound->m_env1Release = m_parameterVTS->getRawParameterValue(paramEnvRelease.ID[envnr]);
    m_sound->m_env1Level = m_parameterVTS->getRawParameterValue(paramEnvLevel.ID[envnr]);
    
    envnr = 1;
    // ENvelope 2 (Filter Cutoff)
    m_sound->m_env2Delay = m_parameterVTS->getRawParameterValue(paramEnvDelay.ID[envnr]);
    m_sound->m_env2Attack = m_parameterVTS->getRawParameterValue(paramEnvAttack.ID[envnr]);
    m_sound->m_env2Hold = m_parameterVTS->getRawParameterValue(paramEnvHold.ID[envnr]);
    m_sound->m_env2Decay = m_parameterVTS->getRawParameterValue(paramEnvDecay.ID[envnr]);
    m_sound->m_env2Sustain = m_parameterVTS->getRawParameterValue(paramEnvSustain.ID[envnr]);
    m_sound->m_env2Release = m_parameterVTS->getRawParameterValue(paramEnvRelease.ID[envnr]);
    m_sound->m_env2Level = m_parameterVTS->getRawParameterValue(paramEnvLevel.ID[envnr]);
    m_sound->m_env2Invert = m_parameterVTS->getRawParameterValue(paramEnvInvert.ID[envnr]);

    int lfonr = 0;
    m_sound->m_lfo1level = m_parameterVTS->getRawParameterValue(paramLFOLevel.ID[lfonr]);
    m_sound->m_lfo1rate = m_parameterVTS->getRawParameterValue(paramLFORate.ID[lfonr]);
    m_sound->m_lfo1waveform = m_parameterVTS->getRawParameterValue(paramLFOWaveform.ID[lfonr]);
    lfonr = 1;
    m_sound->m_lfo2level = m_parameterVTS->getRawParameterValue(paramLFOLevel.ID[lfonr]);
    m_sound->m_lfo2rate = m_parameterVTS->getRawParameterValue(paramLFORate.ID[lfonr]);
    m_sound->m_lfo2waveform = m_parameterVTS->getRawParameterValue(paramLFOWaveform.ID[lfonr]);
    
    // Filter
    m_sound->m_cutoff = m_parameterVTS->getRawParameterValue(paramCutoff.ID);
    m_sound->m_reso = m_parameterVTS->getRawParameterValue(paramResonance.ID);
    m_sound->m_modKeyboard = m_parameterVTS->getRawParameterValue(paramModKeyboard.ID);
    m_sound->m_modDepth = m_parameterVTS->getRawParameterValue(paramModDepth.ID);

    // Noise Source
    m_sound->m_whiteLevel = m_parameterVTS->getRawParameterValue(paramNoiseLevel.ID);
    m_sound->m_whiteGraininess = m_parameterVTS->getRawParameterValue(paramNoiseGraininess.ID);
    m_sound->m_coloredLow = m_parameterVTS->getRawParameterValue(paramNoiseColorLow.ID);
    m_sound->m_coloredHigh = m_parameterVTS->getRawParameterValue(paramNoiseColorHigh.ID);

    
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
    m_fs = sampleRate;
    m_maxMaxBlockSize = samplesPerBlock;
    m_synth.prepareToPlay(m_fs, m_maxMaxBlockSize);

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

    int nrofvoices = *(m_sound->m_voiceNrOfVoices);
    
    if (nrofvoices != m_oldNumberOfVoices)
    {
        m_NumberOfVoices = nrofvoices;
        m_oldNumberOfVoices = nrofvoices;
        m_synth.clearVoices();
        for (auto kk = 0; kk < m_NumberOfVoices; ++kk)
        {
            m_synth.addVoice(new JadeMiniVoice());
        }
        m_synth.prepareToPlay(m_fs, m_maxMaxBlockSize);

    }


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
