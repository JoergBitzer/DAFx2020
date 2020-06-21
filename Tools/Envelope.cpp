#include "Envelope.h"

Envelope::Envelope()
{
	m_fs = 1.0;

	m_delTime_ms = 0.0;

	m_alphaAtt = 0.0;
	m_attOver = 1.0 / 0.77; // from Will Pirkle Book

	m_holdTime_ms = 0.0;

	m_alphaDec = 0.0;

	m_envGain = 0.0;
	m_envelopePhase = envelopePhases::Off;

	m_sampleCounter = 0.0;
}

Envelope::~Envelope()
{
}

int Envelope::getData(std::vector<double>& data)
{
	for (unsigned int kk = 0; kk < data.size(); kk++)
	{
		double Gain;
		double Samples;
		switch (m_envelopePhase)
		{
		case envelopePhases::Off:
			m_envGain = 0.0;
			break;

		case envelopePhases::Delay:
			m_sampleCounter -= 1;
			m_envGain = 0.0;
			if (m_sampleCounter <= 0)
				m_envelopePhase = envelopePhases::Attack;
			else
				break;

		case envelopePhases::Attack:
			Gain = m_alphaAtt * m_envGain + (1 - m_alphaAtt) * m_attOver;
			m_envGain = Gain;
			if (m_envGain >= 1.0)
			{
				m_envelopePhase = envelopePhases::Hold;
				m_envGain = 1.0;
				m_sampleCounter = m_holdSamples;
			}
			break;

		case envelopePhases::Hold:
			m_sampleCounter -= 1;
			m_envGain = 1.0;
			if (m_sampleCounter <= 0)
				m_envelopePhase = envelopePhases::Decay;
			else
				break;

		case envelopePhases::Decay:
			Gain = m_alphaDec * m_envGain + (1 - m_alphaDec) * m_sustainLevel;
			m_envGain = Gain;
			break;
		case envelopePhases::Release:
			Gain = m_alphaRel * m_envGain;
			m_envGain = Gain;
			if (m_envGain <= 1.0e-5)
				m_envelopePhase = envelopePhases::Off;
			break;

		}
		data.at(kk) = m_envGain;
	}
	return 0;
}

void Envelope::NoteOn()
{
	m_envelopePhase = envelopePhases::Delay;
	m_sampleCounter = m_delSamples;
}

void Envelope::NoteOff()
{
	m_envelopePhase = envelopePhases::Release;
	if (m_envGain < 1.0e-5)
		m_envelopePhase = envelopePhases::Off;
}

void Envelope::setSamplerate(double samplerate)
{
	m_fs = samplerate;
	updateTimeConstants();
}

void Envelope::setDelayTime(double del_ms)
{
	m_delTime_ms = del_ms;
	updateTimeConstants();
}

void Envelope::setAttackRate(double att_ms)
{
	m_tauAtt_ms = att_ms;
	updateTimeConstants();
}

void Envelope::setHoldTime(double hold_ms)
{
	m_holdTime_ms = hold_ms;
	updateTimeConstants();
}

void Envelope::setDecayRate(double dec_ms)
{
	m_tauDec_ms = dec_ms;
	updateTimeConstants();
}

void Envelope::setSustainLevel(double level)
{
	m_sustainLevel = level;
}

void Envelope::setReleaseRate(double rel_ms)
{
	m_tauRel_ms = rel_ms;
	updateTimeConstants();
}

void Envelope::updateTimeConstants(void)
{
	m_alphaAtt = exp(-1.0 / (m_tauAtt_ms * 0.001 * m_fs));
	m_alphaDec = exp(-1.0 / (m_tauDec_ms * 0.001 * m_fs));
	m_alphaRel = exp(-1.0 / (m_tauRel_ms * 0.001 * m_fs));
	m_holdSamples = floor(m_holdTime_ms * m_fs * 0.001 + 0.5);
	m_delSamples = floor(m_delTime_ms * m_fs * 0.001 + 0.5);

}

