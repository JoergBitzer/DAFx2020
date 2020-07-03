#include "JadeMiniVoice.h"
#include "JadeMiniSound.h"

// ToDo legato needs another solution


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
	m_osc2.addF0Wavetable(wave);
	m_waveforms.getWavetable(wave, BasicWaveforms::waveforms::rect);
	m_osc1.addF0Wavetable(wave);
	m_osc2.addF0Wavetable(wave);
	m_waveforms.getWavetable(wave, BasicWaveforms::waveforms::tri);
	m_osc1.addF0Wavetable(wave);
	m_osc2.addF0Wavetable(wave);

	m_waveforms.getWavetable(wave, BasicWaveforms::waveforms::noise, 1);
	m_osc1.addF0Wavetable(wave);
	m_osc2.addF0Wavetable(wave);
	m_waveforms.getWavetable(wave, BasicWaveforms::waveforms::noise, 1234);
	m_osc1.addF0Wavetable(wave);
	m_osc2.addF0Wavetable(wave);
	m_waveforms.getWavetable(wave, BasicWaveforms::waveforms::noise, 2345);
	m_osc1.addF0Wavetable(wave);
	m_osc2.addF0Wavetable(wave);

	m_waveforms.getWavetable(wave, BasicWaveforms::waveforms::rectPWM, 75);
	m_osc1.addF0Wavetable(wave);
	m_osc2.addF0Wavetable(wave);
	m_waveforms.getWavetable(wave, BasicWaveforms::waveforms::rectPWM, 50);
	m_osc1.addF0Wavetable(wave);
	m_osc2.addF0Wavetable(wave);
	m_waveforms.getWavetable(wave, BasicWaveforms::waveforms::rectPWM, 25);
	m_osc1.addF0Wavetable(wave);
	m_osc2.addF0Wavetable(wave);
	m_waveforms.getWavetable(wave, BasicWaveforms::waveforms::rectPWM, 5);
	m_osc1.addF0Wavetable(wave);
	m_osc2.addF0Wavetable(wave);

	m_osc1.setSecondWaveform(1);
	m_osc1.setWaveform(0);
	m_osc1.setWaveformXFade(0.0);

	m_osc2.setSecondWaveform(1);
	m_osc2.setWaveform(0);
	m_osc2.setWaveformXFade(0.0);

	m_envelope.setDelayTime(0.0);
	m_envelope.setAttackRate(5.0);
	m_envelope.setHoldTime(0.5);
	m_envelope.setDecayRate(20.0);
	m_envelope.setSustainLevel(0.2);
	m_envelope.setReleaseRate(300.0);
	m_envelope.setMaxLevel(1.0);
	m_envelope.setInvertOnOff(false);

	m_envelope2.setDelayTime(0.0);
	m_envelope2.setAttackRate(5.0);
	m_envelope2.setHoldTime(0.5);
	m_envelope2.setDecayRate(20.0);
	m_envelope2.setSustainLevel(0.2);
	m_envelope2.setReleaseRate(300.0);
	m_envelope2.setMaxLevel(1.0);
	m_envelope2.setInvertOnOff(false);


	m_filter.setCutoffFrequency(1000.0);
	m_filter.setSamplerate(44100.0);
	m_filter.setResonance(1.0);

// noiseGen
	m_noisegen.setHighpassCutoff(10000.f);
	m_noisegen.setLowpassCutoff(150.0);

