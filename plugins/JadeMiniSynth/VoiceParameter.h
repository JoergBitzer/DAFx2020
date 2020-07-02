#pragma once

#include <JuceHeader.h>

const struct
{
	std::string ID = "VoiceGlobalTuneA0";
	std::string name = "Tuning (global)";
	std::string unitName = " Hz";
	float minValue = 400.f;
	float maxValue = 490.f;
	float defaultValue = 440.f;
}paramGlobalTuneA0;

const struct
{
	std::string ID = "SynthNrOfVoice";
	std::string name = "Number of Voices";
	std::string unitName = "";
	float minValue = 1.f;
	float maxValue = 32.f;
	float defaultValue = 8.f;
}paramGlobalNrOfVoices;

const struct
{
	std::string ID = "VoicePortanmentoTime";
	std::string name = "Portamento";
	std::string unitName = " ms";
	float minValue = log(0.1f);
	float maxValue = log(4000.f);
	float defaultValue = log(0.1f);
}paramGlobalPortamentoTime;


class VoiceParameter
{
public:
	// Lowpass

	int addParameter(std::vector < std::unique_ptr<RangedAudioParameter>>& paramVector);
};

