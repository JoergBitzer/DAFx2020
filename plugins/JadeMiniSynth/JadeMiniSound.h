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

    // Parameter for Osc1
    std::atomic<float>* m_osc1wave1;
    std::atomic<float>* m_osc1wave2;
    std::atomic<float>* m_osc1level;
    std::atomic<float>* m_osc1xfade;
    std::atomic<float>* m_osc1moddepth;
    std::atomic<float>* m_osc1tunecoarse;
    std::atomic<float>* m_osc1tunefine;

    // Parameter for Envelop1
    std::atomic<float>* m_env1Delay;
    std::atomic<float>* m_env1Attack;
    std::atomic<float>* m_env1Hold;
    std::atomic<float>* m_env1Decay;
    std::atomic<float>* m_env1Sustain;
    std::atomic<float>* m_env1Release;


	// Params for Moog Ladder
	std::atomic<float>* m_cutoff;
	std::atomic<float>* m_reso;
    std::atomic<float>* m_modKeyboard;
    std::atomic<float>* m_modEnvelope;
    std::atomic<float>* m_modLfo;


};