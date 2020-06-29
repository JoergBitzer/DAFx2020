#include "MoogLadder.h"

MoogLadder::MoogLadder()
	:m_fs(44100.f),
	m_cutoff(1000.f),
	m_resonance(0.f),
	m_modKeyboard(1.0),
	m_modKeyboardFactor(1.0),
	m_currNote(1),
	m_modDepth(0.0),
	m_modUpdateRate_ms(1.0),
	m_modVal(0.0)
{
	reset();
	changeParameter();
	setUpdate();
}

MoogLadder::~MoogLadder()
{
}

int MoogLadder::processData(std::vector<double>& in, std::vector<double>& out)
{
	for (auto kk = 0U; kk < in.size(); ++kk)
	{
		// Modulation update
		if (m_modUpdateCounter == m_modUpdateRate_samples)
		{
			m_modUpdateCounter = 0;
			m_modVal = m_modData[kk];
			//double NewCutoff = checkRange(m_cutoff + m_fs/2*0.9*m_modDepth*m_modVal );
			//setCutoffFrequency(NewCutoff);
			changeParameter();
		}
		++m_modUpdateCounter;
		
		// processing
		double curIn = in[kk];
		double u = m_alpha_0 * (curIn - m_resonance * (m_state1 * m_beta1 +
			m_state2 * m_beta2 + m_state3 * m_beta3 + m_state4 * m_beta4));

		double v1 = m_alpha * (u - m_state1);
		double y1 = v1 + m_state1;

		double v2 = m_alpha * (y1 - m_state2);
		double y2 = v2 + m_state2;

		double v3 = m_alpha * (y2 - m_state3);
		double y3 = v3 + m_state3;

		double v4 = m_alpha * (y3 - m_state4);
		double y4 = v4 + m_state4;

		out[kk] = y4;

		m_state1 = v1 + y1;
		m_state2 = v2 + y2;
		m_state3 = v3 + y3;
		m_state4 = v4 + y4;
	}
	return 0;
}

void MoogLadder::setMidiNote(int midinote)
{
	m_currNote = midinote;
	const int midia0 = 57;
	int distanceInSemitones = midinote - midia0;

	m_modKeyboardFactor = pow(2.0, m_modKeyboard*double(distanceInSemitones) / 12.0);
	changeParameter();
}

void MoogLadder::reset()
{
	m_state1 = 0.0;
	m_state2 = 0.0;
	m_state3 = 0.0;
	m_state4 = 0.0;
}

void MoogLadder::changeParameter()

{
	double cutoff = checkRange(m_modKeyboardFactor * m_cutoff + m_fs / 2 * 0.9 * m_modDepth * m_modVal);
	double g = tan(M_PI * cutoff / m_fs);
	m_alpha = g / (1.0 + g);
	m_alpha_0 = 1.0 / (1.0 + m_resonance * m_alpha * m_alpha * m_alpha * m_alpha);
	m_beta4 = 1.0 / (1.0 + g);
	m_beta3 = m_alpha / (1.0 + g);
	m_beta2 = m_alpha * m_alpha / (1.0 + g);
	m_beta1 = m_alpha * m_alpha * m_alpha / (1.0 + g);
}
#ifdef USE_JUCE

int MoogLadderParameter::addParameter(std::vector<std::unique_ptr<RangedAudioParameter>>& paramVector)
{
	paramVector.push_back(std::make_unique<AudioParameterFloat>(paramCutoff.ID,
		paramCutoff.name,
		NormalisableRange<float>(paramCutoff.minValue, paramCutoff.maxValue),
		paramCutoff.defaultValue,
		paramCutoff.unitName,
		AudioProcessorParameter::genericParameter,
		[](float value, int MaxLen) { value = int(exp(value) * 10)*0.1;  return String(value, MaxLen); },
		[](const String& text) {return text.getFloatValue(); }));
	
	paramVector.push_back(std::make_unique<AudioParameterFloat>(paramResonance.ID,
		paramResonance.name,
		NormalisableRange<float>(paramResonance.minValue, paramResonance.maxValue),
		paramResonance.defaultValue,
		paramResonance.unitName,
		AudioProcessorParameter::genericParameter,
		[](float value, int MaxLen) {return String(value, MaxLen); },
		[](const String& text)	{ return text.getFloatValue();}));

	paramVector.push_back(std::make_unique<AudioParameterFloat>(paramModKeyboard.ID,
		paramModKeyboard.name,
		NormalisableRange<float>(paramModKeyboard.minValue, paramModKeyboard.maxValue),
		paramModKeyboard.defaultValue,
		paramModKeyboard.unitName,
		AudioProcessorParameter::genericParameter,
		[](float value, int MaxLen) {return String(value, MaxLen); },
		[](const String& text) { return text.getFloatValue(); }));

	paramVector.push_back(std::make_unique<AudioParameterFloat>(paramModDepth.ID,
		paramModDepth.name,
		NormalisableRange<float>(paramModDepth.minValue, paramModDepth.maxValue),
		paramModDepth.defaultValue,
		paramModDepth.unitName,
		AudioProcessorParameter::genericParameter,
		[](float value, int MaxLen) {return String(value, MaxLen); },
		[](const String& text) { return text.getFloatValue(); }));
	
	return 0;
}

