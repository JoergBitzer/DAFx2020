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
    m_lfoRight.setStartphase(0.0);//Einziger Unterschied zwischen Links und Rechts
	m_width = 0.0;
	m_nominalDelay = m_absolutMaxDelay_ms / 2.0;
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
	m_highLeftForward.setDesignroutine(FirstOrderFilter::FilterDesign::lowShelv);
	m_highLeftForward.setGainIndB(-6.0);
	m_highLeftForward.setCutoff(1500.0);
	
	m_lowRightForward.setSamplerate(m_fs);
	m_lowRightForward.setDesignroutine(FirstOrderFilter::FilterDesign::lowpassButter);
	m_lowRightForward.setCutoff(4000.0);



	m_highRightForward.setSamplerate(m_fs);
	m_highRightForward.setDesignroutine(FirstOrderFilter::FilterDesign::lowShelv);
	m_highRightForward.setGainIndB(-6.0);
	m_highRightForward.setCutoff(1500.0);


	m_lowLeftFeedback.setSamplerate(m_fs);
	m_lowLeftFeedback.setDesignroutine(FirstOrderFilter::FilterDesign::lowpassButter);
	m_lowLeftFeedback.setCutoff(6000.0);


	m_highLeftFeedback.setSamplerate(m_fs);
	m_highLeftFeedback.setDesignroutine(FirstOrderFilter::FilterDesign::lowShelv);
	m_highLeftFeedback.setGainIndB(-6.0);
	m_highLeftFeedback.setCutoff(1500.0);

	
	
	m_lowRightFeedback.setSamplerate(m_fs);
	m_lowRightFeedback.setDesignroutine(FirstOrderFilter::FilterDesign::lowpassButter);
	m_lowRightFeedback.setCutoff(6000.0);

	m_highRightFeedback.setSamplerate(m_fs);
	m_highRightFeedback.setDesignroutine(FirstOrderFilter::FilterDesign::lowShelv);
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
    m_nominalDelay = (m_lfoMaxDelay - m_lfoMinDelay)*0.5 + m_lfoMinDelay;
    m_nominalDelay_samples = int(m_nominalDelay * 0.001 * m_fs + 0.5);
    if (m_nominalDelay_samples <= 0)
        m_nominalDelay_samples = 1;
    m_width = (1.0 - m_lfoMinDelay / m_nominalDelay) * 100.0;
    m_lfoLeft.setMin(m_lfoMinDelay * 0.001 * m_fs);
    m_lfoRight.setMin(m_lfoMinDelay * 0.001 * m_fs);
}

