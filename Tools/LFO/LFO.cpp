#include "LFO.h"

LFO::LFO()
	:m_freq(0.25),
	m_fs(1.0),
	m_max(1.0),
	m_min(1.0),
	m_mean(0.0),
	m_amplitude(1.0),
	m_maxmin(2.0),
	m_startphase(0.0),
	m_slopeTime(0.0),
	m_phase(0.0)
{
	m_lfoBasis = &m_lfosin;
}

LFO::~LFO()
{
}

int LFO::getData(std::vector<double>& data)
{
	m_lfoBasis->getData(data);

	for (unsigned int kk = 0; kk < data.size(); kk++)
	{
		data[kk] *= m_maxmin;
		data[kk] += m_min;
	}
	return 0;
}

void LFO::setMin(double newMin)
{
	m_min = newMin;
	m_maxmin = m_max - m_min;
	m_amplitude = m_maxmin * 0.5;
	m_mean = (m_max + m_min) * 0.5;
}

void LFO::setMax(double newMax)
{
	m_max = newMax;
	m_maxmin = m_max - m_min;
	m_amplitude = m_maxmin * 0.5;
	m_mean = (m_max + m_min) * 0.5;
}

void LFO::setMean(double newMean)
{
	m_mean = newMean;
	m_max = m_mean + m_amplitude;
	m_min = m_mean - m_amplitude;
	m_maxmin = m_max - m_min;
}

void LFO::setAmplitude(double newAmplitude)
{
	m_amplitude = newAmplitude;
	m_max = m_mean + m_amplitude;
	m_min = m_mean - m_amplitude;
	m_maxmin = m_max - m_min;
}

void LFO::setFrequency(double frequency)
{
	if (frequency < 0.0)
		frequency = 0.0;

	if (frequency >= m_fs / 4)
		frequency = m_fs / 4;
	
	m_freq = frequency;
	m_lfoBasis->setFrequency(m_freq);
}

void LFO::setSamplerate(double samplerate)
{
	if (samplerate < 0.0)
		samplerate = 1.0;
	m_fs = samplerate;
	m_lfoBasis->setSamplingrate(m_fs);
}

void LFO::setStartphase(double startPhase)
{
	if (startPhase < -3.1415)
		startPhase = -3.1415;
	if (startPhase > 2.0*3.1415)
		startPhase = 0.0;
	m_startphase = startPhase;
	m_lfoBasis->setStartPhase(m_startphase);
}

void LFO::setPhase(double phase)
{
	if (phase < -3.1415)
		phase = -3.1415;
	if (phase > 2.0*3.1415)
		phase = 0.0;
	m_phase = phase;
	m_lfoBasis->setPhase(m_phase);
}

void LFO::setSlopeTime(double slopetime)
{
	m_slopeTime = slopetime;
	m_lfoBasis->setSlopeTime(m_slopeTime);
}

void LFO::setLFOFunction(LFOFunctions newFunc)
{
	switch (newFunc)
	{
	case LFOFunctions::sinus:
		m_lfoBasis = &m_lfosin;
		break;
	case LFOFunctions::rect:
		m_lfoBasis = &m_lforect;
		break;
	case LFOFunctions::saw:
		m_lfoBasis = &m_lfosaw;
		break;
	case LFOFunctions::tri:
		m_lfoBasis = &m_lfotri;
		break;
	case LFOFunctions::sampleAndHold:
		m_lfoBasis = &m_sampleandhold;
		break;
	}
	reset();
}

void LFO::reset()
{
	m_lfoBasis->setFrequency(m_freq);
	m_lfoBasis->setSamplingrate(m_fs);
	m_lfoBasis->setStartPhase(m_startphase);
	m_lfoBasis->setSlopeTime(m_slopeTime);
	m_lfoBasis->reset();
	m_lfoBasis->setPhase(m_phase);

}

