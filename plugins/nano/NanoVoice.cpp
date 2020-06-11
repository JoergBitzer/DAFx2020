#include "NanoVoice.h"
#include "NanoSound.h"

NanoVoice::NanoVoice()
	:SynthesiserVoice(),
	m_isAudioOn(false),
	m_a0tuning(440.0),
	m_pitchbendWidth_semitones(12.0),
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
	m_envelope.setAttackRate(5.0);
	m_envelope.setHoldTime(0.5);
	m_envelope.setDecayRate(20.0);
	m_envelope.setSustainLevel(0.2);
	m_envelope.setReleaseRate(300.0);
	m_portaTime_ms = -1.0;
	m_resonance = 3.7;
	m_cutoff = 600.0;
	m_filter.setCutoffFrequency(m_cutoff);
	m_filter.setSamplerate(44100.0);
	m_filter.setResonance(m_resonance);
	m_oldcutoff = m_cutoff;


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
	Envelope::envelopePhases envphase =  m_envelope.getEnvelopeStatus();
	if (envphase == Envelope::envelopePhases::Off)
		m_osc.setPortamentoTime(-1.0);
	else
		m_osc.setPortamentoTime(m_portaTime_ms);
	int midia0 = 69;
	m_curNote = midiNoteNumber;
	int distanceInSemitones = m_curNote - midia0;

	//double pitchBend = double(currentPitchWheelPosition - m_pitchBendMidPos)/8192.0;

	//m_freq = m_a0tuning * pow(2.0, double(distanceInSemitones+pitchBend*m_pitchbendWidth_semitones) / 12.0);
	m_freq = m_a0tuning * pow(2.0, double(distanceInSemitones) / 12.0);

	m_osc.setFrequency(m_freq);
	
	pitchWheelMoved(currentPitchWheelPosition);

	// m_isAudioOn = true;
	m_envelope.NoteOn();
	m_level = velocity;
	m_curSound = dynamic_cast<NanoSound*> (getCurrentlyPlayingSound().get());

}

void NanoVoice::stopNote(float velocity, bool allowTailOff)
{
	//m_isAudioOn = false;
	//clearCurrentNote(); // EIgentlich erst wenn release zu Ende ist.
	m_envelope.NoteOff();

}

void NanoVoice::pitchWheelMoved(int newPitchWheelValue)
{
	//int midia0 = 69;
	//int distanceInSemitones = m_curNote - midia0;
	double pitchBend = double(newPitchWheelValue - m_pitchBendMidPos)/8192.0;
	//double newfreq = m_freq * pow(2.0, double(pitchBend*m_pitchbendWidth_semitones) / 12.0);

	m_osc.changeFrequencyRelative(pitchBend*m_pitchbendWidth_semitones);

}

void NanoVoice::controllerMoved(int controllerNumber, int newControllerValue)
{
	if (controllerNumber == 1)
	{
		double val = double(newControllerValue) / 127.0;
		m_osc.setWaveformXFade(val);
	}

}

void NanoVoice::renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{

	if (getCurrentlyPlayingNote() <= 0)
		return;


	float cutoff = exp(*(m_curSound->m_cutoff));
	if (cutoff != m_oldcutoff)
	{
		m_filter.setCutoffFrequency(cutoff);
		m_oldcutoff = cutoff;
	}
	float reso = *(m_curSound->m_reso);
	if (reso != m_oldreso)
	{
		m_filter.setResonance(reso);
		m_oldreso = reso;
	}

	m_data.resize(numSamples);
	m_osc.getData(m_data);
	
	m_filter.processData(m_data, m_data);

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
	m_filter.setSamplerate(m_fs);
}
