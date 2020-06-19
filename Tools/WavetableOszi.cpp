#include "WavetableOszi.h"

WavetableOszi::WavetableOszi()
	:m_lenOfWavetable(512),
	m_fs(0.0),
	m_curPos(0.0),
	m_curWaveform(0),
	m_curWavetable(0),
	m_fsmin(44100),
	m_nrOfWaveforms(0),
	m_nrOfWavetables(0),
	m_interpolationOrder(1),
	m_incStep(0.0),
	m_freq(1.0),
	m_fft(m_lenOfWavetable),
	m_waveformXFadeFactor(0.0),
	m_secondWaveform(0),
	m_incStepModifier(1.0)
{
	m_f0Table = m_fsmin / m_lenOfWavetable;
	double maxAliasingFreeMidiNote = 108.0;
	double a0 = 440.0;
	double midia0 = 69.0;
	//double maxFreq = pow(2.0, (maxAliasingFreeMidiNote - midia0) / 12.0) * a0;
	int neededOctaves = 1 + static_cast<int> (log(m_fsmin / (2.0 * m_f0Table)) / log(2.0));
	m_semitonespertable = 6;
	m_nrOfWavetables = neededOctaves * 12 / m_semitonespertable;
	
	// aliasing free wavetables
	m_real.resize(m_lenOfWavetable / 2 + 1);
	m_imag.resize(m_lenOfWavetable / 2 + 1);
	m_newwave.resize(m_lenOfWavetable);
	computeIncStep();
	
	m_tunecoarse = 0.0;
	m_tunefine = 0.0;
}

WavetableOszi::~WavetableOszi()
{
}

int WavetableOszi::addF0Wavetable(const std::vector<double>& waveform)
{
	if (waveform.size() != 512)
		return -1;

	m_nrOfWaveforms++;
	m_wavetable.resize(m_nrOfWaveforms);
	m_wavetable[m_nrOfWaveforms - 1].resize(m_nrOfWavetables);
	m_wavetable[m_nrOfWaveforms - 1][0].resize(m_lenOfWavetable + 2*m_interpolationOrder);

	copyNewWavetable(waveform, m_nrOfWaveforms - 1, 0);
	computeFullWavetable();

	return 0;
}

int WavetableOszi::getData(std::vector<double>& data)
{
	double* pDataLow = &m_wavetable[m_curWaveform][m_curWavetable][0];
	pDataLow += m_interpolationOrder;
	double* pDataSecond = &m_wavetable[m_secondWaveform][m_curWavetable][0];
	pDataSecond += m_interpolationOrder;
	for (auto kk = 0; kk < data.size(); kk++)
	{
		int pos = int(m_curPos);
		double fracpos = m_curPos - pos;

		double out = (1.0 - fracpos) * *(pDataLow + pos) + fracpos * *(pDataLow + pos + 1);
		double out2 = (1.0 - fracpos) * *(pDataSecond + pos) + fracpos * *(pDataSecond + pos + 1);

		data[kk] = m_level*((1.0-m_waveformXFadeFactor) * out + m_waveformXFadeFactor*out2);

		if (m_portamentoCounter > 0)
		{
			--m_portamentoCounter;
			m_incStep *= m_portamentoFactor;
		}

		m_curPos += (m_incStep*m_incStepModifier);
		if (m_curPos >= m_lenOfWavetable)
			m_curPos -= m_lenOfWavetable;

	}
	return 0;
}

void WavetableOszi::setSamplerate(double samplerate)
{
	m_fs = samplerate;
	m_f0Table = m_fs / m_lenOfWavetable;
	computeIncStep();
}

void WavetableOszi::setFrequency(double freq)
{
	m_freq = freq;
	if (m_portamentoTime_samples <= 0)
	{
		computeIncStep();
		m_portamentoCounter = -1;
	}
	else
	{
		m_portamentoCounter = m_portamentoTime_samples;
		double targetInc = m_freq / m_f0Table;
		m_portamentoFactor = exp(log(targetInc / m_incStep) / m_portamentoTime_samples);
	}
	m_curWavetable = static_cast<int> (log(freq / m_f0Table) / log(pow(2.0, double(m_semitonespertable) / 12.0)));
	if (m_curWavetable < 0)
		m_curWavetable = 0 ;
}

