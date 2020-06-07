#pragma once

#include "../JuceLibraryCode/JuceHeader.h"


class NanoSound : public SynthesiserSound
{
public:
    NanoSound() {};
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
    std::atomic<float>* m_cutoff;
    std::atomic<float>* m_reso;
};

