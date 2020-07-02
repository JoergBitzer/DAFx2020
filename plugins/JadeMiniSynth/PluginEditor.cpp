/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
JadeMiniSynthAudioProcessorEditor::JadeMiniSynthAudioProcessorEditor(JadeMiniSynthAudioProcessor& p, AudioProcessorValueTreeState& vts, PresetHandler& ph)
    : AudioProcessorEditor(&p), m_processor(p), m_vts(vts), m_presetGUI(ph), m_keyboard(p.m_keyboardState, 
        MidiKeyboardComponent::Orientation::horizontalKeyboard),
    m_osc1(vts,0,"Osc1"), m_osc2(vts, 1, "Osc2"), m_env1(vts,0,"EnvelopeVCA"),
    m_moogladder(vts),m_noise(vts), 
    m_env2(vts, 1, "EnvelopeFilter"),
    m_lfo1(vts, 0, "LFOFilter")
{
    ScopedLock sp();
    addAndMakeVisible(m_presetGUI);
    addAndMakeVisible(m_keyboard);

    addAndMakeVisible(m_osc1);
    m_osc1.somethingChanged = [this]() {m_presetGUI.setSomethingChanged(); };
    addAndMakeVisible(m_osc2);
    m_osc2.somethingChanged = [this]() {m_presetGUI.setSomethingChanged(); };


    addAndMakeVisible(m_env1);
    m_env1.somethingChanged = [this]() {m_presetGUI.setSomethingChanged(); };

    addAndMakeVisible(m_env2);
    m_env2.somethingChanged = [this]() {m_presetGUI.setSomethingChanged(); };
    m_env2.setShowDelay();
    m_env2.setShowLevel();
    m_env2.setShowInvert();

    addAndMakeVisible(m_moogladder);
    m_moogladder.somethingChanged = [this]() {m_presetGUI.setSomethingChanged(); };

    addAndMakeVisible(m_noise);
    m_noise.somethingChanged = [this]() {m_presetGUI.setSomethingChanged(); };

    addAndMakeVisible(m_lfo1);
    m_lfo1.somethingChanged = [this]() {m_presetGUI.setSomethingChanged(); };


    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (1200, 600);
}

JadeMiniSynthAudioProcessorEditor::~JadeMiniSynthAudioProcessorEditor()
{
}

//==============================================================================
void JadeMiniSynthAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
}
#define GUI_ELEMENT_MIN_DISTANCE 5
#define PRESETHANDLER_HEIGHT 30
#define KEYBOARD_HEIGHT 50

#define OSC_WIDTH 260
#define OSC_HEIGHT 150

#define ENV_HEIGHT_HORZ 90
#define ENV_WIDTH_HORZ 330
#define ENV_FULLWIDTH_HORZ 525
#define PLUGIN_MIN_DISTANCE 5

#define MOOG_HEIGHT_HORZ 90
#define MOOG_WIDTH_HORZ 250

#define NOISE_HEIGHT_HORZ 90
#define NOISE_WIDTH_HORZ 250

#define LFO_HEIGHT_HORZ 90
#define LFO_WIDTH_HORZ 220


void JadeMiniSynthAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto r = getBounds();
    m_presetGUI.setBounds(r.removeFromTop(PRESETHANDLER_HEIGHT));
    m_keyboard.setBounds(r.removeFromBottom(KEYBOARD_HEIGHT));

    auto rinner = r;
    auto s = rinner.removeFromTop(OSC_HEIGHT);
    m_osc1.setBounds(s.removeFromLeft(OSC_WIDTH));
    rinner.removeFromTop(PLUGIN_MIN_DISTANCE);
    s = rinner.removeFromTop(OSC_HEIGHT);
    m_osc2.setBounds(s.removeFromLeft(OSC_WIDTH));


    rinner = r;
    s = rinner.removeFromTop(ENV_HEIGHT_HORZ);
    s.removeFromLeft(OSC_WIDTH + PLUGIN_MIN_DISTANCE);
//    s = rinner.removeFromLeft(OSC_WIDTH + PLUGIN_MIN_DISTANCE);
    m_env1.setBounds(s.removeFromLeft(ENV_WIDTH_HORZ));

    m_moogladder.setBounds(r.getWidth() / 2 - MOOG_WIDTH_HORZ / 2, r.getHeight() / 2 - MOOG_HEIGHT_HORZ / 2, MOOG_WIDTH_HORZ, MOOG_HEIGHT_HORZ);
    m_env2.setBounds(r.getWidth() / 2 - ENV_FULLWIDTH_HORZ / 2, r.getHeight() / 2 + MOOG_HEIGHT_HORZ / 2 + GUI_ELEMENT_MIN_DISTANCE ,ENV_FULLWIDTH_HORZ, ENV_HEIGHT_HORZ);
    m_lfo1.setBounds(r.getWidth() / 2 - LFO_WIDTH_HORZ / 2, r.getHeight() / 2 + MOOG_HEIGHT_HORZ / 2 + ENV_HEIGHT_HORZ + 2* GUI_ELEMENT_MIN_DISTANCE, LFO_WIDTH_HORZ, LFO_HEIGHT_HORZ);


    rinner = r;
    m_noise.setBounds((rinner.removeFromBottom(NOISE_HEIGHT_HORZ)).removeFromLeft(NOISE_WIDTH_HORZ));


}