// Lfo
	m_lfo1.setMin(0.0);
	m_lfo1.setMax(0.0000000000000001);
	m_lfo1.setPhase(0.0);
	m_lfo1.setLFOFunction(LFO::LFOFunctions::sinus);
	m_lfo1.setStartphase(0.0);
	m_lfo1.setFrequency(0.0);

	m_lfo2.setMin(0.0);
	m_lfo2.setMax(0.0000000000000001);
	m_lfo2.setPhase(0.0);
	m_lfo2.setLFOFunction(LFO::LFOFunctions::sinus);
	m_lfo2.setStartphase(0.0);
	m_lfo2.setFrequency(0.0);



	resetOldParams();
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
	m_curSound = dynamic_cast<JadeMiniSound*> (getCurrentlyPlayingSound().get());
	resetOldParams();
	// global
	float portaTime = exp(*(m_curSound->m_voicePortamentoTime));

	if (portaTime != m_oldvoicePortamentoTime)
	{
		if (portaTime < 0.2f) // SwitchOff Portamento for very small values
			m_portaTime_ms = -1.0;
		else
			m_portaTime_ms = portaTime;

		m_oldvoicePortamentoTime = portaTime;

	}
	Envelope::envelopePhases envphase =  m_envelope.getEnvelopeStatus();
	if (envphase == Envelope::envelopePhases::Off)
	{
		m_osc1.setPortamentoTime(-1.0);
		m_osc2.setPortamentoTime(-1.0);
	}
	else
	{
		m_osc1.setPortamentoTime(m_portaTime_ms);
		m_osc2.setPortamentoTime(m_portaTime_ms);
	}

	const int midia0 = 69;
	m_curNote = midiNoteNumber;
	int distanceInSemitones = m_curNote - midia0;

	m_freq = m_a0tuning * pow(2.0, double(distanceInSemitones) / 12.0);

	m_osc1.setFrequency(m_freq);
	m_osc2.setFrequency(m_freq);

	pitchWheelMoved(currentPitchWheelPosition);

	// m_isAudioOn = true;
	m_envelope.NoteOn();
	m_envelope2.NoteOn(); // filter envelope // ToDo legato need another solution here
	float modKeyboard = *(m_curSound->m_modKeyboard);
	if (modKeyboard != m_oldmodKeyboard)
	{
		m_filter.setKeyboardModulation(modKeyboard);
		m_oldmodKeyboard = modKeyboard;
	}
	m_filter.setMidiNote(midiNoteNumber);
	m_filter.reset();

	m_level = velocity;

	m_lfo1.reset();
	m_lfo2.reset();
}

void JadeMiniVoice::stopNote(float velocity, bool allowTailOff)
{
	//m_isAudioOn = false;
	//clearCurrentNote(); // EIgentlich erst wenn release zu Ende ist.
	
	m_envelope.NoteOff();
	m_envelope2.NoteOff();
}

void JadeMiniVoice::pitchWheelMoved(int newPitchWheelValue)
{
	double pitchBend = double(newPitchWheelValue - m_pitchBendMidPos)/8192.0;
	m_osc1.changeFrequencyRelative(pitchBend*m_pitchbendWidth_semitones);
	m_osc2.changeFrequencyRelative(pitchBend * m_pitchbendWidth_semitones);
}

void JadeMiniVoice::controllerMoved(int controllerNumber, int newControllerValue)
{
	if (controllerNumber == 1)
	{
		double val = double(newControllerValue) / 127.0;
		m_osc1.setWaveformXFade(val);
		m_osc2.setWaveformXFade(val);
	}
}