void WavetableOszi::computeIncStep()
{
	m_incStep = m_freq*m_tunefinal / m_f0Table;
}
void WavetableOszi::computeTuning()
{
	m_tunefinal = pow(2.0, (m_tunecoarse + m_tunefine * 0.01) / 12.0);
	computeIncStep();
}
void WavetableOszi::setWaveform(int newwaveformnumber)
{
	if (newwaveformnumber < m_nrOfWaveforms)
		m_curWaveform = newwaveformnumber;
}

void WavetableOszi::setWaveformXFade(double xfadefactor)
{
	if (xfadefactor >= 0.0 && xfadefactor <= 1.0)
		m_waveformXFadeFactor = xfadefactor;
}
void WavetableOszi::setSecondWaveform(int waveformnumber)
{
	if (waveformnumber < m_nrOfWaveforms)
		m_secondWaveform = waveformnumber;

}

void WavetableOszi::resetPhase()
{
	m_curPos = 0.0;
}

void WavetableOszi::changeFrequencyRelative(double change_semitones)
{
	m_incStepModifier = pow(2.0, change_semitones / 12.0);
}

void WavetableOszi::setPortamentoTime(double portaTime_ms)
{
	m_portamentoTime_samples = static_cast<int>(portaTime_ms * 0.001 * m_fs);
}

void WavetableOszi::setLevel(double level_dB)
{
	m_level = pow(10.0,level_dB/20.0);
}

void WavetableOszi::setTuneCoarse(float semitones)
{
	m_tunecoarse = semitones;
	computeTuning();
}

void WavetableOszi::setTuneFine(float cents)
{
	m_tunefine = cents;
	computeTuning();
}

void WavetableOszi::computeFullWavetable()
{
	double* pData = m_wavetable[m_nrOfWaveforms-1][0].data();
	pData += m_interpolationOrder;
	double* pReal = m_real.data();
	double* pImag = m_imag.data();

	m_fft.fft(pData, pReal, pImag);
	double NewFreq = 16000.0 / (m_fsmin / 2.0);

	for (auto tt = 1; tt < m_nrOfWavetables; tt++)
	{
		NewFreq = NewFreq * pow(2.0, double(-m_semitonespertable) / 12.0);
		int maxIndex = static_cast<int>(NewFreq * m_real.size());
		for (auto ff = maxIndex; ff < m_real.size(); ff++)
		{
			m_real[ff] = 0.0;
			m_imag[ff] = 0.0;
		}
		for (auto ff = 1; ff < maxIndex; ff++)
		{
			double LZsigma = sin(M_PI * double(ff-1) / maxIndex + 0.000000000001) / (M_PI * double(ff-1) / maxIndex + 0.000000000001);
			m_real[ff] *= LZsigma;
			m_imag[ff] *= LZsigma;
		}
		pData = m_newwave.data();
		m_fft.ifft(pReal, pImag, pData);
		for (auto ff = 1; ff < maxIndex; ff++)
		{
			double LZsigma = sin(M_PI * double(ff-1) / maxIndex + 0.000000000001) / (M_PI * double(ff-1) / maxIndex + 0.000000000001);
			m_real[ff] /= LZsigma;
			m_imag[ff] /= LZsigma;
		}
		m_wavetable[m_nrOfWaveforms - 1][tt].resize(m_lenOfWavetable + 2 * m_interpolationOrder);

		copyNewWavetable(m_newwave, m_nrOfWaveforms - 1, tt);
	}
}

void WavetableOszi::copyNewWavetable(std::vector<double> newwaveform,int nrWaveform, int nrWavetable)
{
	std::copy(newwaveform.begin(), newwaveform.end(), m_wavetable[m_nrOfWaveforms - 1][nrWavetable].begin() + m_interpolationOrder);
	for (auto kk = 0; kk < m_interpolationOrder; kk++)
	{
		m_wavetable[nrWaveform][nrWavetable][kk] = newwaveform[newwaveform.size() - m_interpolationOrder + kk];
		m_wavetable[nrWaveform][nrWavetable][m_lenOfWavetable + m_interpolationOrder + kk] = newwaveform[kk];
	}
}

