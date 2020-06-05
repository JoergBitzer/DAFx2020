#pragma once

#include <JuceHeader.h>

class MiniDisplayTest :public Component
{
public:
	MiniDisplayTest(double logFreqMin, double logFreqMax);
	void paint(Graphics& g) override;
	void setLPFreq(double logFreq) { m_freq = logFreq; repaint(); };
	void setLPOrder(double order) { m_order = order; repaint(); };
	void mouseDown(const MouseEvent& event) override;
	void mouseDrag(const MouseEvent& event) override;
	std::function<void()> onValueChange;
	double getLPorder() { return m_order; };
	double getFreqLP() { return m_freq; };

private:
	double m_logFreqMin;
	double m_logFreqMax;
	double m_order;
	double m_freq;
};