int EnvelopeParameter::addParameter(std::vector < std::unique_ptr<RangedAudioParameter>>& paramVector, int instance)
{
	if (instance < MAX_ENV_INSTANCES)
	{
		paramVector.push_back(std::make_unique<AudioParameterFloat>(paramEnvAttack.ID[instance],
			paramEnvAttack.name,
			NormalisableRange<float>(paramEnvAttack.minValue, paramEnvAttack.maxValue),
			paramEnvAttack.defaultValue,
			paramEnvAttack.unitName,
			AudioProcessorParameter::genericParameter,
			[](float value, int MaxLen) { return (String(int(exp(value) * 10 + 0.5) * 0.1, MaxLen) + " ms"); },
			[](const String& text) {return text.getFloatValue(); }));

		paramVector.push_back(std::make_unique<AudioParameterFloat>(paramEnvDecay.ID[instance],
			paramEnvDecay.name,
			NormalisableRange<float>(paramEnvDecay.minValue, paramEnvDecay.maxValue),
			paramEnvDecay.defaultValue,
			paramEnvDecay.unitName,
			AudioProcessorParameter::genericParameter,
			[](float value, int MaxLen) { return (String(int(exp(value) * 10 + 0.5) * 0.1, MaxLen) + " ms"); },
			[](const String& text) {return text.getFloatValue(); }));
		
		paramVector.push_back(std::make_unique<AudioParameterFloat>(paramEnvRelease.ID[instance],
			paramEnvRelease.name,
			NormalisableRange<float>(paramEnvRelease.minValue, paramEnvRelease.maxValue),
			paramEnvRelease.defaultValue,
			paramEnvRelease.unitName,
			AudioProcessorParameter::genericParameter,
			[](float value, int MaxLen) { return (String(int(exp(value) * 10 + 0.5) * 0.1, MaxLen) + " ms"); },
			[](const String& text) {return text.getFloatValue(); }));

		paramVector.push_back(std::make_unique<AudioParameterFloat>(paramEnvDelay.ID[instance],
			paramEnvDelay.name,
			NormalisableRange<float>(paramEnvDelay.minValue, paramEnvDelay.maxValue),
			paramEnvDelay.defaultValue,
			paramEnvDelay.unitName,
			AudioProcessorParameter::genericParameter,
			[](float value, int MaxLen) { return (String(0.1*int((value) * 10 + 0.5), MaxLen) + " ms"); },
			[](const String& text) {return text.getFloatValue(); }));

		paramVector.push_back(std::make_unique<AudioParameterFloat>(paramEnvHold.ID[instance],
			paramEnvHold.name,
			NormalisableRange<float>(paramEnvHold.minValue, paramEnvHold.maxValue),
			paramEnvHold.defaultValue,
			paramEnvHold.unitName,
			AudioProcessorParameter::genericParameter,
			[](float value, int MaxLen) { return (String(0.1 * int((value) * 10 + 0.5), MaxLen) + " ms"); },
			[](const String& text) {return text.getFloatValue(); }));

		paramVector.push_back(std::make_unique<AudioParameterFloat>(paramEnvSustain.ID[instance],
			paramEnvSustain.name,
			NormalisableRange<float>(paramEnvSustain.minValue, paramEnvSustain.maxValue),
			paramEnvSustain.defaultValue,
			paramEnvSustain.unitName,
			AudioProcessorParameter::genericParameter,
			[](float value, int MaxLen) { return (String(0.01 * int((value) * 100.0 + 0.5), MaxLen) + ""); },
			[](const String& text) {return text.getFloatValue(); }));

	}
	return 0;
}

EnvelopeParameterComponent::EnvelopeParameterComponent(AudioProcessorValueTreeState& vts, int index, const String& envName)
	:m_vts(vts), somethingChanged(nullptr), m_name(envName), m_index(index),m_style(EnvelopeStyle::horizontal)
{

	m_EnvDelayLabel.setText("Delay", NotificationType::dontSendNotification);
	m_EnvDelayLabel.setJustificationType(Justification::centred);
	addAndMakeVisible(m_EnvDelayLabel);

	m_EnvDelaySlider.setSliderStyle(Slider::SliderStyle::Rotary);
	m_EnvDelaySlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 60, 20);
	m_EnvDelayAttachment = std::make_unique<SliderAttachment>(m_vts, paramEnvDelay.ID[m_index], m_EnvDelaySlider);
	addAndMakeVisible(m_EnvDelaySlider);
	m_EnvDelaySlider.onValueChange = [this]() {if (somethingChanged != nullptr) somethingChanged(); };

	m_EnvAttackLabel.setText("Attack", NotificationType::dontSendNotification);
	m_EnvAttackLabel.setJustificationType(Justification::centred);
	addAndMakeVisible(m_EnvAttackLabel);

	m_EnvAttackSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	m_EnvAttackSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 60, 20);
	m_EnvAttackAttachment = std::make_unique<SliderAttachment>(m_vts, paramEnvAttack.ID[m_index], m_EnvAttackSlider);
	addAndMakeVisible(m_EnvAttackSlider);
	m_EnvAttackSlider.onValueChange = [this]() {if (somethingChanged != nullptr) somethingChanged(); };

	m_EnvHoldLabel.setText("Hold", NotificationType::dontSendNotification);
	m_EnvHoldLabel.setJustificationType(Justification::centred);
	addAndMakeVisible(m_EnvHoldLabel);

	m_EnvHoldSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	m_EnvHoldSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 60, 20);
	m_EnvHoldAttachment = std::make_unique<SliderAttachment>(m_vts, paramEnvHold.ID[m_index], m_EnvHoldSlider);
	addAndMakeVisible(m_EnvHoldSlider);
	m_EnvHoldSlider.onValueChange = [this]() {if (somethingChanged != nullptr) somethingChanged(); };

	m_EnvDecayLabel.setText("Decay", NotificationType::dontSendNotification);
	m_EnvDecayLabel.setJustificationType(Justification::centred);
	addAndMakeVisible(m_EnvDecayLabel);

	m_EnvDecaySlider.setSliderStyle(Slider::SliderStyle::Rotary);
	m_EnvDecaySlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 60, 20);
	m_EnvDecayAttachment = std::make_unique<SliderAttachment>(m_vts, paramEnvDecay.ID[m_index], m_EnvDecaySlider);
	addAndMakeVisible(m_EnvDecaySlider);
	m_EnvDecaySlider.onValueChange = [this]() {if (somethingChanged != nullptr) somethingChanged(); };

	m_EnvSustainLabel.setText("Sustain", NotificationType::dontSendNotification);
	m_EnvSustainLabel.setJustificationType(Justification::centred);
	addAndMakeVisible(m_EnvSustainLabel);

	m_EnvSustainSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	m_EnvSustainSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 60, 20);
	m_EnvSustainAttachment = std::make_unique<SliderAttachment>(m_vts, paramEnvSustain.ID[m_index], m_EnvSustainSlider);
	addAndMakeVisible(m_EnvSustainSlider);
	m_EnvSustainSlider.onValueChange = [this]() {if (somethingChanged != nullptr) somethingChanged(); };

	m_EnvReleaseLabel.setText("Release", NotificationType::dontSendNotification);
	m_EnvReleaseLabel.setJustificationType(Justification::centred);
	addAndMakeVisible(m_EnvReleaseLabel);

	m_EnvReleaseSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	m_EnvReleaseSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 60, 20);
	m_EnvReleaseAttachment = std::make_unique<SliderAttachment>(m_vts, paramEnvRelease.ID[m_index], m_EnvReleaseSlider);
	addAndMakeVisible(m_EnvReleaseSlider);
	m_EnvReleaseSlider.onValueChange = [this]() {if (somethingChanged != nullptr) somethingChanged(); };

}

