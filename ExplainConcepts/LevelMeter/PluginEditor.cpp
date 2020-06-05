/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
DatenAnzeigeAudioProcessorEditor::DatenAnzeigeAudioProcessorEditor (DatenAnzeigeAudioProcessor& p)
    : AudioProcessorEditor (&p), m_processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    addAndMakeVisible(m_textdisplay);
    addAndMakeVisible(m_level);
    setSize (400, 300);
    startTimerHz(30.0);
}

DatenAnzeigeAudioProcessorEditor::~DatenAnzeigeAudioProcessorEditor()
{
}

//==============================================================================
void DatenAnzeigeAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

}

void DatenAnzeigeAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    m_textdisplay.setBounds(20, 20, 120, 40);
    m_level.setBounds(150, 20, 30, getHeight() - 20);
}

void DatenAnzeigeAudioProcessorEditor::timerCallback()
{
    double rms = m_processor.getRMS();
    rms = 20.0 * log10(rms + 0.0000000001);
    m_textdisplay.setButtonText(String(rms) + " dB");
    m_level.setLevel(rms);
}
