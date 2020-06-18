#pragma once

#include <JuceHeader.h>


class JadeMiniSynth : public Synthesiser
{
public: 
	JadeMiniSynth();
	void prepareToPlay(double samplerate, int samplesperblock);

private:
	double m_fs;
	int m_blocksize;
};

