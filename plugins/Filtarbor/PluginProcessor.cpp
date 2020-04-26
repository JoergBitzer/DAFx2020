/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Parameter.h"

#define ORDER_OFF 0.05

//==============================================================================
FilarborAudioProcessor::FilarborAudioProcessor()
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
    m_chns = 2;
    m_fs = 48000.0;

    m_cutoffLowpass = exp(paramLpCutoff.defaultValue);
    m_orderLowpass = paramLpOrder.defaultValue;
    m_cutoffHighpass = exp(paramHpCutoff.defaultValue);
    m_orderHighpass = paramHpOrder.defaultValue;

    m_filterorder = 4.0;
    m_nrofcoefficients = m_filterorder + 1;
    m_b.resize(m_nrofcoefficients);
    m_a.resize(m_nrofcoefficients);
    std::fill(m_b.begin(), m_b.end(), 0.0);
    std::fill(m_a.begin(), m_a.end(), 0.0);
    m_b[0] = 1.0;
    m_a[0] = 1.0;
    m_filterdesigner.setOutputFilterOrder(m_filterorder, m_filterorder);

    m_lowpassFilter.resize(m_chns);
    m_highpassFilter.resize(m_chns);


    for (auto idx = 0; idx < m_lowpassFilter.size(); idx++)
    {
        m_lowpassFilter[idx].setCoeffs(m_b, m_a);
        m_lowpassFilter[idx].setXFadeSamples(100);
    }

    for (auto idx = 0; idx < m_highpassFilter.size(); idx++)
    {
        m_highpassFilter[idx].setXFadeSamples(100);
        m_highpassFilter[idx].setCoeffs(m_b, m_a);
    }

    // Parameter setzen
    // Lowpass
    m_paramVector.push_back(std::make_unique<AudioParameterFloat>(paramLpCutoff.ID,
        paramLpCutoff.name,
        NormalisableRange<float>(paramLpCutoff.minValue, paramLpCutoff.maxValue),
        paramLpCutoff.defaultValue,
        paramLpCutoff.unitName,
        AudioProcessorParameter::genericParameter,
        [](float value, int MaxLen) { value = int(exp(value)*10)*0.1;  return String(value, MaxLen); },
        [](const String& text) {return text.getFloatValue(); }));

    m_paramVector.push_back(std::make_unique<AudioParameterFloat>(paramLpOrder.ID,
        paramLpOrder.name,
        NormalisableRange<float>(paramLpOrder.minValue, paramLpOrder.maxValue),
        paramLpOrder.defaultValue,
        paramLpOrder.unitName,
        AudioProcessorParameter::genericParameter,
        [](float value, int MaxLen)
        {
            if (value >= ORDER_OFF) 
                return String(value, MaxLen);
            else 
                return String("Off");
        },
        [](const String& text)
        {
            return text.getFloatValue();
        }
        ));

    // Highpass
    m_paramVector.push_back(std::make_unique<AudioParameterFloat>(paramHpCutoff.ID,
        paramHpCutoff.name,
        NormalisableRange<float>(paramHpCutoff.minValue, paramHpCutoff.maxValue),
        paramHpCutoff.defaultValue,
        paramHpCutoff.unitName,
        AudioProcessorParameter::genericParameter,
        [](float value, int MaxLen) { value = int(exp(value) * 10) * 0.1;  return String(value, MaxLen); },
        [](const String& text) {return text.getFloatValue(); }));

    m_paramVector.push_back(std::make_unique<AudioParameterFloat>(paramHpOrder.ID,
        paramHpOrder.name,
        NormalisableRange<float>(paramHpOrder.minValue, paramHpOrder.maxValue),
        paramHpOrder.defaultValue,
        paramHpOrder.unitName,
        AudioProcessorParameter::genericParameter,
        [](float value, int MaxLen)
        {
            if (value >= ORDER_OFF)
                return String(value, MaxLen);
            else
                return String("Off");
        },
        [](const String& text)
        {
            return text.getFloatValue();
        }
        ));
    // alle weiteren Parameter mit push_back => Liste von Zeigern auf Parameter

    m_parameterVTS = std::make_unique<AudioProcessorValueTreeState>(*this, nullptr, Identifier("FiltarborVTS"),
        AudioProcessorValueTreeState::ParameterLayout(m_paramVector.begin(), m_paramVector.end()));
}

