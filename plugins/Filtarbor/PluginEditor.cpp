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
    setSize (400, 300);

    // Lowpass
    m_cutoffLpLabel.setText(paramLpCutoff.name, dontSendNotification);
    addAndMakeVisible(m_cutoffLpLabel);

    m_orderLpLabel.setText(paramLpOrder.name, dontSendNotification);
    addAndMakeVisible(m_orderLpLabel);
    
    m_cutoffLpSlider.setComponentID(paramLpCutoff.ID);
    m_cutoffLpSlider.addListener(this);
    m_cutoffLpAttachment = std::make_unique<SliderAttachment>(m_paramVTS, paramLpCutoff.ID, m_cutoffLpSlider);
    addAndMakeVisible(m_cutoffLpSlider);
    m_cutoffLpSlider.setValue(paramLpCutoff.defaultValue);

    m_orderLpSlider.setComponentID(paramLpOrder.ID);
    m_orderLpSlider.addListener(this);
    m_orderLpAttachment = std::make_unique<SliderAttachment>(m_paramVTS, paramLpOrder.ID, m_orderLpSlider);
    addAndMakeVisible(m_orderLpSlider);
    m_orderLpSlider.setValue(paramLpOrder.defaultValue);

    // Highpass
    m_cutoffHpLabel.setText(paramHpCutoff.name, dontSendNotification);
    addAndMakeVisible(m_cutoffHpLabel);

    m_orderHpLabel.setText(paramHpOrder.name, dontSendNotification);
    addAndMakeVisible(m_orderHpLabel);

    m_cutoffHpSlider.setComponentID(paramHpCutoff.ID);
    m_cutoffHpSlider.addListener(this);
    m_cutoffHpAttachment = std::make_unique<SliderAttachment>(m_paramVTS, paramHpCutoff.ID, m_cutoffHpSlider);
    addAndMakeVisible(m_cutoffHpSlider);
    m_cutoffHpSlider.setValue(paramHpCutoff.defaultValue);

    m_orderHpSlider.setComponentID(paramHpOrder.ID);
    m_orderHpSlider.addListener(this);
    m_orderHpAttachment = std::make_unique<SliderAttachment>(m_paramVTS, paramHpOrder.ID, m_orderHpSlider);
    addAndMakeVisible(m_orderHpSlider);
    m_orderHpSlider.setValue(paramHpOrder.defaultValue);
}

FilarborAudioProcessorEditor::~FilarborAudioProcessorEditor()
{
}

//==============================================================================
void FilarborAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (Colour::fromFloatRGBA(0.25, 0.25, 0.25, 1.0));


}

#define GUI_HEIGHT_ELEMENT 20
#define GUI_LABEL_WIDTH 100

void FilarborAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..

    auto r = getLocalBounds();
    r.removeFromTop(GUI_HEIGHT_ELEMENT);
    auto lowpassRect = r;
    lowpassRect.setHeight(GUI_HEIGHT_ELEMENT);
    m_cutoffLpLabel.setBounds(lowpassRect.removeFromLeft(GUI_LABEL_WIDTH));
    m_cutoffLpSlider.setBounds(lowpassRect);
    
    r.removeFromTop(GUI_HEIGHT_ELEMENT);
    auto orderLpRect = r;
    orderLpRect.setHeight(GUI_HEIGHT_ELEMENT);
    m_orderLpLabel.setBounds(orderLpRect.removeFromLeft(GUI_LABEL_WIDTH));
    m_orderLpSlider.setBounds(orderLpRect);
    
    r.removeFromTop(2*GUI_HEIGHT_ELEMENT);
    auto highpassRect = r;
    highpassRect.setHeight(GUI_HEIGHT_ELEMENT);
    m_cutoffHpLabel.setBounds(highpassRect.removeFromLeft(GUI_LABEL_WIDTH));
    m_cutoffHpSlider.setBounds(highpassRect);
    
    r.removeFromTop(GUI_HEIGHT_ELEMENT);
    auto orderHpRect = r;
    orderHpRect.setHeight(GUI_HEIGHT_ELEMENT);
    m_orderHpLabel.setBounds(orderHpRect.removeFromLeft(GUI_LABEL_WIDTH));
    m_orderHpSlider.setBounds(orderHpRect);
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
