#include "WhiteNoiseGenerator.h"

WhiteNoiseGenerator::WhiteNoiseGenerator()
{
	m_amplitude = 1.0;
	m_grainFactor = 0.98;
	std::random_device rd;
	m_mt = std::mt19937(rd());
	m_dist = std::uniform_real_distribution<double>(-M_PI*0.5, M_PI*0.5);
}

WhiteNoiseGenerator::~WhiteNoiseGenerator()
{
}

int WhiteNoiseGenerator::getData(std::vector<double>& data)
{
	for (unsigned int kk = 0; kk < data.size(); kk++)
	{
		double noise = m_dist(m_mt); 
		data.at(kk) = m_normalize * m_amplitude * tan(noise * m_grainFactor);
	}
	return 0;
}

void WhiteNoiseGenerator::setGraininess(double grainFak)
{
	if ((grainFak >= 0.0) & (grainFak <= 1.0))
	{
		m_grainFactor = 0.99998 * (0.8 + (grainFak * 0.2));
	}
	m_normalize = 1.0 / tan(m_grainFactor * M_PI * 0.5);
}
