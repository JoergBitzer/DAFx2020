/*
  ==============================================================================

    MultiTapDelay.h
    Created: 11 May 2020 6:53:28am
    Author:  bitzer

  ==============================================================================
*/

#pragma once

#include <vector>

class MultiTapDelay
{
    struct Tap
    {
        double forward;
        double feedback;
        double delay_ms;
        double delay_samples;
        bool isTimeVariant;
        double minDelay_ms;
        double maxDelay_ms;
        LFO::form lfoform;

    };
public:
    MultiTapDelay();

private:
    int m_writeCounter;
    int m_maxDelay;
    std::vector<std::vector<double>> m_delayLine;
    int m_nrOfTaps;
    std::vector<std::vector<Tap>> m_Taps;
    int m_nrofchannels;
    int m_blend;




private:
};