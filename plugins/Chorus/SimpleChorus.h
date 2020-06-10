#pragma once

#include <vector>
#include "LFO/LFO.h"

class SimpleChorus
{
public:
    SimpleChorus();
    void setSamplerate(double samplerate);
    void setMaxBlockSize(int maxSize);
    int processData(std::vector<double>& dataInLeft, std::vector<double>& dataInRight,
        std::vector<double>& dataOutLeft, std::vector<double>& dataOutRight);

    void setNrofChns(int nrofchns) { m_nrofchns = nrofchns; };
    void setFeedback(double feedback) { m_feedback = feedback; };
    void setForward(double forward) { m_forward = forward; };
    void setBlend(double blend) { m_blend = blend; };
    void setLFOMinDelay(double mindelay) { m_lfoMinDelay = mindelay; setMinDelay(); };
    void setLFOMaxDelay(double maxdelay) { m_lfoMaxDelay = maxdelay; setMaxDelay(); };
    void setDelay(double delay) { m_nominalDelay = delay; computeSampleDelay(); };
    void setWidth(double width) {
        m_width = width; computeSampleDelay();
    };
    void setRate(float rate_Hz);
private:
    double m_fs;
    const double m_absolutMaxDelay_ms = 250;
    int m_absolutDelay_samples;
    std::vector<double> m_delayMemoryLeft;
    std::vector<double> m_delayMemoryRight;

    double m_nominalDelay;
    double m_nominalDelay_samples;
    double m_width;

    int m_writeCounter;
    
    LFO m_lfoLeft;
    LFO m_lfoRight;
    std::vector<double> m_lfoDataLeft;
    std::vector<double> m_lfoDataRight;
    double m_lfoRate;
    double m_lfoMinDelay;
    double m_lfoMaxDelay;

    int m_nrofchns;
    double m_feedback;
    double m_forward;
    double m_blend;
    double m_lastDelayValueLeft;
    double m_lastDelayValueRight;
    
    void initChorus();
    void setMinDelay();
    void setMaxDelay();
    void computeSampleDelay();

};

