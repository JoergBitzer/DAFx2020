#include "LFO.h"

LFO::LFO()
	:m_freq(1.0),
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
	m_lfoBasis = &m_lfotri;
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
	m_freq = frequency;
	m_lfoBasis->setFrequency(m_freq);
}

void LFO::setSamplerate(double samplerate)
{
	m_fs = samplerate;
	m_lfoBasis->setSamplingrate(m_fs);
}

void LFO::setStartphase(double startPhase)
{
	m_startphase = startPhase;
	m_lfoBasis->setStartPhase(m_startphase);
}

void LFO::setPhase(double phase)
{
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
	}
	return 0;
}

LFOParameterComponent::LFOParameterComponent(AudioProcessorValueTreeState &vts, int index, const String& lfoName)
:m_vts(vts), m_index(index), m_name(lfoName), m_style(LFOComponentStyle::longhorizontal),somethingChanged(nullptr)
{
	// Rate
	m_lforateLabel.setText("Rate",NotificationType::dontSendNotification);
	addAndMakeVisible(m_lforateLabel);
	m_lforateSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	m_lforateSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxRight, true, 80, 20);
	m_lforateAttachment = std::make_unique<SliderAttachment>(m_vts, paramLFORate.ID[m_index], m_lforateSlider);
	addAndMakeVisible(m_lforateSlider);
	m_lforateSlider.onValueChange = [this]() {if (somethingChanged != nullptr) somethingChanged(); };

	// Waveform
	m_lfowaveformLabel.setText("Waveform", NotificationType::dontSendNotification);
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
	g.fillAll((getLookAndFeel().findColour(ResizableWindow::backgroundColourId)).brighter(0.2));

	g.setColour(Colours::red);
	auto r = getBounds();
	g.drawRect(r, 4.0);
	
//	g.setFont(15.0f);
//	g.drawFittedText(m_name, getLocalBounds().reduced(4.0), Justification::centredTop,1);
}

#define GUI_MIN_DISTANCE 5
#define ELEMNT_HEIGHT 20
#define ROTARYSIZE 40
#define LABEL_WIDTH 60
#define COMBO_WIDTH 80
void LFOParameterComponent::resized()
{
	if (m_style == LFOComponentStyle::compact)
	{
		auto r = getBounds();
		//r.removeFromTop(20);
		m_lforateLabel.setBounds(GUI_MIN_DISTANCE, GUI_MIN_DISTANCE + 10, r.getWidth() / 4, r.getHeight() / 2 - GUI_MIN_DISTANCE);
		m_lforateSlider.setBounds(r.getWidth() / 4 + GUI_MIN_DISTANCE, GUI_MIN_DISTANCE + 10, 3 * r.getWidth() / 4 - 2 * GUI_MIN_DISTANCE, r.getHeight() / 2 - GUI_MIN_DISTANCE);
		m_lfowaveformLabel.setBounds(GUI_MIN_DISTANCE, r.getHeight() / 2 + GUI_MIN_DISTANCE, 2 * r.getWidth() / 5, r.getHeight() / 2 - 2 * GUI_MIN_DISTANCE);
		m_lfowaveformCombo.setBounds(2 * r.getWidth() / 5 + GUI_MIN_DISTANCE, r.getHeight() / 2 + GUI_MIN_DISTANCE, 3 * r.getWidth() / 5 - 2 * GUI_MIN_DISTANCE, r.getHeight() / 2 - 2 * GUI_MIN_DISTANCE);
	}
	if (m_style == LFOComponentStyle::longhorizontal)
	{
		auto r = getLocalBounds();
		r.removeFromLeft(GUI_MIN_DISTANCE);
		//m_lforateLabel.setVisible(false);
		m_lforateLabel.setBounds(r.removeFromLeft(LABEL_WIDTH));
		r.removeFromLeft(GUI_MIN_DISTANCE);
		m_lforateSlider.setBounds(r.removeFromLeft(3*ROTARYSIZE));
		r.removeFromLeft(2*GUI_MIN_DISTANCE);
		m_lfowaveformLabel.setBounds(r.removeFromLeft(LABEL_WIDTH));
		r.removeFromLeft(GUI_MIN_DISTANCE);
		m_lfowaveformCombo.setBounds(r.removeFromLeft(COMBO_WIDTH));

	}
}

void LFOParameterComponent::setStyle(LFOComponentStyle style)
{
	m_style = style;
	resized();
}
