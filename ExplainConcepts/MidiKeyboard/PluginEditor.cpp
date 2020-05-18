/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#define GUI_WIDTH 800

//==============================================================================
NanoAudioProcessorEditor::NanoAudioProcessorEditor (NanoAudioProcessor& p)
    : AudioProcessorEditor (&p), m_processor (p),
    m_MidiKeyboard(m_MidiState,MidiKeyboardComponent::Orientation::horizontalKeyboard)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (GUI_WIDTH, 300);
    addAndMakeVisible(m_MidiKeyboard);
}

NanoAudioProcessorEditor::~NanoAudioProcessorEditor()
{
}

//==============================================================================
void NanoAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

}

void NanoAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    m_MidiKeyboard.setBounds(0, 200, getWidth(), 100);
}
