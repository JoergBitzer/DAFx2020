#include "MiniDisplayTest.h"
#include "Parameter.h"

MiniDisplayTest::MiniDisplayTest(double logMinFreq, double logMaxFreq)
	:Component(),
	m_logFreqMax(logMaxFreq),
	m_logFreqMin(logMinFreq),
	m_order(1),
	m_freq(3),
	onValueChange(nullptr)
{
}

void MiniDisplayTest::paint(Graphics& g)
{
	g.fillAll(Colours::aquamarine);
	double maxX = 259.0;
	double startX = 20, startY = 20;
	double len = 1.0 + maxX * (m_freq - m_logFreqMin) / (m_logFreqMax - m_logFreqMin);
	g.drawLine(startX, startY, startX + len, startY, 3.0);

	double endY = jmin(static_cast<double>(getHeight()) - startY, startY + (m_logFreqMax - m_freq) * m_order * 6);
	double endX;
	
	if (endY < (getHeight() - startY))
		endX = getWidth() - startX;
	else
	{
		endX = ((getHeight() - startY) - startY) / 6.0 / m_order + m_freq;
		endX = startX + maxX * (endX - m_logFreqMin) / (m_logFreqMax - m_logFreqMin);
	}
	g.drawLine(startX+len,startY, endX, endY, 3.0);

}

void MiniDisplayTest::mouseDown(const MouseEvent& event)
{
	int x = event.x;
	int y = event.y;
	double maxX = 259.0;
	double startX = 20, startY = 20;
	
	if (x >= startX && x <= startX + maxX && y > 0 && y <= startY)
		m_freq = static_cast<double> (x - startX) / maxX * (m_logFreqMax - m_logFreqMin) + m_logFreqMin;

	if (y > startY && y < getHeight() - startY && x < getWidth() - startX && x > startX + maxX * (m_freq - m_logFreqMin) / (m_logFreqMax - m_logFreqMin))
	{
		double logval = double(x - startX) / maxX * (m_logFreqMax - m_logFreqMin) + m_logFreqMin;
		m_order = (y - startY) / 6.0 / (logval - m_freq);

		if (m_order > paramLpOrder.maxValue)
			m_order = paramLpOrder.maxValue;
	}
	if (onValueChange != nullptr)
		onValueChange();
	//repaint();
}

void MiniDisplayTest::mouseDrag(const MouseEvent& event)
{
	mouseDown(event);
}
