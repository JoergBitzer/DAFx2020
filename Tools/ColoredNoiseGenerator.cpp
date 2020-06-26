#include "ColoredNoiseGenerator.h"

ColoredNoiseGenerator::ColoredNoiseGenerator()
	:WhiteNoiseGenerator()
{
	m_filterorder = 4;
	m_nrofcoefficients = m_filterorder + 1;
	m_b.resize(m_nrofcoefficients);
	m_a.resize(m_nrofcoefficients);
	std::fill(m_b.begin(), m_b.end(), 0.0);
	std::fill(m_a.begin(), m_a.end(), 0.0);
	m_b[0] = 1.0;
	m_a[0] = 1.0;
	m_filter.setOutputFilterOrder(m_filterorder, m_filterorder);

}

ColoredNoiseGenerator::~ColoredNoiseGenerator()
{
}

int ColoredNoiseGenerator::getData(std::vector<double>& data)
{
	int nrofsamples = data.size();
	for (unsigned int kk = 0; kk < nrofsamples; kk++)
	{
		data.at(kk) = getOneSample();
	}
	m_lowpass.processData(data, data);
	m_highpass.processData(data, data);

	return 0;
}
#define ORDER_OFF 0.3
void ColoredNoiseGenerator::computeCoeffs(FreeOrderLowHighpassFilter::FilterType type)
{
	std::fill(m_b.begin(), m_b.end(), 0.0);
	std::fill(m_a.begin(), m_a.end(), 0.0);
	m_b[0] = 1.0;
	m_a[0] = 1.0;
	//Hier Filterdesign
	// Lowpass
	if (m_lporder >= ORDER_OFF && type == FreeOrderLowHighpassFilter::FilterType::lowpass)
		m_filter.getCoefficients(m_b, m_a, m_fs, m_lpcut, m_lporder, FreeOrderLowHighpassFilter::FilterType::lowpass);

	if (m_hporder >= ORDER_OFF && type == FreeOrderLowHighpassFilter::FilterType::highpass)
		m_filter.getCoefficients(m_b, m_a, m_fs, m_hpcut, m_hporder, FreeOrderLowHighpassFilter::FilterType::highpass);
	//Setzen der Koeffizienten in den Filtern
	if (type == FreeOrderLowHighpassFilter::FilterType::lowpass)
		m_lowpass.setCoeffs(m_b, m_a);
	if (type == FreeOrderLowHighpassFilter::FilterType::highpass)
		m_highpass.setCoeffs(m_b, m_a);

}

#ifdef USE_JUCE

int ColoredNoiseParameter::addParameter(std::vector<std::unique_ptr<RangedAudioParameter>>& paramVector)
{
	paramVector.push_back(std::make_unique<AudioParameterFloat>(paramNoiseColorLow.ID,
		paramNoiseColorLow.name,
		NormalisableRange<float>(paramNoiseColorLow.minValue, paramNoiseColorLow.maxValue),
		paramNoiseColorLow.defaultValue,
		paramNoiseColorLow.unitName,
		AudioProcessorParameter::genericParameter,
		[](float value, int MaxLen) { value = int((value) * 10) * 0.1;  return String(value, MaxLen); },
		[](const String& text) {return text.getFloatValue(); }));

	paramVector.push_back(std::make_unique<AudioParameterFloat>(paramNoiseColorHigh.ID,
		paramNoiseColorHigh.name,
		NormalisableRange<float>(paramNoiseColorHigh.minValue, paramNoiseColorHigh.maxValue),
		paramNoiseColorHigh.defaultValue,
		paramNoiseColorHigh.unitName,
		AudioProcessorParameter::genericParameter,
		[](float value, int MaxLen) { value = int((value) * 10) * 0.1;  return String(value, MaxLen); },
		[](const String& text) {return text.getFloatValue(); }));


	return 0;
}

#endif

