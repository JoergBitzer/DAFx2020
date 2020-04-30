#include "NanoVoice.h"
#include "NanoSound.h"

NanoVoice::NanoVoice()
	:SynthesiserVoice(),
	m_isAudioOn(false),
	m_a0tuning(440.0),
	m_waveforms(m_wavelen, 16000.0, 44100.0 / m_wavelen)
{
	std::vector<double> wave;
	wave.resize(m_wavelen);
	m_waveforms.getWavetable(wave, BasicWaveforms::waveforms::saw);
	m_osc.addF0Wavetable(wave);
	m_waveforms.getWavetable(wave, BasicWaveforms::waveforms::rect);
	m_osc.addF0Wavetable(wave);
	m_waveforms.getWavetable(wave, BasicWaveforms::waveforms::tri);
	m_osc.addF0Wavetable(wave);
	m_osc.setSecondWaveform(1);
	m_osc.setWaveform(0);
	m_osc.setWaveformXFade(0.0);

	m_envelope.setDelayTime(0.0);
	m_envelope.setAttackRate(1.0);
	m_envelope.setHoldTime(0.5);
	m_envelope.setDecayRate(20.0);
	m_envelope.setSustainLevel(0.2);
	m_envelope.setReleaseRate(300.0);
}

NanoVoice::~NanoVoice()
{
}

bool NanoVoice::canPlaySound(SynthesiserSound* sound)
{
	return dynamic_cast<NanoSound*>(sound) != nullptr;

}

void NanoVoice::startNote(int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition)
{
	int midia0 = 69;
	int distanceInSemitones = midiNoteNumber - midia0;

	m_freq = m_a0tuning * pow(2.0, double(distanceInSemitones) / 12.0);
	
	m_osc.setFrequency(m_freq);

	// m_isAudioOn = true;
	m_envelope.NoteOn();
	m_level = velocity;
}

void NanoVoice::stopNote(float velocity, bool allowTailOff)
{
	//m_isAudioOn = false;
	//clearCurrentNote(); // EIgentlich erst wenn release zu Ende ist.
	m_envelope.NoteOff();

}

void NanoVoice::pitchWheelMoved(int newPitchWheelValue)
{
}

void NanoVoice::controllerMoved(int controllerNumber, int newControllerValue)
{
}

void NanoVoice::renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{
	if (getCurrentlyPlayingNote() <= 0)
		return;

	m_data.resize(numSamples);
	m_osc.getData(m_data);
	m_envdata.resize(numSamples);
	m_envelope.getData(m_envdata);
	
	auto* channelData = outputBuffer.getArrayOfWritePointers();
	int counter;
	for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
	{
		counter = 0;
		for (auto kk = startSample; kk < startSample + numSamples; ++kk)
		{
			channelData[channel][kk] += m_level*m_data[counter] * m_envdata[counter];

			counter++;
		}
	}
	if (m_envelope.getEnvelopeStatus() == Envelope::envelopePhases::Off)
	{
		clearCurrentNote();
	}
}

void NanoVoice::prepareVoice(double samplerate, int maxBlockLen)
{
	m_fs = samplerate;
	m_maxLen = maxBlockLen;
	m_data.resize(m_maxLen);
	m_osc.setSamplerate(m_fs);

	m_envdata.resize(m_maxLen);
	m_envelope.setSamplerate(m_fs);

}
