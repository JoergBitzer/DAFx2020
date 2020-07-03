#pragma once

#include <vector>
#include "../JuceLibraryCode/JuceHeader.h"
#include "BasicWaveforms.h"
#include "WavetableOszi.h"
#include "MoogLadder.h"
#include "Envelope.h"
#include "WhiteNoiseGenerator.h"
#include "ColoredNoiseGenerator.h"

#include "LFO/LFO.h"

#include "JadeMiniSound.h"

class JadeMiniVoice : public SynthesiserVoice
{
public:
    JadeMiniVoice();
    ~JadeMiniVoice();
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
    void resetOldParams();
    static const int m_wavelen = 512;
    int m_maxLen;
    double m_fs;
    double m_freq;
    int m_curNote;

    double m_a0tuning;
    bool m_isAudioOn;

    // pitchbending
    const int m_pitchBendMidPos = 8192;
    double m_pitchbendWidth_semitones;
    JadeMiniSound* m_curSound;
    double m_level;
    double m_portaTime_ms;


    BasicWaveforms m_waveforms;
    WavetableOszi m_osc1;
    std::vector<double> m_data;
    WavetableOszi m_osc2;
    std::vector<double> m_data2;

    // Gloabl
    float m_oldvoiceTuneA0;
    float m_oldvoicePortamentoTime;


    // parameter osc1 
    float m_oldosc1wave1;
    float m_oldosc1wave2;
    float m_oldosc1level;
    float m_oldosc1xfade;
    float m_oldosc1moddepth;
    float m_oldosc1tunecoarse;
    float m_oldosc1tunefine;

    // parameter osc2 
    float m_oldosc2wave1;
    float m_oldosc2wave2;
    float m_oldosc2level;
    float m_oldosc2xfade;
    float m_oldosc2moddepth;
    float m_oldosc2tunecoarse;
    float m_oldosc2tunefine;


// envelope
    std::vector<double> m_envdata;
    // Envelope;
    Envelope m_envelope;

// parameter envelope
    float m_oldenv1Delay;
    float m_oldenv1Attack;
    float m_oldenv1Hold;
    float m_oldenv1Decay;
    float m_oldenv1Sustain;
    float m_oldenv1Release;
    float m_oldenv1Level;

    // envelope
    std::vector<double> m_env2data;
    Envelope m_envelope2;

    // parameter envelope
    float m_oldenv2Delay;
    float m_oldenv2Attack;
    float m_oldenv2Hold;
    float m_oldenv2Decay;
    float m_oldenv2Sustain;
    float m_oldenv2Release;
    float m_oldenv2Level;
    float m_oldenv2Invert;

// Filter
	MoogLadder m_filter;

	float m_oldcutoff;
	float m_oldreso;
    float m_oldmodKeyboard;
    float m_oldmodDepth;


// Noise Source
    std::vector<double> m_noisedata;
    ColoredNoiseGenerator m_noisegen;
    float m_oldwhiteLevel;
    float m_oldwhiteGraininess;
    float m_oldcoloredLow;
    float m_oldcoloredHigh;

// LFOs
    LFO m_lfo1;
    std::vector<double> m_lfo1data;
    float m_oldlfo1level;
    float m_oldlfo1waveform;
    float m_oldlfo1rate;

    LFO m_lfo2;
    std::vector<double> m_lfo2data;
    float m_oldlfo2level;
    float m_oldlfo2waveform;
    float m_oldlfo2rate;

    void updateParameter();

};

#pragma once

#include <JuceHeader.h>

const struct
{
    std::string ID = "VoiceGlobalTuneA0";
    std::string name = "Tuning (global)";
    std::string unitName = " Hz";
    float minValue = 400.f;
    float maxValue = 490.f;
    float defaultValue = 440.f;
}paramGlobalTuneA0;

const struct
{
    std::string ID = "SynthNrOfVoice";
    std::string name = "Number of Voices";
    std::string unitName = "";
    float minValue = 1.f;
    float maxValue = 32.f;
    float defaultValue = 8.f;
}paramGlobalNrOfVoices;

const struct
{
    std::string ID = "VoicePortanmentoTime";
    std::string name = "Portamento";
    std::string unitName = " ms";
    float minValue = log(0.1f);
    float maxValue = log(4000.f);
    float defaultValue = log(0.1f);
}paramGlobalPortamentoTime;


class VoiceParameter
{
public:
    // Lowpass

    int addParameter(std::vector < std::unique_ptr<RangedAudioParameter>>& paramVector);
};



typedef AudioProcessorValueTreeState::SliderAttachment SliderAttachment;
typedef AudioProcessorValueTreeState::ComboBoxAttachment ComboAttachment;

class VoiceParameterComponent : public Component
{
public:
    enum class ComponentStyle
    {
        compact,
        horizontal,
        vertical
    };
    VoiceParameterComponent(AudioProcessorValueTreeState&);

    void paint(Graphics& g) override;
    void resized() override;
    std::function<void()> somethingChanged;
    void setStyle(ComponentStyle style) { m_style = style; };

private:
    AudioProcessorValueTreeState& m_vts;
    ComponentStyle m_style;

    Label m_NrOfVoicesLabel;
    Slider m_NrOfVoicesSlider;
    std::unique_ptr<SliderAttachment> m_NrOfVoicesAttachment;

    Label m_PortamentoTimeLabel;
    Slider m_PortamentoTimeSlider;
    std::unique_ptr<SliderAttachment> m_PortamentoTimeAttachment;

    Label m_GlobalTuneA0Label;
    Slider m_GlobalTuneA0Slider;
    std::unique_ptr<SliderAttachment> m_GlobalTuneA0Attachment;

};

