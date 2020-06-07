#include "NanoSynth.h"
#include "NanoVoice.h"

NanoSynth::NanoSynth()
	:Synthesiser()
{
}

void NanoSynth::prepareToPlay(double samplerate, int samplesperblock)
{
	m_fs = samplerate;
	m_blocksize = samplesperblock;

	const ScopedLock sl(lock); // lock is defined in Synthesizer

	Synthesiser::setCurrentPlaybackSampleRate(m_fs);

	for (int kk = 0; kk < voices.size(); ++kk)
	{
		NanoVoice* const onenanovoice = dynamic_cast<NanoVoice*>(voices.getUnchecked(kk));
		onenanovoice->prepareVoice(m_fs, m_blocksize);
	}

}