void EnvelopeParameterComponent::paint(Graphics& g)
{
	g.fillAll((getLookAndFeel().findColour(ResizableWindow::backgroundColourId)).darker(0.1));

}

#define ENV_LABEL_WIDTH 60
#define ENV_ROTARY_WIDTH 60
#define ENV_MIN_DISTANCE 5
#define ENV_LABEL_HEIGHT 20

void EnvelopeParameterComponent::resized()
{
	auto r = getLocalBounds();
	r.reduce(ENV_MIN_DISTANCE,ENV_MIN_DISTANCE);
	auto s = r;
	auto t = r;
	switch (m_style)
	{
	case EnvelopeStyle::compact:

		break;
	case EnvelopeStyle::horizontal:
		s = r.removeFromTop(ENV_LABEL_HEIGHT);

		m_EnvDelayLabel.setBounds(s.removeFromLeft(ENV_LABEL_WIDTH));
		s.removeFromLeft(ENV_MIN_DISTANCE);
		m_EnvAttackLabel.setBounds(s.removeFromLeft(ENV_LABEL_WIDTH));
		s.removeFromLeft(ENV_MIN_DISTANCE);
		m_EnvHoldLabel.setBounds(s.removeFromLeft(ENV_LABEL_WIDTH));
		s.removeFromLeft(ENV_MIN_DISTANCE);
		m_EnvDecayLabel.setBounds(s.removeFromLeft(ENV_LABEL_WIDTH));
		s.removeFromLeft(ENV_MIN_DISTANCE);
		m_EnvSustainLabel.setBounds(s.removeFromLeft(ENV_LABEL_WIDTH));
		s.removeFromLeft(ENV_MIN_DISTANCE);
		m_EnvReleaseLabel.setBounds(s.removeFromLeft(ENV_LABEL_WIDTH));

		s = r;
		t = s.removeFromBottom(ENV_ROTARY_WIDTH);
		m_EnvDelaySlider.setBounds(t.removeFromLeft(ENV_ROTARY_WIDTH));
		t.removeFromLeft(ENV_MIN_DISTANCE);
		m_EnvAttackSlider.setBounds(t.removeFromLeft(ENV_ROTARY_WIDTH));
		t.removeFromLeft(ENV_MIN_DISTANCE);
		m_EnvHoldSlider.setBounds(t.removeFromLeft(ENV_ROTARY_WIDTH));
		t.removeFromLeft(ENV_MIN_DISTANCE);
		m_EnvDecaySlider.setBounds(t.removeFromLeft(ENV_ROTARY_WIDTH));
		t.removeFromLeft(ENV_MIN_DISTANCE);
		m_EnvSustainSlider.setBounds(t.removeFromLeft(ENV_ROTARY_WIDTH));
		t.removeFromLeft(ENV_MIN_DISTANCE);
		m_EnvReleaseSlider.setBounds(t.removeFromLeft(ENV_ROTARY_WIDTH));

		break;
	case EnvelopeStyle::vertical:

		break;
	}
}
