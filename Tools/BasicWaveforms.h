/*
BasicWaveforms computes some waveforms by using Fourier series
author: Joerg Bitzer @ TGM @ Jade Hochschule 
date:  Mai 2019

version 1.0 JB init
version 1.1 extended to rectPulse by A. Schiller (TGM student)

*/
#pragma once
#define _USE_MATH_DEFINES

#include <cmath>
#include <vector>
#include "WhiteNoiseGenerator.h"

class BasicWaveforms
{
public:
	enum waveforms
	{
		rect = 0,
		saw,
		tri,
		rectPulse,
		rectPWM,
		noise,
		nrofwaveforms
	};

	BasicWaveforms(int lenWavetable, double maxFreq, double tablef0);
	void getWavetable(std::vector<double>& wavetable, waveforms waveform, int addParam = -1);
	void setDutyCycle(double durtyCicle);
	void setSeed(int seed) { m_wng.setSeed(seed); };

private:
	double m_maxFreq;
	double m_tablef0;
	int m_len;
	int m_maxHarm;
	void computeRect();
	void computeSaw();
	void computeTriangle();
	void computeNoise(int seed);
	void computeRectPWM(int DutyCycle_percent);
	double m_dutyCycle;
	void computeRectPulse();

	std::vector<double> m_waveform;
	WhiteNoiseGenerator m_wng;
};