int OscParameter::addParameter(std::vector<std::unique_ptr<RangedAudioParameter>>& paramVector, int instance)
{
	if (instance < MAX_OSC_INSTANCES)
	{
		StringArray choiceNames(paramOscWaveform1.Choices);

		paramVector.push_back(std::make_unique<AudioParameterChoice>(paramOscWaveform1.ID[instance],
			paramOscWaveform1.name,
			choiceNames,
			paramOscWaveform1.defaultIndex,
			paramOscWaveform1.unitName));

		paramVector.push_back(std::make_unique<AudioParameterChoice>(paramOscWaveform2.ID[instance],
			paramOscWaveform2.name,
			choiceNames,
			paramOscWaveform2.defaultIndex,
			paramOscWaveform2.unitName));


		paramVector.push_back(std::make_unique<AudioParameterFloat>(paramOscTuneCoarse.ID[instance],
			paramOscTuneCoarse.name,
			NormalisableRange<float>(paramOscTuneCoarse.minValue, paramOscTuneCoarse.maxValue),
			paramOscTuneCoarse.defaultValue,
			paramOscTuneCoarse.unitName,
			AudioProcessorParameter::genericParameter,
			[](float value, int MaxLen) { return (String(float(int(value + 0.5)), MaxLen) + " semi"); },
			[](const String& text) {return text.getFloatValue(); }));

		paramVector.push_back(std::make_unique<AudioParameterFloat>(paramOscTuneFine.ID[instance],
			paramOscTuneFine.name,
			NormalisableRange<float>(paramOscTuneFine.minValue, paramOscTuneFine.maxValue),
			paramOscTuneFine.defaultValue,
			paramOscTuneFine.unitName,
			AudioProcessorParameter::genericParameter,
			[](float value, int MaxLen) { return (String((0.1 * int(value * 10 + 0.5)), MaxLen) + " cent"); },
			[](const String& text) {return text.getFloatValue(); }));

		paramVector.push_back(std::make_unique<AudioParameterFloat>(paramOscXFade.ID[instance],
			paramOscXFade.name,
			NormalisableRange<float>(paramOscXFade.minValue, paramOscXFade.maxValue),
			paramOscXFade.defaultValue,
			paramOscXFade.unitName,
			AudioProcessorParameter::genericParameter,
			[](float value, int MaxLen) { return (String((0.1 * int(value * 10 + 0.5)), MaxLen) + " %"); },
			[](const String& text) {return text.getFloatValue(); }));

		paramVector.push_back(std::make_unique<AudioParameterFloat>(paramOscLevel.ID[instance],
			paramOscLevel.name,
			NormalisableRange<float>(paramOscLevel.minValue, paramOscLevel.maxValue),
			paramOscLevel.defaultValue,
			paramOscLevel.unitName,
			AudioProcessorParameter::genericParameter,
			[](float value, int MaxLen) { return (String((0.1 * int(value * 10 + 0.5)), MaxLen) + " dB"); },
			[](const String& text) {return text.getFloatValue(); }));

		paramVector.push_back(std::make_unique<AudioParameterFloat>(paramOscModDepth.ID[instance],
			paramOscModDepth.name,
			NormalisableRange<float>(paramOscModDepth.minValue, paramOscModDepth.maxValue),
			paramOscModDepth.defaultValue,
			paramOscModDepth.unitName,
			AudioProcessorParameter::genericParameter,
			[](float value, int MaxLen) { return (String((0.1 * int(value * 10 + 0.5)), MaxLen)); },
			[](const String& text) {return  text.getFloatValue(); }));

	}
	return 0;
}

