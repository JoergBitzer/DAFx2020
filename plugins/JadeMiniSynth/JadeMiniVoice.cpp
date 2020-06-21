#include "JadeMiniVoice.h"
#include "JadeMiniSound.h"

JadeMiniVoice::JadeMiniVoice()
	:SynthesiserVoice(),
	m_isAudioOn(false),
	m_a0tuning(440.0),
	m_pitchbendWidth_semitones(12.0),
	m_waveforms(m_wavelen, 16000.0, 44100.0 / m_wavelen),
	m_level(1.0),
	m_portaTime_ms(-1.0)
{
	std::vector<double> wave;
	wave.resize(m_wavelen);

	m_waveforms.getWavetable(wave, BasicWaveforms::waveforms::saw);
	m_osc1.addF0Wavetable(wave);
	m_waveforms.getWavetable(wave, BasicWaveforms::waveforms::rect);
	m_osc1.addF0Wavetable(wave);
	m_waveforms.getWavetable(wave, BasicWaveforms::waveforms::tri);
	m_osc1.addF0Wavetable(wave);


	m_osc1.setSecondWaveform(1);
	m_osc1.setWaveform(0);
	m_osc1.setWaveformXFade(0.0);

	m_envelope.setDelayTime(0.0);
	m_envelope.setAttackRate(5.0);
	m_envelope.setHoldTime(0.5);
	m_envelope.setDecayRate(20.0);
	m_envelope.setSustainLevel(0.2);
	m_envelope.setReleaseRate(300.0);

	m_filter.setCutoffFrequency(1000.0);
	m_filter.setSamplerate(44100.0);
	m_filter.setResonance(1.0);
	m_oldcutoff = -1.f;
	m_oldreso = -1.f;
	m_oldmodKeyboard = -2.f;
	m_oldmodEnvelope = -2.f;
	m_oldmodLfo =-2.f;

	m_oldosc1wave1 = -1.0;
	m_oldosc1wave2= -1.0;
	m_oldosc1level= -100.0;
	m_oldosc1xfade= -1.0;
	m_oldosc1moddepth= 0.0;
	m_oldosc1tunecoarse= -1000.0;
	m_oldosc1tunefine= -100.0;

	m_oldenv1Delay= -1.0;
	m_oldenv1Attack= 0.0;
	m_oldenv1Hold= -1.0;
	m_oldenv1Decay= 0.0;
	m_oldenv1Sustain= -1.0;
	m_oldenv1Release= 0.0;


}

JadeMiniVoice::~JadeMiniVoice()
{
}

bool JadeMiniVoice::canPlaySound(SynthesiserSound* sound)
{
	return dynamic_cast<JadeMiniSound*>(sound) != nullptr;

}

void JadeMiniVoice::startNote(int midiNoteNumber, float velocity, SynthesiserSound* sound, int currentPitchWheelPosition)
{

	Envelope::envelopePhases envphase =  m_envelope.getEnvelopeStatus();
	if (envphase == Envelope::envelopePhases::Off)
		m_osc1.setPortamentoTime(-1.0);
	else
		m_osc1.setPortamentoTime(m_portaTime_ms);


	const int midia0 = 69;
	m_curNote = midiNoteNumber;
	int distanceInSemitones = m_curNote - midia0;

	m_freq = m_a0tuning * pow(2.0, double(distanceInSemitones) / 12.0);

	m_osc1.setFrequency(m_freq);
	
	pitchWheelMoved(currentPitchWheelPosition);
	m_curSound = dynamic_cast<JadeMiniSound*> (getCurrentlyPlayingSound().get());

	// m_isAudioOn = true;
	m_envelope.NoteOn();
	float modKeyboard = *(m_curSound->m_modKeyboard);
	if (modKeyboard != m_oldmodKeyboard)
	{
		m_filter.setKeyboardModulation(modKeyboard);
		m_oldmodKeyboard = modKeyboard;
	}
	m_filter.setMidiNote(midiNoteNumber);
	m_filter.reset();

	m_level = velocity;
}

void JadeMiniVoice::stopNote(float velocity, bool allowTailOff)
{
	//m_isAudioOn = false;
	//clearCurrentNote(); // EIgentlich erst wenn release zu Ende ist.
	
	m_envelope.NoteOff();

}

void JadeMiniVoice::pitchWheelMoved(int newPitchWheelValue)
{
	double pitchBend = double(newPitchWheelValue - m_pitchBendMidPos)/8192.0;
	m_osc1.changeFrequencyRelative(pitchBend*m_pitchbendWidth_semitones);
}

