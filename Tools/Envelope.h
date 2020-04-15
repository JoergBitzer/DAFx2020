/*----------------------------------------------------------------------------------------
envelope class: Implements a typical synthesizer envelope
version: 0.01 first build
version 1.0 ADSR tested and works fine
author: Martin Berdau
(c) BSD 3-clause licence (https://opensource.org/licenses/BSD-3-Clause)
-----------------------------------------------------------------------------------------*/

// Aufgabe: Erweiterung auf Extended ADSR
// Delay und Hold als Phasen einbauen. -> Angaben in Zeit -> Samples zählen

#pragma once

#include <vector>
#include <math.h>

class Envelope
{
public:
	enum envelopePhases
	{
		Off = 0,
		Delay,
		Attack,
		Hold,
		Decay,
		Sustain,
		Release,
	};
	Envelope();
	~Envelope();

	int getData(std::vector<double>& data);
	void NoteOn(); //to do: was passiert wenn noteon in einem Block?
	void NoteOff();
	void setSamplerate(double samplerate);
	void setDelayTime(double del_ms);
	void setAttackRate(double att_ms);
	void setHoldTime(double hold_ms);
	void setDecayRate(double dec_ms);
	void setSustainLevel(double level);
	void setReleaseRate(double rel_ms);
	envelopePhases getEnvelopeStatus() { return m_envelopePhase; };

protected:
	void updateTimeConstants(void);
	double m_fs;

	double m_delTime_ms;
	double m_delSamples;

	double m_tauAtt_ms;
	double m_alphaAtt;
	double m_attOver;

	double m_holdTime_ms;
	double m_holdSamples;

	double m_tauDec_ms;
	double m_alphaDec;

	double m_tauRel_ms;
	double m_alphaRel;

	double m_sustainLevel;

	envelopePhases m_envelopePhase;
	double m_envGain;

	int m_sampleCounter;

};

