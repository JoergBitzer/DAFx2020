/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"
#include "Parameter.h"

//==============================================================================
FilarborAudioProcessorEditor::FilarborAudioProcessorEditor (FilarborAudioProcessor& p, AudioProcessorValueTreeState& vts)
    : AudioProcessorEditor (&p), m_processor (p), m_paramVTS(vts)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (500, 120);

    // Lowpass
    m_cutoffLpLabel.setText("Lowpass", dontSendNotification);
    m_cutoffLpLabel.setFont(Font("Arial", 30.0, Font::plain));
    addAndMakeVisible(m_cutoffLpLabel);

    //m_orderLpLabel.setText(paramLpOrder.name, dontSendNotification);
    //addAndMakeVisible(m_orderLpLabel);
    
    m_cutoffLpSlider.setComponentID(paramLpCutoff.ID);
    m_cutoffLpSlider.addListener(this);
    m_cutoffLpSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxAbove, true, 50, 20);
    m_cutoffLpAttachment = std::make_unique<SliderAttachment>(m_paramVTS, paramLpCutoff.ID, m_cutoffLpSlider);
    addAndMakeVisible(m_cutoffLpSlider);
    m_cutoffLpSlider.setValue(paramLpCutoff.defaultValue);

    m_orderLpSlider.setComponentID(paramLpOrder.ID);
    m_orderLpSlider.addListener(this);
    m_orderLpSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    m_orderLpSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 50, 20);
    m_orderLpAttachment = std::make_unique<SliderAttachment>(m_paramVTS, paramLpOrder.ID, m_orderLpSlider);
    
    addAndMakeVisible(m_orderLpSlider);
    m_orderLpSlider.setValue(paramLpOrder.defaultValue);

    // Highpass
    m_cutoffHpLabel.setText("Highpass", dontSendNotification);
    m_cutoffHpLabel.setFont(Font("Arial", 30.0, Font::plain));
    addAndMakeVisible(m_cutoffHpLabel);

 
    m_cutoffHpSlider.setComponentID(paramHpCutoff.ID);
    m_cutoffHpSlider.addListener(this);
    m_cutoffHpAttachment = std::make_unique<SliderAttachment>(m_paramVTS, paramHpCutoff.ID, m_cutoffHpSlider);
    m_cutoffHpSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 20);
    m_cutoffHpSlider.setValue(paramHpCutoff.defaultValue);
    addAndMakeVisible(m_cutoffHpSlider);
  
 
    m_orderHpSlider.setComponentID(paramHpOrder.ID);
    m_orderHpSlider.addListener(this);
    m_orderHpSlider.setSliderStyle(Slider::SliderStyle::Rotary);
    m_orderHpSlider.setTextBoxStyle(Slider::TextBoxBelow, true, 50, 20);
    m_orderHpSlider.setValue(paramHpOrder.defaultValue);
    m_orderHpAttachment = std::make_unique<SliderAttachment>(m_paramVTS, paramHpOrder.ID, m_orderHpSlider);
    addAndMakeVisible(m_orderHpSlider);
    //*/
    }

FilarborAudioProcessorEditor::~FilarborAudioProcessorEditor()
{
}

//==============================================================================
void FilarborAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (getLookAndFeel().findColour(ResizableWindow::backgroundColourId));
}

#define GUI_HEIGHT_ELEMENT 50
#define GUI_LABEL_WIDTH 100
#define GUI_LABEL_HEIGHT 30
#define GUI_ELEMENT_DISTANCE 60

void FilarborAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    auto r = getLocalBounds();
    auto lowpassRect = r;
    m_cutoffLpLabel.setBounds(lowpassRect.removeFromLeft(GUI_LABEL_WIDTH).removeFromTop(GUI_LABEL_HEIGHT));
    lowpassRect.setWidth(r.getWidth() - 2 * GUI_LABEL_WIDTH - GUI_ELEMENT_DISTANCE);
    m_cutoffLpSlider.setBounds(lowpassRect.removeFromTop(GUI_HEIGHT_ELEMENT));
    
    auto orderLpRect = r;
    m_orderLpSlider.setBounds(orderLpRect.removeFromLeft(GUI_LABEL_WIDTH).removeFromBottom(GUI_LABEL_WIDTH));
 
    auto highPassRect = r;
    m_cutoffHpLabel.setBounds(highPassRect.removeFromRight(GUI_LABEL_WIDTH).removeFromTop(GUI_LABEL_HEIGHT));

    highPassRect = r;
    highPassRect.removeFromLeft(GUI_LABEL_WIDTH + GUI_ELEMENT_DISTANCE);
    highPassRect.setWidth(r.getWidth() - 2 * GUI_LABEL_WIDTH - GUI_ELEMENT_DISTANCE);
    m_cutoffHpSlider.setBounds(highPassRect.removeFromBottom(GUI_HEIGHT_ELEMENT));

    auto orderHpRect = r;
    m_orderHpSlider.setBounds(orderHpRect.removeFromRight(GUI_LABEL_WIDTH).removeFromBottom(GUI_LABEL_WIDTH));

    //*/
}

void FilarborAudioProcessorEditor::sliderValueChanged(Slider* slider)
{
    float value;
    if (slider == &m_cutoffLpSlider)
    {
        value = m_cutoffLpSlider.getValue();
        m_processor.setCutoffLowpass(exp(value));
    }
    if (slider == &m_orderLpSlider)
    {
        value = m_orderLpSlider.getValue();
        m_processor.setOrderLowpass(value);
    }
    if (slider == &m_cutoffHpSlider)
    {
        value = m_cutoffHpSlider.getValue();
        m_processor.setCutoffHighpass(exp(value));
    }
    if (slider == &m_orderHpSlider)
    {
        value = m_orderHpSlider.getValue();
        m_processor.setOrderHighpass(value);
    }
}
