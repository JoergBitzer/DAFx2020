/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
SimpleChorusAudioProcessorEditor::SimpleChorusAudioProcessorEditor (SimpleChorusAudioProcessor& p, AudioProcessorValueTreeState & vts, PresetHandler &ph)
    : AudioProcessorEditor (&p), m_processor (p),m_vts(vts),m_lfo1params(m_vts,0,"LFO_1"), m_chorusparams(m_vts), m_presetGUI(ph)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
	ScopedLock sp();

	m_presetGUI.setNoCategory();
	addAndMakeVisible(m_presetGUI);
	addAndMakeVisible(m_lfo1params);
	m_lfo1params.somethingChanged = [this]() {m_presetGUI.setSomethingChanged(); };
	addAndMakeVisible(m_chorusparams);
	m_chorusparams.somethingChanged = [this]() {m_presetGUI.setSomethingChanged(); };
	
    setSize (600, 250);
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
//    g.setFont (15.0f);
//    g.drawFittedText ("Chorus", getLocalBounds(), Justification::centredTop, 1);
}

#define LFO_WIDTH 180
#define LFO_HEIGHT 80
#define GUI_ELEMENT_MIN_DISTANCE 5
#define PRESETHANDLER_HEIGHT 30
void SimpleChorusAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
	m_presetGUI.setBounds(0, 0, getWidth(), PRESETHANDLER_HEIGHT);
	auto r = getBounds();
	r.removeFromTop(PRESETHANDLER_HEIGHT);
	r.removeFromRight(GUI_ELEMENT_MIN_DISTANCE);
	r.removeFromLeft(GUI_ELEMENT_MIN_DISTANCE);
	m_lfo1params.setBounds(r.removeFromBottom(LFO_HEIGHT/2));
	r.removeFromBottom(GUI_ELEMENT_MIN_DISTANCE);
	m_chorusparams.setBounds(r);
}