void SimpleChorus::setMaxDelay()
{
    m_nominalDelay = (m_lfoMaxDelay - m_lfoMinDelay)*0.5 + m_lfoMinDelay;
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

ChorusParameterComponent::ChorusParameterComponent(AudioProcessorValueTreeState & vts)
	:m_vts(vts), somethingChanged(nullptr)
{
	somethingChanged = nullptr;
	m_delayLabel.setText("Delay", NotificationType::dontSendNotification);
	addAndMakeVisible(m_delayLabel);
	m_delaySlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
	m_delaySlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxAbove, true, 80, 20);
	m_delayAttachment = std::make_unique<SliderAttachment>(m_vts, paramChorusDelay.ID, m_delaySlider);
	addAndMakeVisible(m_delaySlider);
	m_delaySlider.onValueChange = [this]() {if (somethingChanged != nullptr)  somethingChanged(); };

	m_widthLabel.setText("Width", NotificationType::dontSendNotification);
	addAndMakeVisible(m_widthLabel);
	m_widthSlider.setSliderStyle(Slider::SliderStyle::LinearHorizontal);
	m_widthSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 80, 20);
	m_widthAttachment = std::make_unique<SliderAttachment>(m_vts, paramChorusWidth.ID, m_widthSlider);
	addAndMakeVisible(m_widthSlider);
	m_widthSlider.onValueChange = [this]() {if (somethingChanged != nullptr) somethingChanged(); };

	m_forwardLabel.setText("Forward", NotificationType::dontSendNotification);
	addAndMakeVisible(m_forwardLabel);
	m_forwardSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	m_forwardSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 80, 20);
	m_forwardAttachment = std::make_unique<SliderAttachment>(m_vts, paramChorusForward.ID, m_forwardSlider);
	addAndMakeVisible(m_forwardSlider);
	m_forwardSlider.onValueChange = [this]() {if (somethingChanged != nullptr) somethingChanged(); };

	m_ffLowLabel.setText("FW Low", NotificationType::dontSendNotification);
	addAndMakeVisible(m_ffLowLabel);
	m_ffLowSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	m_ffLowSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 80, 20);
	m_ffLowAttachment = std::make_unique<SliderAttachment>(m_vts, paramChorusFFLow.ID, m_ffLowSlider);
	addAndMakeVisible(m_ffLowSlider);
	m_ffLowSlider.onValueChange = [this]() {if (somethingChanged != nullptr) somethingChanged(); };

	m_ffHighLabel.setText("FW High", NotificationType::dontSendNotification);
	addAndMakeVisible(m_ffHighLabel);
	m_ffHighSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	m_ffHighSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 80, 20);
	m_ffHighAttachment = std::make_unique<SliderAttachment>(m_vts, paramChorusFFHigh.ID, m_ffHighSlider);
	addAndMakeVisible(m_ffHighSlider);
	m_ffHighSlider.onValueChange = [this]() {if (somethingChanged != nullptr) somethingChanged(); };

	m_feedbackLabel.setText("Feedback", NotificationType::dontSendNotification);
	addAndMakeVisible(m_feedbackLabel);
	m_feedbackSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	m_feedbackSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 80, 20);
	m_feedbackAttachment = std::make_unique<SliderAttachment>(m_vts, paramChorusFeedback.ID, m_feedbackSlider);
	addAndMakeVisible(m_feedbackSlider);
	m_feedbackSlider.onValueChange = [this]() {if (somethingChanged != nullptr) somethingChanged(); };

	m_fbLowLabel.setText("FB Low", NotificationType::dontSendNotification);
	addAndMakeVisible(m_fbLowLabel);
	m_fbLowSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	m_fbLowSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 80, 20);

	m_fbLowAttachment = std::make_unique<SliderAttachment>(m_vts, paramChorusFBLow.ID, m_fbLowSlider);
	addAndMakeVisible(m_fbLowSlider);
	m_fbLowSlider.onValueChange = [this]() {if (somethingChanged != nullptr) somethingChanged(); };

	m_fbHighLabel.setText("FB High", NotificationType::dontSendNotification);
	addAndMakeVisible(m_fbHighLabel);
	m_fbHighSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	m_fbHighSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 80, 20);
	m_fbHighAttachment = std::make_unique<SliderAttachment>(m_vts, paramChorusFBHigh.ID, m_fbHighSlider);
	addAndMakeVisible(m_fbHighSlider);
	m_fbHighSlider.onValueChange = [this]() {if (somethingChanged != nullptr) somethingChanged(); };

	m_directLabel.setText("Direct Out", NotificationType::dontSendNotification);
	addAndMakeVisible(m_directLabel);
	m_directSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	m_directSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 80, 20);

	m_directAttachment = std::make_unique<SliderAttachment>(m_vts, paramChorusDirectOut.ID, m_directSlider);
	addAndMakeVisible(m_directSlider);
	m_directSlider.onValueChange = [this]() {if (somethingChanged != nullptr) somethingChanged(); };

	m_phaseLRLabel.setText("Phase L/R", NotificationType::dontSendNotification);
	addAndMakeVisible(m_phaseLRLabel);
	m_phaseLRSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	m_phaseLRSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 80, 20);

	m_phaseLRAttachment = std::make_unique<SliderAttachment>(m_vts, paramChorusPhase.ID, m_phaseLRSlider);
	addAndMakeVisible(m_phaseLRSlider);
	m_phaseLRSlider.onValueChange = [this]() {if (somethingChanged != nullptr) somethingChanged(); };


}

void ChorusParameterComponent::paint(Graphics & g)
{
	g.fillAll((getLookAndFeel().findColour(ResizableWindow::backgroundColourId)).brighter(0.2));

}