OscParameterComponent::OscParameterComponent(AudioProcessorValueTreeState& vts, int index, const String& oscName)
	:m_vts(vts), somethingChanged(nullptr), m_name(oscName),m_index(index)
{

	// Waveform

	m_osc1waveform1Label.setText("Waveform 1", NotificationType::dontSendNotification);
	m_osc1waveform1Label.setJustificationType(Justification::centred);
	addAndMakeVisible(m_osc1waveform1Label);

	StringArray WaveformNames(paramOscWaveform1.Choices);
	m_osc1waveform1Combo.addItemList(WaveformNames, 1);
	m_osc1waveform1Combo.setSelectedItemIndex(0, false);
	m_osc1waveform1Combo.setJustificationType(Justification::centred);
	m_osc1waveform1Combo.onChange = [this]() {if (somethingChanged != nullptr) somethingChanged(); };
	addAndMakeVisible(m_osc1waveform1Combo);
	m_osc1waveform1Attachment = std::make_unique<ComboAttachment>(m_vts, paramOscWaveform1.ID[m_index], m_osc1waveform1Combo);


	m_osc1waveform2Label.setText("Waveform 2", NotificationType::dontSendNotification);
	m_osc1waveform2Label.setJustificationType(Justification::centred);
	addAndMakeVisible(m_osc1waveform2Label);

	//StringArray WaveformNames(paramOscWaveform1.Choices);
	m_osc1waveform2Combo.addItemList(WaveformNames, 1);
	m_osc1waveform2Combo.setSelectedItemIndex(0, false);
	m_osc1waveform2Combo.setJustificationType(Justification::centred);
	m_osc1waveform2Combo.onChange = [this]() {if (somethingChanged != nullptr) somethingChanged(); };
	addAndMakeVisible(m_osc1waveform2Combo);
	m_osc1waveform2Attachment = std::make_unique<ComboAttachment>(m_vts, paramOscWaveform2.ID[m_index], m_osc1waveform2Combo);


	// XFAde
	m_osc1xfadeLabel.setText("Mix", NotificationType::dontSendNotification);
	m_osc1xfadeLabel.setJustificationType(Justification::centred);
	addAndMakeVisible(m_osc1xfadeLabel);
	
	m_osc1xfadeSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	m_osc1xfadeSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 60, 20);
	m_osc1xfadeAttachment = std::make_unique<SliderAttachment>(m_vts, paramOscXFade.ID[m_index], m_osc1xfadeSlider);
	addAndMakeVisible(m_osc1xfadeSlider);
	m_osc1xfadeSlider.onValueChange = [this]() {if (somethingChanged != nullptr) somethingChanged(); };
	
	// tune
	m_osc1tunecoarseLabel.setText("Coarse", NotificationType::dontSendNotification);
	m_osc1tunecoarseLabel.setJustificationType(Justification::centred);
	addAndMakeVisible(m_osc1tunecoarseLabel);

	m_osc1tunecoarseSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	m_osc1tunecoarseSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 45, 20);
	m_osc1tunecoarseAttachment = std::make_unique<SliderAttachment>(m_vts, paramOscTuneCoarse.ID[m_index], m_osc1tunecoarseSlider);
	addAndMakeVisible(m_osc1tunecoarseSlider);
	m_osc1tunecoarseSlider.onValueChange = [this]() {if (somethingChanged != nullptr) somethingChanged(); };

	m_osc1tunefineLabel.setText("Fine", NotificationType::dontSendNotification);
	m_osc1tunefineLabel.setJustificationType(Justification::centred);
	addAndMakeVisible(m_osc1tunefineLabel);

	m_osc1tunefineSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	m_osc1tunefineSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 45, 20);
	m_osc1tunefineAttachment = std::make_unique<SliderAttachment>(m_vts, paramOscTuneFine.ID[m_index], m_osc1tunefineSlider);
	addAndMakeVisible(m_osc1tunefineSlider);
	m_osc1tunefineSlider.onValueChange = [this]() {if (somethingChanged != nullptr) somethingChanged(); };

	m_osc1levelLabel.setText("Level", NotificationType::dontSendNotification);
	m_osc1levelLabel.setJustificationType(Justification::centred);
	addAndMakeVisible(m_osc1levelLabel);

	m_osc1levelSlider.setSliderStyle(Slider::SliderStyle::Rotary);
	m_osc1levelSlider.setTextBoxStyle(Slider::TextEntryBoxPosition::TextBoxBelow, true, 45, 20);
	m_osc1levelAttachment = std::make_unique<SliderAttachment>(m_vts, paramOscLevel.ID[m_index], m_osc1levelSlider);
	addAndMakeVisible(m_osc1levelSlider);
	m_osc1levelSlider.onValueChange = [this]() {if (somethingChanged != nullptr) somethingChanged(); };

}

