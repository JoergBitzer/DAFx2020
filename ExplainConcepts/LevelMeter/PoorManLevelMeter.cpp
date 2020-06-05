/*
  ==============================================================================

    PoorManLevelMeter.cpp
    Created: 20 May 2020 4:12:16pm
    Author:  Bitzer

  ==============================================================================
*/

#include <JuceHeader.h>
#include "PoorManLevelMeter.h"

//==============================================================================
PoorManLevelMeter::PoorManLevelMeter()
{
    // In your constructor, you should add any child components, and
    // initialise any special settings that your component needs.

}

PoorManLevelMeter::~PoorManLevelMeter()
{
}

void PoorManLevelMeter::paint (Graphics& g)
{
    /* This demo code just fills the component's background and
       draws some placeholder text to get you started.

       You should replace everything in this method with your own
       drawing code..
    */

    g.fillAll (getLookAndFeel().findColour (ResizableWindow::backgroundColourId));   // clear the background

    g.setColour (Colours::grey);
    g.drawRect (getLocalBounds(), 1);   // draw an outline around the component

    g.setColour (Colours::green);
    
    double level_min = -80.0;
    double level_max = 10.0;

    if (m_level < level_min)
        m_level = level_min;

    if (m_level > level_max)
        m_level = level_max;

    double rel_level = (m_level - level_min)/(level_max - level_min);

    g.drawLine(getWidth() / 2, getHeight(), getWidth() / 2, getHeight() * (1.0 - rel_level), 20.0);

}

void PoorManLevelMeter::resized()
{
    // This method is where you should set the bounds of any child
    // components that your component contains..

}
