#include "JadeMiniSynth.h"
#include "JadeMiniVoice.h"

JadeMiniSynth::JadeMiniSynth()
	:Synthesiser()
{
}

void JadeMiniSynth::prepareToPlay(double samplerate, int samplesperblock)
{
	m_fs = samplerate;
	m_blocksize = samplesperblock;

	const ScopedLock sl(lock); // lock is defined in Synthesizer

	Synthesiser::setCurrentPlaybackSampleRate(m_fs);

	for (int kk = 0; kk < voices.size(); ++kk)
	{
		JadeMiniVoice* const oneJadeMinivoice = dynamic_cast<JadeMiniVoice*>(voices.getUnchecked(kk));
		oneJadeMinivoice->prepareVoice(m_fs, m_blocksize);
	}

}
