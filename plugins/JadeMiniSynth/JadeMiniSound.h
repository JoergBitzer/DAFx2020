#pragma once

#include <JuceHeader.h>


class JadeMiniSound : public SynthesiserSound
{
public:
    JadeMiniSound() {};
    //==============================================================================
    /** Returns true if this sound should be played when a given midi note is pressed.

        The Synthesiser will use this information when deciding which sounds to trigger
        for a given note.
    */
    bool appliesToNote(int midiNoteNumber) { return true; };

    /** Returns true if the sound should be triggered by midi events on a given channel.

        The Synthesiser will use this information when deciding which sounds to trigger
        for a given note.
    */
    bool appliesToChannel(int midiChannel) { return true; };

    // global parameter
    std::atomic<float>* m_voiceTuneA0;
    std::atomic<float>* m_voiceNrOfVoices;
    std::atomic<float>* m_voicePortamentoTime;

    // Parameter for Osc1
    std::atomic<float>* m_osc1wave1;
    std::atomic<float>* m_osc1wave2;
    std::atomic<float>* m_osc1level;
    std::atomic<float>* m_osc1xfade;
    std::atomic<float>* m_osc1moddepth;
    std::atomic<float>* m_osc1tunecoarse;
    std::atomic<float>* m_osc1tunefine;

    // Parameter for Osc2
    std::atomic<float>* m_osc2wave1;
    std::atomic<float>* m_osc2wave2;
    std::atomic<float>* m_osc2level;
    std::atomic<float>* m_osc2xfade;
    std::atomic<float>* m_osc2moddepth;
    std::atomic<float>* m_osc2tunecoarse;
    std::atomic<float>* m_osc2tunefine;


    // Parameter for Envelop1
    std::atomic<float>* m_env1Delay;
    std::atomic<float>* m_env1Attack;
    std::atomic<float>* m_env1Hold;
    std::atomic<float>* m_env1Decay;
    std::atomic<float>* m_env1Sustain;
    std::atomic<float>* m_env1Release;
    std::atomic<float>* m_env1Level;

    // Parameter for Envelop2
    std::atomic<float>* m_env2Delay;
    std::atomic<float>* m_env2Attack;
    std::atomic<float>* m_env2Hold;
    std::atomic<float>* m_env2Decay;
    std::atomic<float>* m_env2Sustain;
    std::atomic<float>* m_env2Release;
    std::atomic<float>* m_env2Level;
    std::atomic<float> * m_env2Invert;

// Parameter for LFO1
    std::atomic<float>* m_lfo1level;
    std::atomic<float>* m_lfo1waveform;
    std::atomic<float>* m_lfo1rate;

    // Parameter for LFO2
    std::atomic<float>* m_lfo2level;
    std::atomic<float>* m_lfo2waveform;
    std::atomic<float>* m_lfo2rate;


	// Params for Moog Ladder
	std::atomic<float>* m_cutoff;
	std::atomic<float>* m_reso;
    std::atomic<float>* m_modKeyboard;
    std::atomic<float>* m_modDepth;

    // Params for Noise Gen
    std::atomic<float>* m_whiteLevel;
    std::atomic<float>* m_whiteGraininess;
    std::atomic<float>* m_coloredLow;
    std::atomic<float>* m_coloredHigh;




};