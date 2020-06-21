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
    m_osc1(vts,0,"Osc1"),m_env1(vts,0,"EnvelopeVCA")
{
    ScopedLock sp();
    addAndMakeVisible(m_presetGUI);
    addAndMakeVisible(m_keyboard);

    addAndMakeVisible(m_osc1);
    m_osc1.somethingChanged = [this]() {m_presetGUI.setSomethingChanged(); };

    addAndMakeVisible(m_env1);
    m_env1.somethingChanged = [this]() {m_presetGUI.setSomethingChanged(); };

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (1000, 600);
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
#define PLUGIN_MIN_DISTANCE 5

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

    rinner = r;
    s = rinner.removeFromTop(ENV_HEIGHT_HORZ);
    s.removeFromLeft(OSC_WIDTH + PLUGIN_MIN_DISTANCE);
//    s = rinner.removeFromLeft(OSC_WIDTH + PLUGIN_MIN_DISTANCE);
    m_env1.setBounds(s.removeFromLeft(ENV_WIDTH_HORZ));


}
