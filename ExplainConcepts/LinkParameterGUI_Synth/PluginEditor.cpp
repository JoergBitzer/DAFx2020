/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

#define GUI_WIDTH 800
#define GUI_HEIGHT 500

//==============================================================================
NanoAudioProcessorEditor::NanoAudioProcessorEditor (NanoAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), m_processor (p),m_vts(vts),
    m_MidiKeyboard(m_processor.m_keyboardState,MidiKeyboardComponent::Orientation::horizontalKeyboard)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    addAndMakeVisible(m_MidiKeyboard);
    //m_backgroundImage = ImageCache::getFromMemory(BinaryData::Hintergrund_jpg, BinaryData::Hintergrund_jpgSize);

    m_cutoffSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    m_cutoffSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxAbove, true, 60, 20);
    addAndMakeVisible(m_cutoffSlider);
    m_cutoffAttachment = std::make_unique<SliderAttachment>(m_vts, paramCutoff.ID, m_cutoffSlider);

    m_resoSlider.setSliderStyle(Slider::SliderStyle::LinearVertical);
    m_resoSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxAbove, true, 60, 20);
    addAndMakeVisible(m_resoSlider);
    m_resoAttachment = std::make_unique<SliderAttachment>(m_vts, paramResonance.ID, m_resoSlider);
    
    setSize(GUI_WIDTH, GUI_HEIGHT);

}

NanoAudioProcessorEditor::~NanoAudioProcessorEditor()
{
}

//==============================================================================
void NanoAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));
    //g.drawImage(m_backgroundImage, getLocalBounds().toFloat());
}

void NanoAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    m_MidiKeyboard.setBounds(0, getHeight()-50, getWidth(), 50);

    m_cutoffSlider.setBounds(20, 40, 50, 300);
    m_resoSlider.setBounds(90, 40, 50, 300);
}
