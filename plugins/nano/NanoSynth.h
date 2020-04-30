#pragma once

#include "../JuceLibraryCode/JuceHeader.h"


class NanoSynth : public Synthesiser
{
public: 
	NanoSynth();
	void prepareToPlay(double samplerate, int samplesperblock);

private:
	double m_fs;
	int m_blocksize;
};

