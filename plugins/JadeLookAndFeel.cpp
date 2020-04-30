#include "JadeLookAndFeel.h"

JadeLookAndFeel::JadeLookAndFeel()
{
	setColour(ResizableWindow::backgroundColourId, JadeWhite);

	// define Slider
	setColour(Slider::backgroundColourId, JadeGray);
	setColour(Slider::thumbColourId, JadeRed);
	setColour(Slider::trackColourId, JadeGray);
	//setColour(Slider::textBoxBackgroundColourId, JadeGray);
	setColour(Slider::textBoxTextColourId, JadeGray);
	setColour(Label::textColourId, JadeGray);
	setColour(Slider::ColourIds::textBoxTextColourId, JadeGray);
	setColour(TextEditor::ColourIds::textColourId, JadeGray);
	
	// define TextButton
	setColour(TextButton::ColourIds::buttonColourId, JadeGray);
	setColour(TextButton::ColourIds::textColourOnId, JadeWhite);
	setColour(TextButton::ColourIds::buttonOnColourId, Colours::darkgrey);
	setColour(TextButton::ColourIds::textColourOffId, JadeLightRed1);

//	setColour(TextButton::ColourIds::textColourOnId, JadeRed);
	setDefaultLookAndFeel(this);
}