int LFOParameter::addParameter(std::vector<std::unique_ptr<RangedAudioParameter>>& paramVector, int instance)
{

	if (instance < MAX_LFO_INSTANCES)
	{
		StringArray a(paramLFOWaveform.Choices);

		paramVector.push_back(std::make_unique<AudioParameterChoice>(paramLFOWaveform.ID[instance],
			paramLFOWaveform.name,
			a,
			paramLFOWaveform.defaultIndex,
			paramLFOWaveform.unitName));

		paramVector.push_back(std::make_unique<AudioParameterFloat>(paramLFORate.ID[instance],
			paramLFORate.name,
			NormalisableRange<float>(paramLFORate.minValue, paramLFORate.maxValue),
			paramLFORate.defaultValue,
			paramLFORate.unitName,
			AudioProcessorParameter::genericParameter,
			[](float value, int MaxLen) { return (String(int(exp(value)*10+0.5)*0.1, MaxLen) + " Hz"); },
			[](const String& text) {return text.getFloatValue(); }));

		paramVector.push_back(std::make_unique<AudioParameterFloat>(paramLFOLevel.ID[instance],
			paramLFOLevel.name,
			NormalisableRange<float>(paramLFOLevel.minValue, paramLFOLevel.maxValue),
			paramLFOLevel.defaultValue,
			paramLFOLevel.unitName,
			AudioProcessorParameter::genericParameter,
			[](float value, int MaxLen) { return (String(int((value) * 10 + 0.5) * 0.1, MaxLen) + " dB"); },
			[](const String& text) {return text.getFloatValue(); }));

	}
	return 0;
}

#define JADE_MINI_SYNTH 1

LFOParameterComponent::LFOParameterComponent(AudioProcessorValueTreeState &vts, int index, const String& lfoName)
:m_vts(vts), m_index(index), m_name(lfoName), m_style(LFOComponentStyle::longhorizontal),somethingChanged(nullptr)
{
	// Rate
	m_lforateLabel.setText("Rate",NotificationType::dontSendNotification);
	m_lforateLabel.setJustificationType(Justification::centred);

	addAndMakeVisible(m_lforateLabel);
	m_lforateSlider.setSliderStyle(Slider::SliderStyle::Rotary);
#ifndef JADE_MINI_SYNTH
	m_lforateSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxRight, true, 80, 20);
#else	
	m_lforateSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 60, 20);
#endif
	
	m_lforateAttachment = std::make_unique<SliderAttachment>(m_vts, paramLFORate.ID[m_index], m_lforateSlider);
	addAndMakeVisible(m_lforateSlider);
	m_lforateSlider.onValueChange = [this]() {if (somethingChanged != nullptr) somethingChanged(); };

	// Level
	m_lfolevelLabel.setText("Level", NotificationType::dontSendNotification);
	m_lfolevelLabel.setJustificationType(Justification::centred);
	addAndMakeVisible(m_lfolevelLabel);
	m_lfolevelSlider.setSliderStyle(Slider::SliderStyle::Rotary);
#ifndef JADE_MINI_SYNTH
	m_lfolevelSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxRight, true, 80, 20);
#else
	m_lfolevelSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 60, 20);
#endif

	m_lfolevelAttachment = std::make_unique<SliderAttachment>(m_vts, paramLFOLevel.ID[m_index], m_lfolevelSlider);
	addAndMakeVisible(m_lfolevelSlider);
	m_lfolevelSlider.onValueChange = [this]() {if (somethingChanged != nullptr) somethingChanged(); };


	// Waveform
	m_lfowaveformLabel.setText("Waveform", NotificationType::dontSendNotification);
	m_lfowaveformLabel.setJustificationType(Justification::centred);

	addAndMakeVisible(m_lfowaveformLabel);
	
	StringArray WaveformNames(paramLFOWaveform.Choices);
	m_lfowaveformCombo.addItemList(WaveformNames, 1);


	//m_lfowaveformCombo.onChange = [this]() {itemchanged(); };
	m_lfowaveformCombo.setSelectedItemIndex(0, false);
	m_lfowaveformCombo.setJustificationType(Justification::centredRight);
	m_lfowaveformAttachment = std::make_unique<ComboAttachment>(m_vts, paramLFOWaveform.ID[m_index], m_lfowaveformCombo);
	//m_lfowaveformCombo.isTextEditable();
	//m_lfowaveformCombo.setEditableText(true);


	addAndMakeVisible(m_lfowaveformCombo);
	m_lfowaveformCombo.onChange = [this]() {if (somethingChanged != nullptr) somethingChanged(); };

}