void OscParameterComponent::paint(Graphics& g)
{
	g.fillAll((getLookAndFeel().findColour(ResizableWindow::backgroundColourId)).darker(0.1));
}

#define OSC_LABEL_WIDTH 90
#define OSC_COMBO_WIDTH 90
#define OSC_COMBO_HEIGHT 30
#define OSC_ROTARY_WIDTH 60
#define OSC_SMALLROTARY_WIDTH 45
#define OSC_MIN_DISTANCE 5
void OscParameterComponent::resized()
{
	auto r = getLocalBounds();
	r.removeFromLeft(OSC_MIN_DISTANCE);
	r.removeFromTop(OSC_MIN_DISTANCE);
	auto s = r;
	s.removeFromBottom(getHeight()-OSC_COMBO_HEIGHT);
	m_osc1waveform1Label.setBounds(s.removeFromLeft(OSC_LABEL_WIDTH));

	// label xfade
	s.removeFromLeft(OSC_MIN_DISTANCE);
	m_osc1xfadeLabel.setBounds(s.removeFromLeft(OSC_ROTARY_WIDTH));

	// label waveform2
	s.removeFromLeft(OSC_MIN_DISTANCE);
	m_osc1waveform2Label.setBounds(s.removeFromLeft(OSC_LABEL_WIDTH));


	s = r;
	s.removeFromTop(OSC_COMBO_HEIGHT);
	s.removeFromBottom(getHeight() - 2*OSC_COMBO_HEIGHT); // hoehe der Reihe
	m_osc1waveform1Combo.setBounds(s.removeFromLeft(OSC_COMBO_WIDTH));
	
	s = r;
	s.removeFromTop(OSC_COMBO_HEIGHT);
	s.removeFromBottom(getHeight() - 3 * OSC_COMBO_HEIGHT);
	s.removeFromLeft(OSC_MIN_DISTANCE + OSC_COMBO_WIDTH);
	m_osc1xfadeSlider.setBounds(s.removeFromLeft(OSC_ROTARY_WIDTH));

	s = r;
	s.removeFromTop(OSC_COMBO_HEIGHT);
	s.removeFromBottom(getHeight() - 2 * OSC_COMBO_HEIGHT);
	s.removeFromLeft(2* OSC_MIN_DISTANCE + OSC_COMBO_WIDTH + OSC_ROTARY_WIDTH);
	m_osc1waveform2Combo.setBounds(s.removeFromLeft(OSC_COMBO_WIDTH));


// second row
	s = r;
	s.removeFromTop(2*OSC_COMBO_HEIGHT + 2 * OSC_MIN_DISTANCE);
	s.removeFromBottom(getHeight() -  3*OSC_COMBO_HEIGHT);
	m_osc1tunecoarseLabel.setBounds(s.removeFromLeft(OSC_SMALLROTARY_WIDTH));
	//s.removeFromLeft(OSC_MIN_DISTANCE);
	m_osc1tunefineLabel.setBounds(s.removeFromLeft(OSC_SMALLROTARY_WIDTH));
	s.removeFromRight(OSC_MIN_DISTANCE);
	m_osc1levelLabel.setBounds(s.removeFromRight(OSC_SMALLROTARY_WIDTH));


	s = r;
	auto DistTop = 3 * OSC_COMBO_HEIGHT ;
	s.removeFromTop(DistTop);
	s.removeFromBottom(getHeight() - DistTop - 2*OSC_COMBO_HEIGHT );
	m_osc1tunecoarseSlider.setBounds(s.removeFromLeft(OSC_SMALLROTARY_WIDTH));

	m_osc1tunefineSlider.setBounds(s.removeFromLeft(OSC_SMALLROTARY_WIDTH));

	s.removeFromRight(OSC_MIN_DISTANCE);
	m_osc1levelSlider.setBounds(s.removeFromRight(OSC_SMALLROTARY_WIDTH));
	
}