void JadeMiniVoice::renderNextBlock(AudioBuffer<float>& outputBuffer, int startSample, int numSamples)
{

	if (getCurrentlyPlayingNote() <= 0)
		return;

	updateParameter();

	// data gen
	m_data.resize(numSamples);
	m_osc1.getData(m_data);

	m_data2.resize(numSamples);
	m_osc2.getData(m_data2);

	m_noisedata.resize(numSamples);
	m_noisegen.getData(m_noisedata);

	for (auto kk = 0u; kk < numSamples; kk++)
	{
		m_data[kk] += m_noisedata[kk];
		m_data[kk] += m_data2[kk];
	}

	m_envdata.resize(numSamples);
	m_envelope.getData(m_envdata);

	m_env2data.resize(numSamples);
	m_envelope2.getData(m_env2data);

	m_lfo1data.resize(numSamples);
	m_lfo1.getData(m_lfo1data);

	m_lfo2data.resize(numSamples);
	m_lfo1.getData(m_lfo2data);

	for (auto kk = 0u; kk < numSamples; kk++)
		m_env2data[kk] += m_lfo1data[kk];

	m_filter.setModData(m_env2data);
	m_filter.processData(m_data, m_data);

// noise Source

	auto* channelData = outputBuffer.getArrayOfWritePointers();
	int counter;
	for (int channel = 0; channel < outputBuffer.getNumChannels(); ++channel)
	{
		counter = 0;
		for (auto kk = startSample; kk < startSample + numSamples; ++kk)
		{
			channelData[channel][kk] += m_level*(m_data[counter]) * m_envdata[counter];

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
	m_osc2.setSamplerate(m_fs);

	m_envdata.resize(m_maxLen);
	m_env2data.resize(m_maxLen);
	m_envelope.setSamplerate(m_fs);
	m_envelope2.setSamplerate(m_fs);
	m_filter.setSamplerate(m_fs);
	m_noisegen.setSamplerate(m_fs);
	m_lfo1.setSamplerate(m_fs);
	m_lfo2.setSamplerate(m_fs);
}

void JadeMiniVoice::resetOldParams()
{
	m_oldvoiceTuneA0 = -1.f;
	m_oldvoicePortamentoTime = -100.f;

	m_oldcutoff = -1.f;
	m_oldreso = -1.f;
	m_oldmodKeyboard = -2.f;
	m_oldmodDepth = -100.f;

	m_oldosc1wave1 = -1.0;
	m_oldosc1wave2 = -1.0;
	m_oldosc1level = -100.0;
	m_oldosc1xfade = -1.0;
	m_oldosc1moddepth = 0.0;
	m_oldosc1tunecoarse = -1000.0;
	m_oldosc1tunefine = -100.0;

	m_oldosc2wave1 = -1.0;
	m_oldosc2wave2 = -1.0;
	m_oldosc2level = -100.0;
	m_oldosc2xfade = -1.0;
	m_oldosc2moddepth = 0.0;
	m_oldosc2tunecoarse = -1000.0;
	m_oldosc2tunefine = -100.0;

	m_oldenv1Delay = -1.0;
	m_oldenv1Attack = 0.0;
	m_oldenv1Hold = -1.0;
	m_oldenv1Decay = 0.0;
	m_oldenv1Sustain = -1.0;
	m_oldenv1Release = 0.0;
	m_oldenv1Level = -100.0;

	m_oldenv2Delay = -1.0;
	m_oldenv2Attack = 0.0;
	m_oldenv2Hold = -1.0;
	m_oldenv2Decay = 0.0;
	m_oldenv2Sustain = -1.0;
	m_oldenv2Release = 0.0;
	m_oldenv2Level = -100.0;
	m_oldenv2Invert = -1.f;


	m_oldwhiteLevel = -1000.f;
	m_oldwhiteGraininess = -1.f;
	m_oldcoloredLow = -1.f;
	m_oldcoloredHigh = 1.f;

	m_oldlfo1level = -100.f;
	m_oldlfo1rate = -1.f;
	m_oldlfo1waveform = -1.f;
	m_oldlfo2level = -100.f;
	m_oldlfo2rate = -1.f;
	m_oldlfo2waveform = -1.f;
}

void JadeMiniVoice::updateParameter()
{


	float globalTuneA0 = *(m_curSound->m_voiceTuneA0);

	if (globalTuneA0 != m_oldvoiceTuneA0)
	{
		m_oldvoiceTuneA0 = globalTuneA0;
		m_a0tuning = globalTuneA0;
	}

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
		m_osc1.setWaveformXFade(osc1xfade * 0.01); // % in 0 ..1
	}

	float osc1tunecoarse = int(*(m_curSound->m_osc1tunecoarse) + 0.5);

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

	float osc2waveform1 = *(m_curSound->m_osc2wave1);

	if (osc2waveform1 != m_oldosc2wave1)
	{
		m_oldosc2wave1 = osc2waveform1;
		m_osc2.setWaveform(static_cast<int> (osc2waveform1));
	}
	float osc2waveform2 = *(m_curSound->m_osc2wave2);

	if (osc2waveform2 != m_oldosc2wave2)
	{
		m_oldosc2wave2 = osc2waveform2;
		m_osc2.setSecondWaveform(static_cast<int> (osc2waveform2));
	}

	float osc2xfade = *(m_curSound->m_osc2xfade);

	if (osc2xfade != m_oldosc2xfade)
	{
		m_oldosc2xfade = osc2xfade;
		m_osc2.setWaveformXFade(osc2xfade * 0.01); // % in 0 ..1
	}

	float osc2tunecoarse = int(*(m_curSound->m_osc2tunecoarse) + 0.5);

	if (osc2tunecoarse != m_oldosc2tunecoarse)
	{
		m_oldosc2tunecoarse = osc2tunecoarse;
		m_osc2.setTuneCoarse(osc2tunecoarse);
	}

	float osc2tunefine = *(m_curSound->m_osc2tunefine);

	if (osc2tunefine != m_oldosc2tunefine)
	{
		m_oldosc2tunefine = osc2tunefine;
		m_osc2.setTuneFine(osc2tunefine);
	}

	float osc2level = *(m_curSound->m_osc2level);

	if (osc2level != m_oldosc2level)
	{
		m_oldosc2level = osc2level;
		m_osc2.setLevel(osc2level);
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

	float env1Level = pow(10.f, *(m_curSound->m_env1Level) / 20.f);

	if (env1Level != m_oldenv1Level)
	{
		m_oldenv1Level = env1Level;
		m_envelope.setMaxLevel(env1Level);
	}

	// envelope filter
	float env2Delay = *(m_curSound->m_env2Delay);

	if (env2Delay != m_oldenv2Delay)
	{
		m_oldenv2Delay = env2Delay;
		m_envelope2.setDelayTime(env2Delay);
	}

	float env2Hold = *(m_curSound->m_env2Hold);

	if (env2Hold != m_oldenv2Hold)
	{
		m_oldenv2Hold = env2Hold;
		m_envelope2.setHoldTime(env2Hold);
	}

	float env2Sustain = *(m_curSound->m_env2Sustain);

	if (env2Sustain != m_oldenv2Sustain)
	{
		m_oldenv2Sustain = env2Sustain;
		m_envelope2.setSustainLevel(env2Sustain);
	}

	float env2Attack = exp(*(m_curSound->m_env2Attack));

	if (env2Attack != m_oldenv2Attack)
	{
		m_oldenv2Attack = env2Attack;
		m_envelope2.setAttackRate(env2Attack);
	}

	float env2Decay = exp(*(m_curSound->m_env2Decay));

	if (env2Decay != m_oldenv2Decay)
	{
		m_oldenv2Decay = env2Decay;
		m_envelope2.setDecayRate(env2Decay);
	}

	float env2Release = exp(*(m_curSound->m_env2Release));

	if (env2Release != m_oldenv2Release)
	{
		m_oldenv2Release = env2Release;
		m_envelope2.setReleaseRate(env2Release);
	}

	float env2Level = pow(10.f, *(m_curSound->m_env2Level) / 20.f);

	if (env2Level != m_oldenv2Level)
	{
		m_oldenv2Level = env2Level;
		m_envelope2.setMaxLevel(env2Level);
	}
	float env2Invert = *(m_curSound->m_env2Invert);

	if (env2Invert != m_oldenv2Invert)
	{
		m_oldenv2Invert = env2Invert;
		if (env2Invert > 0.5)
			m_envelope2.setInvertOnOff(true);
		else
			m_envelope2.setInvertOnOff(false);

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

	float modDepth = pow(10.f, *(m_curSound->m_modDepth) / 20.f);
	if (modDepth != m_oldmodDepth)
	{
		m_filter.setModDepth(modDepth);
		m_oldmodDepth = modDepth;
	}

	// noise Generator
	float whiteLevel = *(m_curSound->m_whiteLevel);
	if (whiteLevel != m_oldwhiteLevel)
	{
		m_noisegen.setLogAmplitude(whiteLevel);
		//		m_filter.setResonance(modLfo);
		m_oldwhiteLevel = whiteLevel;
	}
	float whitegraininess = log(*(m_curSound->m_whiteGraininess));
	if (whitegraininess != m_oldwhiteGraininess)
	{
		m_noisegen.setGraininess(whitegraininess);
		//		m_filter.setResonance(modLfo);
		m_oldwhiteGraininess = whitegraininess;
	}

	float coloredLow = *(m_curSound->m_coloredLow);
	if (whitegraininess != m_oldcoloredLow)
	{
		m_noisegen.setLowpassOrder(coloredLow);
		//		m_filter.setResonance(modLfo);
		m_oldcoloredLow = coloredLow;
	}

	float coloredHigh = *(m_curSound->m_coloredHigh);
	if (coloredHigh != m_oldcoloredHigh)
	{
		m_noisegen.setHighpassOrder(coloredHigh);
		//		m_filter.setResonance(modLfo);
		m_oldcoloredHigh = coloredHigh;
	}

	float lfo1waveform = *(m_curSound->m_lfo1waveform);
	if (lfo1waveform != m_oldlfo1waveform)
	{
		m_oldlfo1waveform = lfo1waveform;
		int index = int(lfo1waveform + 0.5);
		m_lfo1.setLFOFunction(static_cast <LFO::LFOFunctions> (index));
	}

	float lfo1rate = exp(*(m_curSound->m_lfo1rate));
	if (lfo1rate != m_oldlfo1rate)
	{
		m_oldlfo1rate = lfo1rate;
		m_lfo1.setFrequency(lfo1rate);
	}

	float lfo1level = pow(10.f, *(m_curSound->m_lfo1level) / 20.f);
	if (lfo1level != m_oldlfo1level)
	{
		m_oldlfo1level = lfo1level;
		m_lfo1.setMax(lfo1level);
	}
	float lfo2waveform = *(m_curSound->m_lfo2waveform);
	if (lfo2waveform != m_oldlfo2waveform)
	{
		m_oldlfo2waveform = lfo2waveform;
		int index = int(lfo2waveform + 0.5);
		m_lfo2.setLFOFunction(static_cast <LFO::LFOFunctions> (index));
	}

	float lfo2rate = exp(*(m_curSound->m_lfo2rate));
	if (lfo2rate != m_oldlfo2rate)
	{
		m_oldlfo2rate = lfo2rate;
		m_lfo2.setFrequency(lfo2rate);
	}

	float lfo2level = pow(10.f, *(m_curSound->m_lfo2level) / 20.f);
	if (lfo2level != m_oldlfo2level)
	{
		m_oldlfo2level = lfo2level;
		m_lfo2.setMax(lfo2level);
	}

}

int VoiceParameter::addParameter(std::vector < std::unique_ptr<RangedAudioParameter>>& paramVector)
{
	paramVector.push_back(std::make_unique<AudioParameterFloat>(paramGlobalPortamentoTime.ID,
		paramGlobalPortamentoTime.name,
		NormalisableRange<float>(paramGlobalPortamentoTime.minValue, paramGlobalPortamentoTime.maxValue),
		paramGlobalPortamentoTime.defaultValue,
		paramGlobalPortamentoTime.unitName,
		AudioProcessorParameter::genericParameter,
		[](float value, int MaxLen) {value = int(exp(value) * 10) * 0.1;  
									if (value < 0.2) 
										return String("Off"); 
									else 
										return String(value, MaxLen);
										},
		[](const String& text) { return text.getFloatValue(); }));
		
	paramVector.push_back(std::make_unique<AudioParameterFloat>(paramGlobalNrOfVoices.ID,
		paramGlobalNrOfVoices.name,
		NormalisableRange<float>(paramGlobalNrOfVoices.minValue, paramGlobalNrOfVoices.maxValue),
		paramGlobalNrOfVoices.defaultValue,
		paramGlobalNrOfVoices.unitName,
		AudioProcessorParameter::genericParameter,
		[](float value, int MaxLen) {value = int(value+0.5);  return String(value, MaxLen); },
		[](const String& text) { return text.getFloatValue(); }));

	paramVector.push_back(std::make_unique<AudioParameterFloat>(paramGlobalTuneA0.ID,
		paramGlobalTuneA0.name,
		NormalisableRange<float>(paramGlobalTuneA0.minValue, paramGlobalTuneA0.maxValue),
		paramGlobalTuneA0.defaultValue,
		paramGlobalTuneA0.unitName,
		AudioProcessorParameter::genericParameter,
		[](float value, int MaxLen) {value = int((value) * 10) * 0.1;  return String(value, MaxLen); },
		[](const String& text) { return text.getFloatValue(); }));

	return 0;
}

VoiceParameterComponent::VoiceParameterComponent(AudioProcessorValueTreeState& vts)
	:m_vts(vts), somethingChanged(nullptr), m_style(ComponentStyle::horizontal)
{
	m_PortamentoTimeLabel.setText("Glide", NotificationType::dontSendNotification);
	m_PortamentoTimeLabel.setJustificationType(Justification::centred);
	addAndMakeVisible(m_PortamentoTimeLabel);

	m_PortamentoTimeSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	m_PortamentoTimeSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 60, 20);
	m_PortamentoTimeAttachment = std::make_unique<SliderAttachment>(m_vts, paramGlobalPortamentoTime.ID, m_PortamentoTimeSlider);
	addAndMakeVisible(m_PortamentoTimeSlider);
	m_PortamentoTimeSlider.onValueChange = [this]() {if (somethingChanged != nullptr) somethingChanged(); };

	m_NrOfVoicesLabel.setText("NrOfVoices", NotificationType::dontSendNotification);
	m_NrOfVoicesLabel.setJustificationType(Justification::centred);
	addAndMakeVisible(m_NrOfVoicesLabel);

	m_NrOfVoicesSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	m_NrOfVoicesSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 60, 20);
	m_NrOfVoicesAttachment = std::make_unique<SliderAttachment>(m_vts, paramGlobalNrOfVoices.ID, m_NrOfVoicesSlider);
	addAndMakeVisible(m_NrOfVoicesSlider);
	m_NrOfVoicesSlider.onValueChange = [this]() {if (somethingChanged != nullptr) somethingChanged(); };

	m_GlobalTuneA0Label.setText("TuneA0", NotificationType::dontSendNotification);
	m_GlobalTuneA0Label.setJustificationType(Justification::centred);
	addAndMakeVisible(m_GlobalTuneA0Label);

	m_GlobalTuneA0Slider.setSliderStyle(Slider::SliderStyle::Rotary);
	m_GlobalTuneA0Slider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 60, 20);
	m_GlobalTuneA0Attachment = std::make_unique<SliderAttachment>(m_vts, paramGlobalTuneA0.ID, m_GlobalTuneA0Slider);
	addAndMakeVisible(m_GlobalTuneA0Slider);
	m_GlobalTuneA0Slider.onValueChange = [this]() {if (somethingChanged != nullptr) somethingChanged(); };

}

