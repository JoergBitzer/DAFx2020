/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "MoogLadder.h"

//==============================================================================
NanoAudioProcessorEditor::NanoAudioProcessorEditor(NanoAudioProcessor& p, 
	AudioProcessorValueTreeState& vts, PresetHandler& ph)
	: AudioProcessorEditor(&p), m_processor(p),
	m_keyboard(m_processor.m_keyboardState, MidiKeyboardComponent::Orientation::horizontalKeyboard),
	m_paramVTS(vts), m_presetGUI(ph)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    // setSize (700, 500);
    // backgroundImage = ImageCache::getFromMemory(BinaryData::Hintergrund_jpg, BinaryData::Hintergrund_jpgSize);
    // apply placement flags if needed (https://docs.juce.com/master/classRectanglePlacement.html)
    //backgroundImage.setImagePlacement(RectanglePlacement(RectanglePlacement::stretchToFit));
    // add this first so it is furthest back in z order
    //addAndMakeVisible(backgroundImage);

    addAndMakeVisible(m_keyboard);
	
	m_cutoffSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	m_cutoffSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxAbove,true,60,20);
	m_cutoffSlider.onValueChange = [this]() {m_presetGUI.setSomethingChanged(); };
	addAndMakeVisible(m_cutoffSlider);

	m_cutoffAttachment = std::make_unique<SliderAttachment>(m_paramVTS, paramCutoff.ID, m_cutoffSlider);
	
	//m_cutoffSlider.setValue(paramCutoff.defaultValue);
	
//	Value cut = m_paramVTS.getParameterAsValue(paramCutoff.ID);
//	cut = 6.0;
	
	//m_cutoffLpSlider.onValueChange = [this]() {m_minidisp.setLPFreq(m_cutoffLpSlider.getValue()); };
	
	m_resonanceAttachment = std::make_unique<SliderAttachment>(m_paramVTS, paramResonance.ID, m_resonanceSlider);
//	m_resonanceSlider.setValue(paramResonance.defaultValue);
	m_resonanceSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
	m_resonanceSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxAbove, true, 60, 20);
	m_resonanceSlider.onValueChange = [this]() {m_presetGUI.setSomethingChanged(); };

	addAndMakeVisible(m_resonanceSlider);
	addAndMakeVisible(m_presetGUI);
    setSize (700, 400);
 
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
    //backgroundImage.setBounds(getLocalBounds());
    m_keyboard.setBounds(0, getHeight()-50, getWidth(), 50);
	m_cutoffSlider.setBounds(20, 40, 50, 300);
	m_resonanceSlider.setBounds(100, 40, 50, 300);
	m_presetGUI.setBounds(0, 0, getWidth(),30);
}