void JadeMiniVoice::controllerMoved(int controllerNumber, int newControllerValue)
{
	if (controllerNumber == 1)
	{
		double val = double(newControllerValue) / 127.0;
		m_osc1.setWaveformXFade(val);
	}
}

void JadeMiniVoice::renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{

	if (getCurrentlyPlayingNote() <= 0)
		return;

	float osc1waveform1 = *(m_curSound->m_osc1wave1);

	if (osc1waveform1 != m_oldosc1wave1)
	{
		m_oldosc1wave1 = osc1waveform1;
		m_osc1.setWaveform(static_cast<int> (osc1waveform1));
	}
	float osc1waveform2 = *(m_curSound->m_osc1wave2);

	if (osc1waveform2 != m_oldosc1wave2)
	{
		m_oldosc1wave2 = osc1waveform2;
		m_osc1.setSecondWaveform(static_cast<int> (osc1waveform2));
	}

	float osc1xfade = *(m_curSound->m_osc1xfade);

	if (osc1xfade != m_oldosc1xfade)
	{
		m_oldosc1xfade = osc1xfade;
		m_osc1.setWaveformXFade(osc1xfade*0.01); // % in 0 ..1
	}

	float osc1tunecoarse = *(m_curSound->m_osc1tunecoarse);

	if (osc1tunecoarse != m_oldosc1tunecoarse)
	{
		m_oldosc1tunecoarse = osc1tunecoarse;
		m_osc1.setTuneCoarse(osc1tunecoarse);
	}

	float osc1tunefine = *(m_curSound->m_osc1tunefine);

	if (osc1tunefine != m_oldosc1tunefine)
	{
		m_oldosc1tunefine = osc1tunefine;
		m_osc1.setTuneFine(osc1tunefine);
	}

	float osc1level = *(m_curSound->m_osc1level);

	if (osc1level != m_oldosc1level)
	{
		m_oldosc1level = osc1level;
		m_osc1.setLevel(osc1level);
	}

	float env1Delay = *(m_curSound->m_env1Delay);

	if (env1Delay != m_oldenv1Delay)
	{
		m_oldenv1Delay = env1Delay;
		m_envelope.setDelayTime(env1Delay);
	}

	float env1Hold = *(m_curSound->m_env1Hold);

	if (env1Hold != m_oldenv1Hold)
	{
		m_oldenv1Hold = env1Hold;
		m_envelope.setHoldTime(env1Hold);
	}

	float env1Sustain = *(m_curSound->m_env1Sustain);

	if (env1Sustain != m_oldenv1Sustain)
	{
		m_oldenv1Sustain = env1Sustain;
		m_envelope.setSustainLevel(env1Sustain);
	}

	float env1Attack = exp(*(m_curSound->m_env1Attack));

	if (env1Attack != m_oldenv1Attack)
	{
		m_oldenv1Attack = env1Attack;
		m_envelope.setAttackRate(env1Attack);
	}

	float env1Decay = exp(*(m_curSound->m_env1Decay));

	if (env1Decay != m_oldenv1Decay)
	{
		m_oldenv1Decay = env1Decay;
		m_envelope.setDecayRate(env1Decay);
	}

	float env1Release = exp(*(m_curSound->m_env1Release));

	if (env1Release != m_oldenv1Release)
	{
		m_oldenv1Release = env1Release;
		m_envelope.setReleaseRate(env1Release);
	}




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
	
	float modKeyboard = *(m_curSound->m_modKeyboard);
	if (modKeyboard != m_oldmodKeyboard)
	{
		m_filter.setKeyboardModulation(modKeyboard);
		m_oldmodKeyboard = modKeyboard;
	}
	
	float modEnvelope = *(m_curSound->m_modEnvelope);
	if (modEnvelope != m_oldmodEnvelope)
	{
//		m_filter.setResonance(modEnvelope);
		m_oldmodEnvelope = modEnvelope;
	}

	float modLfo = *(m_curSound->m_modLfo);
	if (modLfo != m_oldmodLfo)
	{
//		m_filter.setResonance(modLfo);
		m_oldmodLfo = modLfo;
	}



	m_data.resize(numSamples);
	m_osc1.getData(m_data);
	
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

void JadeMiniVoice::prepareVoice(double samplerate, int maxBlockLen)
{
	m_fs = samplerate;
	m_maxLen = maxBlockLen;

	m_data.resize(m_maxLen);

	m_osc1.setSamplerate(m_fs);

	m_envdata.resize(m_maxLen);
	m_envelope.setSamplerate(m_fs);
	m_filter.setSamplerate(m_fs);

}