ColoredNoiseComponent::ColoredNoiseComponent(AudioProcessorValueTreeState& vts)
	:m_vts(vts), somethingChanged(nullptr), m_style(ComponentStyle::horizontal)
{
	m_NoiseLevelLabel.setText("Level", NotificationType::dontSendNotification);
	m_NoiseLevelLabel.setJustificationType(Justification::centred);
	addAndMakeVisible(m_NoiseLevelLabel);

	m_NoiseLevelSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	m_NoiseLevelSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 60, 20);
	m_NoiseLevelAttachment = std::make_unique<SliderAttachment>(m_vts, paramNoiseLevel.ID, m_NoiseLevelSlider);
	addAndMakeVisible(m_NoiseLevelSlider);
	m_NoiseLevelSlider.onValueChange = [this]() {if (somethingChanged != nullptr) somethingChanged(); };

	m_NoiseGraininessLabel.setText("Graininess", NotificationType::dontSendNotification);
	m_NoiseGraininessLabel.setJustificationType(Justification::centred);
	addAndMakeVisible(m_NoiseGraininessLabel);

	m_NoiseGraininessSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	m_NoiseGraininessSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 60, 20);
	m_NoiseGraininessAttachment = std::make_unique<SliderAttachment>(m_vts, paramNoiseGraininess.ID, m_NoiseGraininessSlider);
	addAndMakeVisible(m_NoiseGraininessSlider);
	m_NoiseGraininessSlider.onValueChange = [this]() {if (somethingChanged != nullptr) somethingChanged(); };

	m_NoiseColorLowLabel.setText("ColorLow", NotificationType::dontSendNotification);
	m_NoiseColorLowLabel.setJustificationType(Justification::centred);
	addAndMakeVisible(m_NoiseColorLowLabel);

	m_NoiseColorLowSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	m_NoiseColorLowSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 60, 20);
	m_NoiseColorLowAttachment = std::make_unique<SliderAttachment>(m_vts, paramNoiseColorLow.ID, m_NoiseColorLowSlider);
	addAndMakeVisible(m_NoiseColorLowSlider);
	m_NoiseColorLowSlider.onValueChange = [this]() {if (somethingChanged != nullptr) somethingChanged(); };

	m_NoiseColorHighLabel.setText("ColorHigh", NotificationType::dontSendNotification);
	m_NoiseColorHighLabel.setJustificationType(Justification::centred);
	addAndMakeVisible(m_NoiseColorHighLabel);

	m_NoiseColorHighSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	m_NoiseColorHighSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 60, 20);
	m_NoiseColorHighAttachment = std::make_unique<SliderAttachment>(m_vts, paramNoiseColorHigh.ID, m_NoiseColorHighSlider);
	addAndMakeVisible(m_NoiseColorHighSlider);
	m_NoiseColorHighSlider.onValueChange = [this]() {if (somethingChanged != nullptr) somethingChanged(); };


}

void ColoredNoiseComponent::paint(Graphics& g)
{
	g.fillAll((getLookAndFeel().findColour(ResizableWindow::backgroundColourId)).darker(0.1));

}

#define NOISE_LABEL_WIDTH 60
#define NOISE_ROTARY_WIDTH 60
#define NOISE_MIN_DISTANCE 5
#define NOISE_LABEL_HEIGHT 20


void ColoredNoiseComponent::resized()
{
	auto r = getLocalBounds();
	r.reduce(NOISE_MIN_DISTANCE, NOISE_MIN_DISTANCE);
	auto s = r;
	auto t = r;
	switch (m_style)
	{
	case ComponentStyle::compact:

		break;
	case ComponentStyle::horizontal:
		s = r.removeFromTop(NOISE_LABEL_HEIGHT);
		m_NoiseGraininessLabel.setBounds(s.removeFromLeft(NOISE_LABEL_WIDTH));
		s.removeFromLeft(NOISE_MIN_DISTANCE);
		m_NoiseColorLowLabel.setBounds(s.removeFromLeft(NOISE_LABEL_WIDTH));
		s.removeFromLeft(NOISE_MIN_DISTANCE);
		m_NoiseColorHighLabel.setBounds(s.removeFromLeft(NOISE_LABEL_WIDTH));
		s.removeFromLeft(NOISE_MIN_DISTANCE);
		m_NoiseLevelLabel.setBounds(s.removeFromLeft(NOISE_LABEL_WIDTH));

		s = r;
		t = s.removeFromBottom(NOISE_ROTARY_WIDTH);
		m_NoiseGraininessSlider.setBounds(t.removeFromLeft(NOISE_ROTARY_WIDTH));
		t.removeFromLeft(NOISE_MIN_DISTANCE);
		m_NoiseColorLowSlider.setBounds(t.removeFromLeft(NOISE_ROTARY_WIDTH));
		t.removeFromLeft(NOISE_MIN_DISTANCE);
		m_NoiseColorHighSlider.setBounds(t.removeFromLeft(NOISE_ROTARY_WIDTH));
		t.removeFromLeft(NOISE_MIN_DISTANCE);
		m_NoiseLevelSlider.setBounds(t.removeFromLeft(NOISE_ROTARY_WIDTH));

		break;
	case ComponentStyle::vertical:

		break;
	}



}
