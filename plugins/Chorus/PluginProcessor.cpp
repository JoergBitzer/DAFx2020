/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin processor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleChorusAudioProcessor::SimpleChorusAudioProcessor()
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
	m_lfoparams.addParameter(m_paramVector,0);
	m_chorusparams.addParameter(m_paramVector);

	m_parameterVTS = std::make_unique<AudioProcessorValueTreeState>(*this, nullptr, Identifier("SimpleChorusVTS"),
		AudioProcessorValueTreeState::ParameterLayout(m_paramVector.begin(), m_paramVector.end()));
	m_parameterVTS->state.setProperty("version", JucePlugin_VersionString, nullptr);

	m_presets.setAudioValueTreeState(m_parameterVTS.get());
	m_presets.loadAllUserPresets();

	m_rate = m_parameterVTS->getRawParameterValue(paramLFORate.ID[0]);
	m_oldrate = *m_rate;
	m_waveform = m_parameterVTS->getRawParameterValue(paramLFOWaveform.ID[0]);
	m_oldwaveform = *m_waveform;
	m_delay = m_parameterVTS->getRawParameterValue(paramChorusDelay.ID);
	m_olddelay = *m_delay;
	m_width = m_parameterVTS->getRawParameterValue(paramChorusWidth.ID);
	m_oldwidth = *m_width;
	m_forward = m_parameterVTS->getRawParameterValue(paramChorusForward.ID);
	m_oldfeedback = *m_forward;
	m_feedback = m_parameterVTS->getRawParameterValue(paramChorusFeedback.ID);
	m_oldfeedback = *m_feedback;
	m_directout = m_parameterVTS->getRawParameterValue(paramChorusDirectOut.ID);
	m_olddirectout = *m_directout;

	m_phase = m_parameterVTS->getRawParameterValue(paramChorusPhase.ID);
	m_oldphase = *m_phase;

	m_lowForward = m_parameterVTS->getRawParameterValue(paramChorusFFLow.ID);
	m_oldlowForward = *m_lowForward;
	m_highForward = m_parameterVTS->getRawParameterValue(paramChorusFFHigh.ID);
	m_oldhighForward = *m_highForward;

	m_lowFeedback = m_parameterVTS->getRawParameterValue(paramChorusFBLow.ID);
	m_oldlowFeedback = *m_lowFeedback;
	m_highFeedback = m_parameterVTS->getRawParameterValue(paramChorusFBHigh.ID);
	m_oldhighFeedback = *m_highFeedback;


	m_chorus.setFeedback(*m_feedback);
	m_chorus.setBlend(*m_directout);
	m_chorus.setForward(*m_forward);

	m_chorus.setDelay(*m_delay);
	m_chorus.setWidth(*m_width);
	m_chorus.setRate(*m_rate);
	m_chorus.setPhase(*m_phase);

	m_chorus.setLowCutForward(*m_lowForward);
	m_chorus.setHighCutForward(*m_highForward);
	m_chorus.setLowCutFeedback(*m_lowFeedback);
	m_chorus.setHighCutFeedback(*m_highFeedback);




}

SimpleChorusAudioProcessor::~SimpleChorusAudioProcessor()
{
}

//==============================================================================
const String SimpleChorusAudioProcessor::getName() const
{
    return JucePlugin_Name;
}

bool SimpleChorusAudioProcessor::acceptsMidi() const
{
   #if JucePlugin_WantsMidiInput
    return true;
   #else
    return false;
   #endif
}

bool SimpleChorusAudioProcessor::producesMidi() const
{
   #if JucePlugin_ProducesMidiOutput
    return true;
   #else
    return false;
   #endif
}

bool SimpleChorusAudioProcessor::isMidiEffect() const
{
   #if JucePlugin_IsMidiEffect
    return true;
   #else
    return false;
   #endif
}

double SimpleChorusAudioProcessor::getTailLengthSeconds() const
{
    return 0.0;
}

int SimpleChorusAudioProcessor::getNumPrograms()
{
    return 1;   // NB: some hosts don't cope very well if you tell them there are 0 programs,
                // so this should be at least 1, even if you're not really implementing programs.
}

int SimpleChorusAudioProcessor::getCurrentProgram()
{
    return 0;
}

void SimpleChorusAudioProcessor::setCurrentProgram (int index)
{
}

const String SimpleChorusAudioProcessor::getProgramName (int index)
{
    return {};
}

void SimpleChorusAudioProcessor::changeProgramName (int index, const String& newName)
{
}

//==============================================================================
void SimpleChorusAudioProcessor::prepareToPlay (double sampleRate, int samplesPerBlock)
{
    // Use this method as the place to do any pre-playback
    // initialisation that you need..
	m_chorus.setSamplerate(sampleRate);
	m_chorus.setMaxBlockSize(samplesPerBlock);
	m_dataLeft.resize(samplesPerBlock);
	m_dataOutLeft.resize(samplesPerBlock);
	m_dataOutRight.resize(samplesPerBlock);
	m_dataRight.resize(samplesPerBlock);

	m_chorus.setRate(*m_rate);

}