void VoiceParameterComponent::paint(Graphics& g)
{
	g.fillAll((getLookAndFeel().findColour(ResizableWindow::backgroundColourId)).darker(0.2));

}

#define GLOBAL_LABEL_WIDTH 60
#define GLOBAL_ROTARY_WIDTH 60
#define GLOBAL_MIN_DISTANCE 5
#define GLOBAL_LABEL_HEIGHT 20

void VoiceParameterComponent::resized()
{
	auto r = getLocalBounds();
	r.reduce(GLOBAL_MIN_DISTANCE, GLOBAL_MIN_DISTANCE);
	auto s = r;
	auto t = r;
	switch (m_style)
	{
	case ComponentStyle::compact:

		break;
	case ComponentStyle::horizontal:
		s = r.removeFromTop(GLOBAL_LABEL_HEIGHT);
		m_NrOfVoicesLabel.setBounds(s.removeFromLeft(GLOBAL_LABEL_WIDTH));
		s.removeFromLeft(GLOBAL_MIN_DISTANCE);
		m_GlobalTuneA0Label.setBounds(s.removeFromLeft(GLOBAL_LABEL_WIDTH));
		s.removeFromLeft(GLOBAL_MIN_DISTANCE);
		m_PortamentoTimeLabel.setBounds(s.removeFromLeft(GLOBAL_LABEL_WIDTH));

		s = r;
		t = s.removeFromBottom(GLOBAL_ROTARY_WIDTH);
		m_NrOfVoicesSlider.setBounds(t.removeFromLeft(GLOBAL_ROTARY_WIDTH));
		t.removeFromLeft(GLOBAL_MIN_DISTANCE);
		m_GlobalTuneA0Slider.setBounds(t.removeFromLeft(GLOBAL_ROTARY_WIDTH));
		t.removeFromLeft(GLOBAL_MIN_DISTANCE);
		m_PortamentoTimeSlider.setBounds(t.removeFromLeft(GLOBAL_ROTARY_WIDTH));

		break;
	case ComponentStyle::vertical:

		break;
	}


}
