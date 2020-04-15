/*------------------------------------------------
envelope class: Implements a typical synthesizer envelope
version: 0.01 first build
version: 1.0 ADSR tested and works fine 
author: j.bitzer@tgm @ jade hs Oldenburg
(c) BSD 3-clause license (https://opensource.org/licenses/BSD-3-Clause)
------------------------------------------------*/

// Aufgabe: Erweiterung auf Extended ADSR
// Delay und Hold als Phasen einbauen.

#pragma once

#include <vector>

class Envelope
{
public:
	enum envelopePhases
	{
		Off = 0, 
		Attack,
		Decay,
		Sustain,
		Release
	};
	Envelope();
	~Envelope();

	int getData(std::vector<double>& data);
	void NoteOn(); // to do was passiert wenn noteon in einem Block
	void NoteOff();
	void setSamplerate(double samplerate);
	void setAttackRate(double att_ms);
	void setDecayRate(double dec_ms);
	void setSustainLevel(double level);
	void setReleaseRate(double rel_ms);
	envelopePhases getEnvelopeStatus() { return m_envelopePhase; };

protected:
	void updateTimeConstants(void);
	double m_fs;
	double m_tauAtt_ms;
	double m_alphaAtt;
	double m_attOver;

	double m_tauDec_ms;
	double m_alphaDec;

	double m_tauRel_ms;
	double m_alphaRel;

	double m_sustainLevel;

	envelopePhases m_envelopePhase;
	double m_envGain;

};

