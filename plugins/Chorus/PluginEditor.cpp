/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleChorusAudioProcessorEditor::SimpleChorusAudioProcessorEditor (SimpleChorusAudioProcessor& p, AudioProcessorValueTreeState & vts)
    : AudioProcessorEditor (&p), m_processor (p),m_vts(vts),m_lfo1params(m_vts,0,"LFO_1")
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
	addAndMakeVisible(m_lfo1params);
    setSize (400, 300);
}

SimpleChorusAudioProcessorEditor::~SimpleChorusAudioProcessorEditor()
{
}

//==============================================================================
void SimpleChorusAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));

    g.setColour (Colours::blue);
    g.setFont (15.0f);
    g.drawFittedText ("Chorus", getLocalBounds(), Justification::centredTop, 1);
}

#define LFO_WIDTH 180
#define LFO_HEIGHT 80


void SimpleChorusAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
	m_lfo1params.setBounds(getWidth() / 2 - LFO_WIDTH / 2, getHeight() - LFO_HEIGHT, LFO_WIDTH, LFO_HEIGHT);
}
