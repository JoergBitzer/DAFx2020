/*
  ==============================================================================

    This file was auto-generated!

    It contains the basic framework code for a JUCE plugin editor.

  ==============================================================================
*/

#include "PluginProcessor.h"
#include "PluginEditor.h"

//==============================================================================
GuibasicsAudioProcessorEditor::GuibasicsAudioProcessorEditor (GuibasicsAudioProcessor& p)
    : AudioProcessorEditor (&p), m_processor (p)
{
    // Make sure that before the constructor has finished, you've set the
    // editor's size to whatever you need it to be.
    setSize (400, 300);
    addAndMakeVisible(m_combobox);
    m_combobox.addItem("More", 1); // zero is not allowed
    m_combobox.addItem("Less", 2);
    m_combobox.addItem("Item 3", 3);
    m_combobox.onChange = [this]() {changeColorCombo(); };
    m_combobox.setSelectedId(1);
    m_red = 0.5;

    addAndMakeVisible(m_slider);
    m_slider.setRange(0.0, 1.0);
    m_slider.onValueChange = [this]() {changeColorSlider(); };
    m_green = 0.5;

    addAndMakeVisible(m_button);
    m_button.onClick = [this]() {changeButton(); };
    m_button.setButtonText("Blue");
    m_button.setColour(TextButton::ColourIds::buttonColourId, Colours::red);
    m_button.setColour(TextButton::ColourIds::textColourOffId, Colours::blueviolet);
   m_blue = 0.25;

 }

GuibasicsAudioProcessorEditor::~GuibasicsAudioProcessorEditor()
{
}

//==============================================================================
void GuibasicsAudioProcessorEditor::paint (Graphics& g)
{
    // (Our component is opaque, so we must completely fill the background with a solid colour)
    g.fillAll (Colour::fromFloatRGBA(m_red,m_green, m_blue, 1.0f));
    
 }

void GuibasicsAudioProcessorEditor::resized()
{
    // This is generally where you'll want to lay out the positions of any
    // subcomponents in your editor..
    auto area = getBounds();
    m_combobox.setBounds(getWidth() / 2, 40 , 100, 30);

    m_slider.setBounds(20, 5, 300, 20);

    m_button.setBounds(getWidth() / 2, 80, 60, 30);

}

void GuibasicsAudioProcessorEditor::changeColorCombo()
{
    if (m_combobox.getSelectedId() == 1)
        m_red += 0.1;
    
    if (m_combobox.getSelectedId() == 2)
        m_red -= 0.1;

    if (m_red > 1.0)
        m_red = 1.0;

    if (m_red < 0.0)
        m_red = 0.0;
    
    repaint();
}

void GuibasicsAudioProcessorEditor::changeColorSlider()
{
    float help = m_slider.getValue();
    m_green = help;
    repaint();
}

void GuibasicsAudioProcessorEditor::changeButton()
{
    if (m_button.getToggleState() == false)
        m_button.setToggleState(true,NotificationType::dontSendNotification);
    else
        m_button.setToggleState(false,NotificationType::dontSendNotification);



     m_blue += 0.25;

    if (m_blue > 1.0)
        m_blue -= 1.0;

    repaint();
}
