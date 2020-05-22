// LFOTester.cpp : This file contains the 'main' function. Program execution begins and ends there.
//
#include <iostream>
#include <fstream>
#include <vector>
#include "LFO.h"

int main()
{
	std::vector <double> data;
	data.resize(30000);

	LFO lfo;
	lfo.setLFOFunction(LFO::LFOFunctions::sinus);
	lfo.setSamplerate(44100.0);
	lfo.setFrequency(20.0);
	lfo.setMin(-2.0);
	lfo.setMax(2.0);
	lfo.setFormFactor(0.001);
	lfo.setPulseWidth(10.0);
	lfo.setStartphase(-3.1414/2.0);
	//lfo.invertForm();
	lfo.setDelay(-50.0);
	lfo.setMaxCycleTime(100.0);
	lfo.setSyncedPhase(true);
	lfo.setSmoothing(true);
	lfo.setEmptyCyclePos(LFO::EmptyCyclePos::zero);

	std::ofstream out("data.txt");

	lfo.getData(data);

	for (auto kk = 0u; kk < data.size(); ++kk)
	{
		out << data[kk] << " ";
	}
	
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