FilarborAudioProcessor::~FilarborAudioProcessor()
{
}

//==============================================================================
const String FilarborAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool FilarborAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool FilarborAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool FilarborAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double FilarborAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int FilarborAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int FilarborAudioProcessor::getCurrentProgram()
{
    return 0;
}

void FilarborAudioProcessor::setCurrentProgram (int index)
{
}

const String FilarborAudioProcessor::getProgramName (int index)
{
    return {};
}

void FilarborAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void FilarborAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
    m_fs = sampleRate;
    computeLowpass();
    computeHighpass();
    m_data.resize(samplesPerBlock);
}

void FilarborAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool FilarborAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void FilarborAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
    auto totalNumInputChannels  = getTotalNumInputChannels();
    auto totalNumOutputChannels = getTotalNumOutputChannels();

    // In case we have more outputs than inputs, this code clears any output
    // channels that didn't contain input data, (because these aren't
    // guaranteed to be empty - they may contain garbage).
    // This is here to avoid people getting screaming feedback
    // when they first compile a plugin, but obviously you don't need to keep
    // this code if your algorithm always overwrites all the output channels.
    for (auto i = totalNumInputChannels; i < totalNumOutputChannels; ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    // This is the place where you'd normally do the guts of your plugin's
    // audio processing...
    // Make sure to reset the state if your inner loop is processing
    // the samples and the outer loop is handling the channels.
    // Alternatively, you can process the samples with the channels
    // interleaved by keeping the same state.
    for (int channel = 0; channel < totalNumInputChannels; ++channel)
    {
        auto* channelData = buffer.getWritePointer (channel);

        // ..do something to the data...
        for (auto idx = 0; idx < buffer.getNumSamples(); idx++)
        {
            m_data[idx] = channelData[idx];
        }
        m_protect.enter();
        m_lowpassFilter[channel].processDataTV(m_data, m_data);
        m_protect.exit();

        m_protect.enter();
        m_highpassFilter[channel].processDataTV(m_data, m_data);
        m_protect.exit();

        for (auto idx = 0; idx < buffer.getNumSamples(); idx++)
        {
            channelData[idx] = m_data[idx];
        }
    }
}

//==============================================================================
bool FilarborAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* FilarborAudioProcessor::createEditor()
{
    return new FilarborAudioProcessorEditor (*this, *m_parameterVTS);
}

//==============================================================================
void FilarborAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
}

void FilarborAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    // You should use this method to restore your parameters from this memory block,
    // whose contents will have been created by the getStateInformation() call.
}

void FilarborAudioProcessor::computeLowpass()
{
    //Hier Filterdesign
    // Lowpass
    if (m_orderLowpass >= ORDER_OFF)
        m_filterdesigner.getCoefficients(m_b, m_a, m_fs, m_cutoffLowpass, m_orderLowpass, FreeOrderLowHighpassFilter::FilterType::lowpass);
    else
    {
        std::fill(m_b.begin(), m_b.end(), 0.0);
        std::fill(m_a.begin(), m_a.end(), 0.0);
        m_b[0] = 1.0;
        m_a[0] = 1.0;
    }
    //Setzen der Koeffizienten in den Filtern
    m_protect.enter();
    for (auto idx = 0; idx < m_lowpassFilter.size(); idx++)
    {
        m_lowpassFilter[idx].setCoeffs(m_b, m_a);
    }
    m_protect.exit();

}

void FilarborAudioProcessor::computeHighpass()
{
    // Highpass
    if (m_orderHighpass >= ORDER_OFF)
        m_filterdesigner.getCoefficients(m_b, m_a, m_fs, m_cutoffHighpass, m_orderHighpass, FreeOrderLowHighpassFilter::FilterType::highpass);
    else
    {
        std::fill(m_b.begin(), m_b.end(), 0.0);
        std::fill(m_a.begin(), m_a.end(), 0.0);
        m_b[0] = 1.0;
        m_a[0] = 1.0;
    }
    //Setzen der Koeffizienten in den Filtern
    m_protect.enter();
    for (auto idx = 0; idx < m_highpassFilter.size(); idx++)
    {
        m_highpassFilter[idx].setCoeffs(m_b, m_a);
    }
    m_protect.exit();
}

//==============================================================================
// This creates new instances of the plugin..
AudioProcessor* JUCE_CALLTYPE createPluginFilter()
{
    return new FilarborAudioProcessor();
}
