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
	m_lowLeftForward.setSamplerate(m_fs);
	m_lowLeftForward.setDesignroutine(FirstOrderFilter::FilterDesign::lowpassButter);
	m_lowLeftForward.setCutoff(4000.0);

	m_highLeftForward.setSamplerate(m_fs);
	m_highLeftForward.setDesignroutine(FirstOrderFilter::FilterDesign::lowpassShelv);
	m_highLeftForward.setGainIndB(-6.0);
	m_highLeftForward.setCutoff(1500.0);
	
	m_lowRightForward.setSamplerate(m_fs);
	m_lowRightForward.setDesignroutine(FirstOrderFilter::FilterDesign::lowpassButter);
	m_lowRightForward.setCutoff(4000.0);



	m_highRightForward.setSamplerate(m_fs);
	m_highRightForward.setDesignroutine(FirstOrderFilter::FilterDesign::lowpassShelv);
	m_highRightForward.setGainIndB(-6.0);
	m_highRightForward.setCutoff(1500.0);


	m_lowLeftFeedback.setSamplerate(m_fs);
	m_lowLeftFeedback.setDesignroutine(FirstOrderFilter::FilterDesign::lowpassButter);
	m_lowLeftFeedback.setCutoff(6000.0);


	m_highLeftFeedback.setSamplerate(m_fs);
	m_highLeftFeedback.setDesignroutine(FirstOrderFilter::FilterDesign::lowpassShelv);
	m_highLeftFeedback.setGainIndB(-2.0);
	m_highLeftFeedback.setCutoff(1500.0);

	
	
	m_lowRightFeedback.setSamplerate(m_fs);
	m_lowRightFeedback.setDesignroutine(FirstOrderFilter::FilterDesign::lowpassButter);
	m_lowRightFeedback.setCutoff(6000.0);

	m_highRightFeedback.setSamplerate(m_fs);
	m_highRightFeedback.setDesignroutine(FirstOrderFilter::FilterDesign::lowpassShelv);
	m_highRightFeedback.setGainIndB(-2.0);
	m_highRightFeedback.setCutoff(1500.0);

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


		double feed_l = m_delayMemoryLeft[leftReadFeedbackPos];
		feed_l = m_lowLeftFeedback.processOneSample(feed_l);
		feed_l = m_highLeftFeedback.processOneSample(feed_l);

		m_lastDelayValueLeft = feed_l;
		
		
		
		double outdata = m_forward * outLeft + m_blend * curInLeft;
		outdata = m_lowLeftForward.processOneSample(outdata);
		outdata = m_highLeftForward.processOneSample(outdata);
		dataOutLeft[kk] = outdata;


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

			double feed_r = m_delayMemoryRight[rightReadFeedbackPos];
			feed_r = m_lowRightFeedback.processOneSample(feed_r);
			feed_r = m_highRightFeedback.processOneSample(feed_r);


            m_lastDelayValueRight = feed_r;
			double outdatar = m_forward * outRight + m_blend * curInRight;
			outdatar = m_lowRightForward.processOneSample(outdatar);
			outdatar = m_highRightForward.processOneSample(outdatar);

            dataOutRight[kk] = outdatar;
            
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

