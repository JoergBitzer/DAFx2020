/*
  ==============================================================================

    SimpleChorus.cpp
    Created: 13 May 2020 10:24:49pm
    Author:  Bitzer

  ==============================================================================
*/

#include "SimpleChorus.h"

SimpleChorus::SimpleChorus()
    :m_lastDelayValueLeft(0.0),
    m_lastDelayValueRight(0.0)
{
    m_fs = 44100.0;
    m_feedback = 0.0;
    m_forward = 0.707;
    m_blend = 1.0;
    m_lfoMinDelay = 3.0;
    m_lfoMaxDelay = 8.5;
    m_lfoLeft.setLFOFunction(LFO::LFOFunctions::sinus);
    m_lfoRight.setLFOFunction(LFO::LFOFunctions::sinus);
    m_lfoLeft.setFrequency(3.5);
    m_lfoRight.setFrequency(3.5);
    m_lfoRight.setStartphase(3.14 / 2.0);//Einziger Unterschied zwischen Links und Rechts

    initChorus();
}

void SimpleChorus::setSamplerate(double samplerate)
{
    m_fs = samplerate;
    m_lfoLeft.setSamplerate(m_fs);
    m_lfoRight.setSamplerate(m_fs);
    setMinDelay();
    setMaxDelay();
    initChorus();
}

void SimpleChorus::setMaxBlockSize(int maxSize)
{
    m_lfoDataLeft.resize(maxSize);
    m_lfoDataRight.resize(maxSize);
}

int SimpleChorus::processData(std::vector<double>& dataInLeft, std::vector<double>& dataInRight, std::vector<double>& dataOutLeft, std::vector<double>& dataOutRight)
{
    m_lfoLeft.getData(m_lfoDataLeft);
    m_lfoRight.getData(m_lfoDataRight);

    for (auto kk = 0U; kk < dataInLeft.size(); ++kk)
    {
        double curDelayLeft = m_lfoDataLeft[kk];
        int curDelayLeftInt = static_cast<int>(curDelayLeft);
        double fracDelayLeft = curDelayLeft - curDelayLeftInt;

        double curDelayRight = m_lfoDataRight[kk];
        int curDelayRightInt = static_cast<int>(curDelayRight);
        double fracDelayRight = curDelayRight - curDelayRightInt;

        double curInRight;
        double curInLeft = dataInLeft[kk] - m_feedback*m_lastDelayValueLeft;

        m_delayMemoryLeft[m_writeCounter] = curInLeft;
        if (m_nrofchns == 2)
        {
            curInRight = dataInRight[kk] - m_feedback * m_lastDelayValueRight;
            m_delayMemoryRight[m_writeCounter] = curInRight;
        }

        double outLeft, outRight;
        int leftReadPos = m_writeCounter - curDelayLeftInt;
        if (leftReadPos < 0)
            leftReadPos += m_absolutDelay_samples;

        int leftReadFeedbackPos = m_writeCounter - m_nominalDelay_samples;
        if (leftReadFeedbackPos < 0)
            leftReadFeedbackPos += m_absolutDelay_samples;


        int nextleftReadPos = leftReadPos - 1;
        if (nextleftReadPos < 0)
            nextleftReadPos += m_absolutDelay_samples;
//        if (nextleftReadPos == m_absolutDelay_samples)
//            nextleftReadPos = 0;

        outLeft = (1.0-fracDelayLeft)*m_delayMemoryLeft[leftReadPos] + 
            fracDelayLeft*m_delayMemoryLeft[nextleftReadPos];



        m_lastDelayValueLeft = m_delayMemoryLeft[leftReadFeedbackPos];

        dataOutLeft[kk] = m_forward * outLeft + m_blend * curInLeft;


        if (m_nrofchns == 2)
        {
            int rightReadPos = m_writeCounter - curDelayRightInt;
            if (rightReadPos < 0)
                rightReadPos += m_absolutDelay_samples;

            int rightReadFeedbackPos = m_writeCounter - m_nominalDelay_samples;
            if (rightReadFeedbackPos < 0)
                rightReadFeedbackPos += m_absolutDelay_samples;

            int nextrightReadPos = rightReadPos - 1;
            if (nextrightReadPos < 0)
                nextrightReadPos += m_absolutDelay_samples;;
            
            outRight = (1.0 - fracDelayRight)*m_delayMemoryRight[rightReadPos] +
                fracDelayRight*m_delayMemoryRight[nextrightReadPos];

            m_lastDelayValueRight = m_delayMemoryRight[rightReadFeedbackPos];

            dataOutRight[kk] = m_forward * outRight + m_blend * curInRight;
            
        }

        ++m_writeCounter;
        if (m_writeCounter == m_absolutDelay_samples)
            m_writeCounter = 0;
    }
    return 0;
}

