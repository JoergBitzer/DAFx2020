/*****************************************************************//**
 * \file   WavetableOszi.h
 * \brief  Wavetable oscillator without aliasing below 16kHz
 * 
 * 
 * \author Bitzer
 * \date   April 2020
***********************************************************************/
#pragma once
#define _USE_MATH_DEFINES

#include <cmath>
#include <vector>
#include "FFT.h"
/**
 * \class  WavetableOsci class
 */
class WavetableOszi
{
public:
	/**
	 * \brief the constructor without arguments (samplerate is often not known )
	*/
	WavetableOszi();
	~WavetableOszi();

	/**
	 * method to add a new waveform (always appended at the end)
	 * 
	 * \param waveform is a vector containing the actual waveform (should hae 512 entries)
	 * \return 0 if accepted and -1 if not 
	 */
	int addF0Wavetable(const std::vector<double>& waveform);
	/**
	 * method to get data of a given length (size of vector)
	 * 
	 * \param data vector as reference
	 * \return 
	 */
	int getData(std::vector<double>& data);
	void setSamplerate(double samplerate); /**< set the samplerate for the internal tablef0*/
	void setFrequency(double freq); /**< set the frequency for the generated data*/
	void setWaveform(int newwaveformnumber); /**< set the current generated waveform */
	void setWaveformXFade(double xfadefactor); /**< set a factor to mix two waveforms range 0...1*/
	void setSecondWaveform(int waveformnumber); /**< set the second waveform*/
	void resetPhase(); /**< reset the phase to start at the beginning of the internal waveform representation*/
	
	// additional functions for frequency control
	void changeFrequencyRelative(double deltaFreq);
	void setPortamentoTime(double newPortamento_ms);

private:
	double m_fs; ///< sample rate
	double m_freq; ///< frequency
	double m_f0Table; ///< the internal fundamental frequency of the table
	double m_fsmin;  ///< an assumed minimal sampling rate

	int m_nrOfWaveforms; ///< how many waveforms are available
	int m_nrOfWavetables; ///< the number of wavetables necessary for aliasing free generation
	int m_lenOfWavetable; ///< the internal length of the wavetable set to 512
	int m_curWaveform; ///< the current waveform (unfortunately no list available)
	int m_curWavetable; ///< the used wavetable for a given frequency
	int m_interpolationOrder; ///< for order of the used interpolation algorithm (1 for linear interpolation ) 
	/**
	 * memory for the wavetables 3 dimensions: first the waveforms, second the wavetables and thirs the actual data
	 */
	std::vector <std::vector<std::vector<double>>> m_wavetable;

	double m_incStep; ///< the internal stepsize to walk through the wavetabel to generate a given frequency
	double m_curPos; ///< the current position in the wavetable

	void computeFullWavetable(); ///< helper function to generate the wavetables with reduced harmonics
	spectrum m_fft; ///< a fft class to compute the wavetables
	std::vector<double> m_real;
	std::vector<double> m_imag;
	std::vector<double> m_newwave;
	int m_semitonespertable; ///< controls the number of necessary wavetables

	double m_waveformXFadeFactor; ///< the internal crossfade factor between two waveforms
	int m_secondWaveform;
	/**
	 * Internal helper function to set the wavetable
	 * 
	 * \param newwaveform : the new waveform to be copied in the internal memory
	 * \param nrWaveform : defines the waveform it belongs to
	 * \param nrWavetable : defines the wavetable it should go
	 */
	void copyNewWavetable(std::vector<double> newwaveform, int nrWaveform, int nrWavetable);
	void computeIncStep(double freq); ///< internal helper for the computation of the inc step
	int m_portamentoCounter;
	int m_portamentorTime_Samples;
	double m_portamentoFactor;
	double m_incStepRelativeFactor;
};

