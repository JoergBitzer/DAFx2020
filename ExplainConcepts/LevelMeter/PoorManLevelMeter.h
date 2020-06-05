/*
  ==============================================================================

    PoorManLevelMeter.h
    Created: 20 May 2020 4:12:16pm
    Author:  Bitzer

  ==============================================================================
*/

#pragma once

#include <JuceHeader.h>

//==============================================================================
/*
*/
class PoorManLevelMeter    : public Component
{
public:
    PoorManLevelMeter();
    ~PoorManLevelMeter();

    void paint (Graphics&) override;
    void resized() override;
    void setLevel(double level_dB) { m_level = level_dB; repaint(); };

private:
    double m_level;
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PoorManLevelMeter)
};