int ChorusParameter::addParameter(std::vector<std::unique_ptr<RangedAudioParameter>>& paramVector)
{
	paramVector.push_back(std::make_unique<AudioParameterFloat>(paramChorusDelay.ID,
		paramChorusDelay.name,
		NormalisableRange<float>(paramChorusDelay.minValue, paramChorusDelay.maxValue),
		paramChorusDelay.defaultValue,
		paramChorusDelay.unitName,
		AudioProcessorParameter::genericParameter,
		[](float value, int MaxLen) { return (String(int((value) * 10 + 0.5)*0.1, MaxLen) + " ms"); },
		[](const String& text) {return text.getFloatValue(); }));

	paramVector.push_back(std::make_unique<AudioParameterFloat>(paramChorusWidth.ID,
		paramChorusWidth.name,
		NormalisableRange<float>(paramChorusWidth.minValue, paramChorusWidth.maxValue),
		paramChorusWidth.defaultValue,
		paramChorusWidth.unitName,
		AudioProcessorParameter::genericParameter,
		[](float value, int MaxLen) { return (String(int((value) * 10 + 0.5)*0.1, MaxLen) + " %"); },
		[](const String& text) {return text.getFloatValue(); }));

	paramVector.push_back(std::make_unique<AudioParameterFloat>(paramChorusDirectOut.ID,
		paramChorusDirectOut.name,
		NormalisableRange<float>(paramChorusDirectOut.minValue, paramChorusDirectOut.maxValue),
		paramChorusDirectOut.defaultValue,
		paramChorusDirectOut.unitName,
		AudioProcessorParameter::genericParameter,
		[](float value, int MaxLen) { return (String(int((value) * 100 + 0.5)*0.01, MaxLen) + ""); },
		[](const String& text) {return text.getFloatValue(); }));

	paramVector.push_back(std::make_unique<AudioParameterFloat>(paramChorusForward.ID,
		paramChorusForward.name,
		NormalisableRange<float>(paramChorusForward.minValue, paramChorusForward.maxValue),
		paramChorusForward.defaultValue,
		paramChorusForward.unitName,
		AudioProcessorParameter::genericParameter,
		[](float value, int MaxLen) { return (String(int((value) * 100 + 0.5)*0.01, MaxLen) + ""); },
		[](const String& text) {return text.getFloatValue(); }));

	paramVector.push_back(std::make_unique<AudioParameterFloat>(paramChorusFFLow.ID,
		paramChorusFFLow.name,
		NormalisableRange<float>(paramChorusFFLow.minValue, paramChorusFFLow.maxValue),
		paramChorusFFLow.defaultValue,
		paramChorusFFLow.unitName,
		AudioProcessorParameter::genericParameter,
		[](float value, int MaxLen) { return (String(int(exp(value) * 10 + 0.5)*0.1, MaxLen) + " Hz"); },
		[](const String& text) {return text.getFloatValue(); }));

	paramVector.push_back(std::make_unique<AudioParameterFloat>(paramChorusFFHigh.ID,
		paramChorusFFHigh.name,
		NormalisableRange<float>(paramChorusFFHigh.minValue, paramChorusFFHigh.maxValue),
		paramChorusFFHigh.defaultValue,
		paramChorusFFHigh.unitName,
		AudioProcessorParameter::genericParameter,
		[](float value, int MaxLen) { return (String(int(exp(value) * 10 + 0.5)*0.1, MaxLen) + " Hz"); },
		[](const String& text) {return text.getFloatValue(); }));



	paramVector.push_back(std::make_unique<AudioParameterFloat>(paramChorusFeedback.ID,
		paramChorusFeedback.name,
		NormalisableRange<float>(paramChorusFeedback.minValue, paramChorusFeedback.maxValue),
		paramChorusFeedback.defaultValue,
		paramChorusFeedback.unitName,
		AudioProcessorParameter::genericParameter,
		[](float value, int MaxLen) { return (String(int((value) * 100 + 0.5)*0.01, MaxLen) + ""); },
		[](const String& text) {return text.getFloatValue(); }));

	paramVector.push_back(std::make_unique<AudioParameterFloat>(paramChorusFBLow.ID,
		paramChorusFBLow.name,
		NormalisableRange<float>(paramChorusFBLow.minValue, paramChorusFBLow.maxValue),
		paramChorusFBLow.defaultValue,
		paramChorusFBLow.unitName,
		AudioProcessorParameter::genericParameter,
		[](float value, int MaxLen) { return (String(int(exp(value) * 10 + 0.5)*0.1, MaxLen) + " Hz"); },
		[](const String& text) {return text.getFloatValue(); }));

	paramVector.push_back(std::make_unique<AudioParameterFloat>(paramChorusFBHigh.ID,
		paramChorusFBHigh.name,
		NormalisableRange<float>(paramChorusFBHigh.minValue, paramChorusFBHigh.maxValue),
		paramChorusFBHigh.defaultValue,
		paramChorusFBHigh.unitName,
		AudioProcessorParameter::genericParameter,
		[](float value, int MaxLen) { return (String(int(exp(value) * 10 + 0.5)*0.1, MaxLen) + " Hz"); },
		[](const String& text) {return text.getFloatValue(); }));

	paramVector.push_back(std::make_unique<AudioParameterFloat>(paramChorusPhase.ID,
		paramChorusPhase.name,
		NormalisableRange<float>(paramChorusPhase.minValue, paramChorusPhase.maxValue),
		paramChorusPhase.defaultValue,
		paramChorusPhase.unitName,
		AudioProcessorParameter::genericParameter,
		[](float value, int MaxLen) { return (String(int((value) * 10 + 0.5)*0.1, MaxLen) + ""); },
		[](const String& text) {return text.getFloatValue(); }));

	return 0;
}