void SimpleChorus::setRate(float rate_Hz)
{
    m_lfoLeft.setFrequency(rate_Hz);
    m_lfoRight.setFrequency(rate_Hz);
}

void SimpleChorus::initChorus()
{
    m_absolutDelay_samples = static_cast<int> (m_absolutMaxDelay_ms * 0.001 * m_fs + 0.5);
    m_delayMemoryLeft.resize(m_absolutDelay_samples);
    m_delayMemoryRight.resize(m_absolutDelay_samples);
    m_writeCounter = 0;
 
}

void SimpleChorus::setMinDelay()
{
    m_nominalDelay = (m_lfoMaxDelay - m_lfoMinDelay)*0.5;
    m_nominalDelay_samples = int(m_nominalDelay * 0.001 * m_fs + 0.5);
    if (m_nominalDelay_samples <= 0)
        m_nominalDelay_samples = 1;
    m_width = (1.0 - m_lfoMinDelay / m_nominalDelay) * 100.0;
    m_lfoLeft.setMin(m_lfoMinDelay * 0.001 * m_fs);
    m_lfoRight.setMin(m_lfoMinDelay * 0.001 * m_fs);
}

void SimpleChorus::setMaxDelay()
{
    m_nominalDelay = (m_lfoMaxDelay - m_lfoMinDelay)*0.5;
    m_nominalDelay_samples = int(m_nominalDelay * 0.001 * m_fs + 0.5);
    if (m_nominalDelay_samples <= 0)
        m_nominalDelay_samples = 1;
    m_width = (1.0-m_lfoMinDelay/ m_nominalDelay)*100.0;

    m_lfoLeft.setMax(m_lfoMaxDelay * 0.001 * m_fs);
    m_lfoRight.setMax(m_lfoMaxDelay * 0.001 * m_fs);
}

void SimpleChorus::computeSampleDelay()
{
    m_nominalDelay_samples = int(m_nominalDelay * 0.001 * m_fs + 0.5);
    if (m_nominalDelay_samples <= 0)
        m_nominalDelay_samples = 1;

    m_lfoMinDelay = m_nominalDelay * (1.0 - m_width * 0.01);
    if (m_lfoMinDelay <= 0.0)
        m_lfoMinDelay = 1.0 / m_fs * 1000.0;
    m_lfoMaxDelay = m_nominalDelay * (1.0 + m_width * 0.01);
    if (m_lfoMaxDelay >= m_absolutMaxDelay_ms)
        m_lfoMaxDelay = m_absolutMaxDelay_ms - 1.0 / m_fs * 1000.0;

    m_lfoLeft.setMin(m_lfoMinDelay * 0.001 * m_fs);
    m_lfoRight.setMin(m_lfoMinDelay * 0.001 * m_fs);
    m_lfoLeft.setMax(m_lfoMaxDelay * 0.001 * m_fs);
    m_lfoRight.setMax(m_lfoMaxDelay * 0.001 * m_fs);
}