void SimpleChorusAudioProcessor::releaseResources()
{
    // When playback stops, you can use this as an opportunity to free up any
    // spare memory, etc.
}

#ifndef JucePlugin_PreferredChannelConfigurations
bool SimpleChorusAudioProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
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

void SimpleChorusAudioProcessor::processBlock (AudioBuffer<float>& buffer, MidiBuffer& midiMessages)
{
    ScopedNoDenormals noDenormals;
	
	if (*m_rate != m_oldrate)
	{
		m_oldrate = *m_rate;
		m_chorus.setRate(exp(*m_rate));
	}

	if (*m_waveform != m_oldwaveform)
	{
		m_oldwaveform = *m_waveform;
		int index = int(*m_waveform + 0.5);
		m_chorus.setWaveform(static_cast <LFO::LFOFunctions> (index));
	}
	if (*m_delay != m_olddelay)
	{
		m_olddelay = *m_delay;
		m_chorus.setDelay((*m_delay));
	}
	if (*m_width != m_oldwidth)
	{
		m_oldwidth = *m_width;
		m_chorus.setWidth((*m_width));
	}
	if (*m_directout != m_olddirectout)
	{
		m_olddirectout = *m_directout;
		m_chorus.setBlend((*m_directout));
	}
	if (*m_forward != m_oldforward)
	{
		m_oldforward = *m_forward;
		m_chorus.setForward((*m_forward));
	}
	if (*m_feedback != m_oldfeedback)
	{
		m_oldfeedback = *m_feedback;
		m_chorus.setFeedback((*m_feedback));
	}
	if (*m_phase != m_oldphase)
	{
		m_oldphase = *m_phase;
		m_chorus.setPhase((*m_phase));
	}
	if (*m_lowForward != m_oldlowForward)
	{
		m_oldlowForward = *m_lowForward;
		m_chorus.setLowCutForward(*m_lowForward);
	}
	if (*m_highForward != m_oldhighForward)
	{
		m_oldhighForward = *m_highForward;
		m_chorus.setHighCutForward(*m_highForward);
	}
	if (*m_lowFeedback != m_oldlowFeedback)
	{
		m_oldlowFeedback = *m_lowFeedback;
		m_chorus.setLowCutFeedback(*m_lowFeedback);
	}
	if (*m_highFeedback != m_oldhighFeedback)
	{
		m_oldhighFeedback = *m_highFeedback;
		m_chorus.setHighCutFeedback(*m_highFeedback);
	}



	//AudioPlayHead::CurrentPositionInfo info;
	//getPlayHead()->getCurrentPosition(info);



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
	auto* left = buffer.getWritePointer(0);
	for (auto kk = 0u; kk < buffer.getNumSamples(); ++kk)
		m_dataLeft[kk] = left[kk];

	if (totalNumInputChannels == 2 && totalNumInputChannels == totalNumOutputChannels)
	{
		m_chorus.setNrofChns(2);
		auto* right = buffer.getWritePointer(1);
		for (auto kk = 0u; kk < buffer.getNumSamples(); ++kk)
			m_dataRight[kk] = right[kk];
	}
	else
	{
		for (auto kk = 0u; kk < buffer.getNumSamples(); ++kk)
			m_dataRight[kk] = left[kk];
	}

	m_chorus.processData(m_dataLeft, m_dataRight, m_dataOutLeft, m_dataOutRight);


	for (int channel = 0; channel < totalNumInputChannels; ++channel)
	{
		auto* channelData = buffer.getWritePointer(channel);

		// ..do something to the data...
		if (channel == 0)
			for (auto kk = 0u; kk < buffer.getNumSamples(); ++kk)
				channelData[kk] = m_dataOutLeft[kk];
		if (channel == 1)
			for (auto kk = 0u; kk < buffer.getNumSamples(); ++kk)
				channelData[kk] = m_dataOutRight[kk];
	}
}

//==============================================================================
bool SimpleChorusAudioProcessor::hasEditor() const
{
    return true; // (change this to false if you choose to not supply an editor)
}

AudioProcessorEditor* SimpleChorusAudioProcessor::createEditor()
{
    return new SimpleChorusAudioProcessorEditor (*this, *m_parameterVTS, m_presets);
}

//==============================================================================
void SimpleChorusAudioProcessor::getStateInformation (MemoryBlock& destData)
{
    // You should use this method to store your parameters in the memory block.
    // You could do that either as raw data, or use the XML or ValueTree classes
    // as intermediaries to make it easy to save and load complex data.
	auto state = m_parameterVTS->copyState();
	std::unique_ptr<XmlElement> xml(state.createXml());
	copyXmlToBinary(*xml, destData);
}

void SimpleChorusAudioProcessor::setStateInformation (const void* data, int sizeInBytes)
{
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
    return new SimpleChorusAudioProcessor();
}