#define GUI_MIN_DISTANCE 5
#define ELEMNT_HEIGHT 20
#define FROM_MID_DISTANCE 40
#define ROTARYSIZE 60
#define LABEL_WIDTH 60
void ChorusParameterComponent::resized()
{
	int w = getWidth();
	int h = getHeight();


	m_delayLabel.setBounds(GUI_MIN_DISTANCE + ROTARYSIZE, GUI_MIN_DISTANCE, LABEL_WIDTH, ELEMNT_HEIGHT);
	m_delaySlider.setBounds(GUI_MIN_DISTANCE + ROTARYSIZE, GUI_MIN_DISTANCE, w - 2 * GUI_MIN_DISTANCE-2*ROTARYSIZE,2*ELEMNT_HEIGHT);

	m_widthLabel.setBounds(GUI_MIN_DISTANCE + w/2 - LABEL_WIDTH/2, GUI_MIN_DISTANCE + 5 * ELEMNT_HEIGHT, LABEL_WIDTH, ELEMNT_HEIGHT);
	m_widthSlider.setBounds(GUI_MIN_DISTANCE + ROTARYSIZE, GUI_MIN_DISTANCE + 3 * ELEMNT_HEIGHT, w - 2 * GUI_MIN_DISTANCE - 2*ROTARYSIZE, 2 * ELEMNT_HEIGHT);

	m_forwardLabel.setBounds(w/2 + FROM_MID_DISTANCE + GUI_MIN_DISTANCE, GUI_MIN_DISTANCE + 4 * ELEMNT_HEIGHT, LABEL_WIDTH, ELEMNT_HEIGHT);
	m_forwardSlider.setBounds(w / 2 + FROM_MID_DISTANCE + GUI_MIN_DISTANCE + LABEL_WIDTH/2-ROTARYSIZE/2, GUI_MIN_DISTANCE + 5 * ELEMNT_HEIGHT, ROTARYSIZE, 3 * ELEMNT_HEIGHT);

	m_ffLowLabel.setBounds(w / 2 + ROTARYSIZE + FROM_MID_DISTANCE + 2*GUI_MIN_DISTANCE, GUI_MIN_DISTANCE + 4 * ELEMNT_HEIGHT, LABEL_WIDTH, ELEMNT_HEIGHT);
	m_ffLowSlider.setBounds(w / 2 + ROTARYSIZE + FROM_MID_DISTANCE + 2*GUI_MIN_DISTANCE + LABEL_WIDTH / 2 - ROTARYSIZE / 2, GUI_MIN_DISTANCE + 5 * ELEMNT_HEIGHT, ROTARYSIZE, 3 * ELEMNT_HEIGHT);

	m_ffHighLabel.setBounds(w / 2 + 2*ROTARYSIZE + FROM_MID_DISTANCE + 3 * GUI_MIN_DISTANCE, GUI_MIN_DISTANCE + 4 * ELEMNT_HEIGHT, LABEL_WIDTH, ELEMNT_HEIGHT);
	m_ffHighSlider.setBounds(w / 2 + 2*ROTARYSIZE + FROM_MID_DISTANCE + 3 * GUI_MIN_DISTANCE + LABEL_WIDTH / 2 - ROTARYSIZE / 2, GUI_MIN_DISTANCE + 5 * ELEMNT_HEIGHT, ROTARYSIZE, 3 * ELEMNT_HEIGHT);

	m_fbHighLabel.setBounds(w / 2 - ROTARYSIZE - FROM_MID_DISTANCE - GUI_MIN_DISTANCE, GUI_MIN_DISTANCE + 4 * ELEMNT_HEIGHT, LABEL_WIDTH, ELEMNT_HEIGHT);
	m_fbHighSlider.setBounds(w / 2 - ROTARYSIZE - FROM_MID_DISTANCE - GUI_MIN_DISTANCE - LABEL_WIDTH / 2 + ROTARYSIZE / 2, GUI_MIN_DISTANCE + 5 * ELEMNT_HEIGHT, ROTARYSIZE, 3 * ELEMNT_HEIGHT);

	m_fbLowLabel.setBounds(w / 2 - 2*ROTARYSIZE - FROM_MID_DISTANCE - 2 * GUI_MIN_DISTANCE, GUI_MIN_DISTANCE + 4 * ELEMNT_HEIGHT, LABEL_WIDTH, ELEMNT_HEIGHT);
	m_fbLowSlider.setBounds(w / 2 - 2*ROTARYSIZE - FROM_MID_DISTANCE - 2 * GUI_MIN_DISTANCE - LABEL_WIDTH / 2 + ROTARYSIZE / 2, GUI_MIN_DISTANCE + 5 * ELEMNT_HEIGHT, ROTARYSIZE, 3 * ELEMNT_HEIGHT);

	m_feedbackLabel.setBounds(w / 2 - 3 * ROTARYSIZE - FROM_MID_DISTANCE - 3 * GUI_MIN_DISTANCE, GUI_MIN_DISTANCE + 4 * ELEMNT_HEIGHT, LABEL_WIDTH, ELEMNT_HEIGHT);
	m_feedbackSlider.setBounds(w / 2 - 3 * ROTARYSIZE - FROM_MID_DISTANCE - 3 * GUI_MIN_DISTANCE - LABEL_WIDTH / 2 + ROTARYSIZE / 2, GUI_MIN_DISTANCE + 5 * ELEMNT_HEIGHT, ROTARYSIZE, 3 * ELEMNT_HEIGHT);

	m_directLabel.setBounds(GUI_MIN_DISTANCE, GUI_MIN_DISTANCE + 2 * ELEMNT_HEIGHT, LABEL_WIDTH, ELEMNT_HEIGHT);
	m_directSlider.setBounds(GUI_MIN_DISTANCE, GUI_MIN_DISTANCE + 3 * ELEMNT_HEIGHT, ROTARYSIZE, 3 * ELEMNT_HEIGHT);

	m_phaseLRLabel.setBounds(w-GUI_MIN_DISTANCE-LABEL_WIDTH, GUI_MIN_DISTANCE + 2 * ELEMNT_HEIGHT, LABEL_WIDTH, ELEMNT_HEIGHT);
	m_phaseLRSlider.setBounds(w-GUI_MIN_DISTANCE-ROTARYSIZE, GUI_MIN_DISTANCE + 3 * ELEMNT_HEIGHT, ROTARYSIZE, 3 * ELEMNT_HEIGHT);

}