void LFOParameterComponent::paint(Graphics & g)
{
#ifndef JADE_MINI_SYNTH
	g.fillAll((getLookAndFeel().findColour(ResizableWindow::backgroundColourId)).brighter(0.2));
	g.setColour(Colours::red);
	auto r = getBounds();
	g.drawRect(r, 4.0);
#else
	g.fillAll((getLookAndFeel().findColour(ResizableWindow::backgroundColourId)).darker(0.2));
#endif

//	g.setFont(15.0f);
//	g.drawFittedText(m_name, getLocalBounds().reduced(4.0), Justification::centredTop,1);
}

#define LFO_MIN_DISTANCE 5
#define ELEMNT_HEIGHT 20
#define LFO_ROTARYSIZE 40
#define LFO_ROTARY_WIDTH 60

#define LFO_LABEL_WIDTH 60
#define LFO_LABEL_HEIGHT 20

#define LFO_COMBO_WIDTH 80
void LFOParameterComponent::resized()
{
	auto r = getLocalBounds();
	r.reduce(LFO_MIN_DISTANCE, LFO_MIN_DISTANCE);
	auto s = r;
	auto t = r;

	if (m_style == LFOComponentStyle::compact)
	{
		auto r = getBounds();
		//r.removeFromTop(20);
		m_lforateLabel.setBounds(LFO_MIN_DISTANCE, LFO_MIN_DISTANCE + 10, r.getWidth() / 4, r.getHeight() / 2 - LFO_MIN_DISTANCE);
		m_lforateSlider.setBounds(r.getWidth() / 4 + LFO_MIN_DISTANCE, LFO_MIN_DISTANCE + 10, 3 * r.getWidth() / 4 - 2 * LFO_MIN_DISTANCE, r.getHeight() / 2 - LFO_MIN_DISTANCE);
		m_lfowaveformLabel.setBounds(LFO_MIN_DISTANCE, r.getHeight() / 2 + LFO_MIN_DISTANCE, 2 * r.getWidth() / 5, r.getHeight() / 2 - 2 * LFO_MIN_DISTANCE);
		m_lfowaveformCombo.setBounds(2 * r.getWidth() / 5 + LFO_MIN_DISTANCE, r.getHeight() / 2 + LFO_MIN_DISTANCE, 3 * r.getWidth() / 5 - 2 * LFO_MIN_DISTANCE, r.getHeight() / 2 - 2 * LFO_MIN_DISTANCE);
	}
	if (m_style == LFOComponentStyle::longhorizontal)
	{
		auto r = getLocalBounds();
		r.removeFromLeft(LFO_MIN_DISTANCE);
		//m_lforateLabel.setVisible(false);
		/*m_lforateLabel.setBounds(r.removeFromLeft(LFO_LABEL_WIDTH));
		r.removeFromLeft(LFO_MIN_DISTANCE);
		m_lforateSlider.setBounds(r.removeFromLeft(3*LFO_ROTARYSIZE));
		r.removeFromLeft(2*LFO_MIN_DISTANCE);
		m_lfowaveformLabel.setBounds(r.removeFromLeft(LABEL_WIDTH));
		r.removeFromLeft(LFO_MIN_DISTANCE);
		m_lfowaveformCombo.setBounds(r.removeFromLeft(LFO_COMBO_WIDTH));
		//*/
		s = r.removeFromTop(LFO_LABEL_HEIGHT);
		m_lforateLabel.setBounds(s.removeFromLeft(LFO_LABEL_WIDTH));
		s.removeFromLeft(LFO_MIN_DISTANCE);
		m_lfowaveformLabel.setBounds(s.removeFromLeft(LFO_COMBO_WIDTH));
		s.removeFromLeft(LFO_MIN_DISTANCE);
		m_lfolevelLabel.setBounds(s.removeFromLeft(LFO_LABEL_WIDTH));

		s = r;
		t = s.removeFromBottom(LFO_ROTARY_WIDTH);
		m_lforateSlider.setBounds(t.removeFromLeft(LFO_ROTARY_WIDTH));
		t.removeFromLeft(LFO_MIN_DISTANCE);
		m_lfowaveformCombo.setBounds(t.removeFromLeft(LFO_COMBO_WIDTH));
		t.removeFromLeft(LFO_MIN_DISTANCE);
		m_lfolevelSlider.setBounds(t.removeFromLeft(LFO_ROTARY_WIDTH));
	}
}

void LFOParameterComponent::setStyle(LFOComponentStyle style)
{
	m_style = style;
	resized();
}
