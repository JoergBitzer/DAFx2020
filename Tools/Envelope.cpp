#include "Envelope.h"

Envelope::Envelope()
{
	m_fs = 1.0;
	m_alphaAtt = 0.0;
	m_attOver = 1.0 / 0.77; // from Will Pirkle Book
	m_alphaDec = 0.0;
	m_envGain = 0.0;
	m_envelopePhase = envelopePhases::Off;
}

Envelope::~Envelope()
{
}

int Envelope::getData(std::vector<double>& data)
{
	for (unsigned int kk = 0; kk < data.size(); kk++)
	{
		double Gain;
		switch (m_envelopePhase)
		{
		case envelopePhases::Off:
			m_envGain = 0.0;
			break;

		case envelopePhases::Attack:
			Gain = m_alphaAtt * m_envGain + (1 - m_alphaAtt) * m_attOver;
			m_envGain = Gain;
			if (m_envGain >= 1.0)
			{
				m_envelopePhase = envelopePhases::Decay;
				m_envGain = 1.0;
			}
			break;

		case envelopePhases::Decay:
			Gain = m_alphaDec * m_envGain + (1 - m_alphaDec) * m_sustainLevel;
			m_envGain = Gain;
			break;
		
		case envelopePhases::Release:
			Gain = m_alphaRel * m_envGain;
			m_envGain = Gain;
			if (m_envGain <= 1.0e-05)
				m_envelopePhase = envelopePhases::Off;

			break;
		}
		data.at(kk) = m_envGain;
	}
	return 0;
}

void Envelope::NoteOn()
{
	m_envelopePhase = envelopePhases::Attack;
}

void Envelope::NoteOff()
{
	m_envelopePhase = envelopePhases::Release;
}

void Envelope::setSamplerate(double samplerate)
{
	m_fs = samplerate;
	updateTimeConstants();
}

void Envelope::setAttackRate(double att_ms)
{
	m_tauAtt_ms = att_ms;
	updateTimeConstants();
}

void Envelope::setDecayRate(double dec_ms)
{
	m_tauDec_ms = dec_ms;
	updateTimeConstants();
}

void Envelope::setSustainLevel(double level)
{
	m_sustainLevel = level;
}

void Envelope::setReleaseRate(double rel_ms)
{
	m_tauRel_ms = rel_ms;
	updateTimeConstants();
}

void Envelope::updateTimeConstants(void)
{
	m_alphaAtt = exp(-1.0 / (m_tauAtt_ms * 0.001 * m_fs));
	m_alphaDec = exp(-1.0 / (m_tauDec_ms * 0.001 * m_fs));
	m_alphaRel = exp(-1.0 / (m_tauRel_ms * 0.001 * m_fs));
}
