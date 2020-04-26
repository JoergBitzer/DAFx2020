#pragma once

#include <string>

// Lowpass
const struct
{
	std::string ID = "lpcutoff";
	std::string name = "LowpassCutoff";
	std::string unitName = "Hz";
	float minValue = log(100.f);
	float maxValue = log(15000.f);
	float defaultValue = log(1000.f);
}paramLpCutoff;

const struct
{
	std::string ID = "lporder";
	std::string name = "LowpassOrder";
	std::string unitName = "";
	float minValue = 0.0f;
	float maxValue = 2.3f;
	float defaultValue = 0.5f;
}paramLpOrder;

// Highpass
const struct
{
	std::string ID = "hpcutoff";
	std::string name = "HighpassCutoff";
	std::string unitName = "Hz";
	float minValue = log(100.f);
	float maxValue = log(15000.f);
	float defaultValue = log(1000.f);
}paramHpCutoff;

const struct
{
	std::string ID = "hporder";
	std::string name = "HighpassOrder";
	std::string unitName = "";
	float minValue = 0.0f;
	float maxValue = 2.3f;
	float defaultValue = 0.5f;
}paramHpOrder;