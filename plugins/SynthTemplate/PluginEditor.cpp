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
    : AudioProcessorEditor(&p), m_processor(p), m_vts(vts), m_presetGUI(ph), m_keyboard(p.m_keyboardState, MidiKeyboardComponent::Orientation::horizontalKeyboard)
{
    ScopedLock sp();
    addAndMakeVisible(m_presetGUI);
    addAndMakeVisible(m_keyboard);

    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (800, 600);
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
void JadeMiniSynthAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto r = getBounds();
    m_presetGUI.setBounds(r.removeFromTop(PRESETHANDLER_HEIGHT));
    m_keyboard.setBounds(r.removeFromBottom(KEYBOARD_HEIGHT));


}