MoogLadderParameterComponent::MoogLadderParameterComponent(AudioProcessorValueTreeState& vts)
	:m_vts(vts), somethingChanged(nullptr), m_style(ComponentStyle::horizontal)
{
	m_CutoffLabel.setText("Cutoff", NotificationType::dontSendNotification);
	m_CutoffLabel.setJustificationType(Justification::centred);
	addAndMakeVisible(m_CutoffLabel);

	m_CutoffSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	m_CutoffSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 60, 20);
	m_CutoffAttachment = std::make_unique<SliderAttachment>(m_vts, paramCutoff.ID, m_CutoffSlider);
	addAndMakeVisible(m_CutoffSlider);
	m_CutoffSlider.onValueChange = [this]() {if (somethingChanged != nullptr) somethingChanged(); };


	m_ResonanceLabel.setText("Resonance", NotificationType::dontSendNotification);
	m_ResonanceLabel.setJustificationType(Justification::centred);
	addAndMakeVisible(m_ResonanceLabel);

	m_ResonanceSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	m_ResonanceSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 60, 20);
	m_ResonanceAttachment = std::make_unique<SliderAttachment>(m_vts, paramResonance.ID, m_ResonanceSlider);
	addAndMakeVisible(m_ResonanceSlider);
	m_ResonanceSlider.onValueChange = [this]() {if (somethingChanged != nullptr) somethingChanged(); };

	m_ModKeyboardLabel.setText("ModKeyboard", NotificationType::dontSendNotification);
	m_ModKeyboardLabel.setJustificationType(Justification::centred);
	addAndMakeVisible(m_ModKeyboardLabel);

	m_ModKeyboardSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	m_ModKeyboardSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 60, 20);
	m_ModKeyboardAttachment = std::make_unique<SliderAttachment>(m_vts, paramModKeyboard.ID, m_ModKeyboardSlider);
	addAndMakeVisible(m_ModKeyboardSlider);
	m_ModKeyboardSlider.onValueChange = [this]() {if (somethingChanged != nullptr) somethingChanged(); };


	m_ModDepthLabel.setText("Depth", NotificationType::dontSendNotification);
	m_ModDepthLabel.setJustificationType(Justification::centred);
	addAndMakeVisible(m_ModDepthLabel);

	m_ModDepthSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	m_ModDepthSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 60, 20);
	m_ModDepthAttachment = std::make_unique<SliderAttachment>(m_vts, paramModDepth.ID, m_ModDepthSlider);
	addAndMakeVisible(m_ModDepthSlider);
	m_ModDepthSlider.onValueChange = [this]() {if (somethingChanged != nullptr) somethingChanged(); };

}

void MoogLadderParameterComponent::paint(Graphics& g)
{
	g.fillAll((getLookAndFeel().findColour(ResizableWindow::backgroundColourId)).darker(0.2));

}
#define MOOG_LABEL_WIDTH 60
#define MOOG_ROTARY_WIDTH 60
#define MOOG_MIN_DISTANCE 5
#define MOOG_LABEL_HEIGHT 20

void MoogLadderParameterComponent::resized()
{
	auto r = getLocalBounds();
	r.reduce(MOOG_MIN_DISTANCE, MOOG_MIN_DISTANCE);
	auto s = r;
	auto t = r;
	switch (m_style)
	{
	case ComponentStyle::compact:

		break;
	case ComponentStyle::horizontal:
		s = r.removeFromTop(MOOG_LABEL_HEIGHT);
		m_CutoffLabel.setBounds(s.removeFromLeft(MOOG_LABEL_WIDTH));
		s.removeFromLeft(MOOG_MIN_DISTANCE);
		m_ResonanceLabel.setBounds(s.removeFromLeft(MOOG_LABEL_WIDTH));
		s.removeFromLeft(MOOG_MIN_DISTANCE);
		m_ModKeyboardLabel.setBounds(s.removeFromLeft(MOOG_LABEL_WIDTH));
		s.removeFromLeft(MOOG_MIN_DISTANCE);
		m_ModDepthLabel.setBounds(s.removeFromLeft(MOOG_LABEL_WIDTH));

		s = r;
		t = s.removeFromBottom(MOOG_ROTARY_WIDTH);
		m_CutoffSlider.setBounds(t.removeFromLeft(MOOG_ROTARY_WIDTH));
		t.removeFromLeft(MOOG_MIN_DISTANCE);
		m_ResonanceSlider.setBounds(t.removeFromLeft(MOOG_ROTARY_WIDTH));
		t.removeFromLeft(MOOG_MIN_DISTANCE);
		m_ModKeyboardSlider.setBounds(t.removeFromLeft(MOOG_ROTARY_WIDTH));
		t.removeFromLeft(MOOG_MIN_DISTANCE);
		m_ModDepthSlider.setBounds(t.removeFromLeft(MOOG_ROTARY_WIDTH));

		break;
	case ComponentStyle::vertical:

		break;
	}

}
#endif
