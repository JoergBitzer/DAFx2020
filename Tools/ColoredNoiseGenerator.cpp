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
