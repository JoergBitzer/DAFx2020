#pragma once

#include <vector>
#include "../JuceLibraryCode/JuceHeader.h"
#include <BasicWaveforms.h>
#include <WavetableOszi.h>
#include <Envelope.h>

class NanoVoice : public SynthesiserVoice
{
public:
    NanoVoice();
    ~NanoVoice();
    /** Must return true if this voice object is capable of playing the given sound.

    If there are different classes of sound, and different classes of voice, a voice can
    choose which ones it wants to take on.

    A typical implementation of this method may just return true if there's only one type
    of voice and sound, or it might check the type of the sound object passed-in and
    see if it's one that it understands.
*/
    bool canPlaySound(SynthesiserSound*);

    /** Called to start a new note.
        This will be called during the rendering callback, so must be fast and thread-safe.
    */
    void startNote(int midiNoteNumber,
        float velocity,
        SynthesiserSound* sound,
        int currentPitchWheelPosition);

    /** Called to stop a note.

        This will be called during the rendering callback, so must be fast and thread-safe.

        The velocity indicates how quickly the note was released - 0 is slowly, 1 is quickly.

        If allowTailOff is false or the voice doesn't want to tail-off, then it must stop all
        sound immediately, and must call clearCurrentNote() to reset the state of this voice
        and allow the synth to reassign it another sound.

        If allowTailOff is true and the voice decides to do a tail-off, then it's allowed to
        begin fading out its sound, and it can stop playing until it's finished. As soon as it
        finishes playing (during the rendering callback), it must make sure that it calls
        clearCurrentNote().
    */
    void stopNote(float velocity, bool allowTailOff);
    /** Called to let the voice know that the pitch wheel has been moved.
        This will be called during the rendering callback, so must be fast and thread-safe.
    */
    void pitchWheelMoved(int newPitchWheelValue);

    /** Called to let the voice know that a midi controller has been moved.
        This will be called during the rendering callback, so must be fast and thread-safe.
    */
    void controllerMoved(int controllerNumber, int newControllerValue);
    //==============================================================================
    /** Renders the next block of data for this voice.

        The output audio data must be added to the current contents of the buffer provided.
        Only the region of the buffer between startSample and (startSample + numSamples)
        should be altered by this method.

        If the voice is currently silent, it should just return without doing anything.

        If the sound that the voice is playing finishes during the course of this rendered
        block, it must call clearCurrentNote(), to tell the synthesiser that it has finished.

        The size of the blocks that are rendered can change each time it is called, and may
        involve rendering as little as 1 sample at a time. In between rendering callbacks,
        the voice's methods will be called to tell it about note and controller events.
    */
    void renderNextBlock(AudioBuffer<float>& outputBuffer,
        int startSample,
        int numSamples) ;

    void prepareVoice(double samplerate, int maxBlockLen);

private:
    static const int m_wavelen = 512;
    int m_maxLen;
    double m_fs;
    double m_freq;

    double m_a0tuning;
    bool m_isAudioOn;

    WavetableOszi m_osc;
    BasicWaveforms m_waveforms;
    std::vector<double> m_data;
    std::vector<double> m_envdata;
    // Envelope;
    Envelope m_envelope;
    double m_level;
};

