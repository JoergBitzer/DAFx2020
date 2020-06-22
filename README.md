# DAFx2020 (SoSe 2020)

Dies ist die korrespondierende Seite zum Kurs Digitale Audio Effekte / Digitale Signalverarbeitung an der [Jade Hochschule](https://www.jade-hs.de/) für die Studiengänge [Hörtechnik und Audiologie](https://www.jade-hs.de/tgm/studium/ha/) (DAFx) und [Assistive Technologien](https://www.jade-hs.de/tgm/studium/at/) (DSP)

## Ziele

* Rauschen und Wellenformen als Signalquellen
* Filterdesign und Implementierung im Kontext von Audio
* Kennenlernen von typischen Plug-In Schnittstellen (VST, AU)
* Aufbau von subtraktiven Synthesizern
* GUI Programmierung
* Probleme und Lösungen für "semi-professionelle" Plug-Ins (z.B. Vermeidung von Zipper-Noise, Parameterhandling, Presets, Portabilität)
* Finden und Umsetzen von kreativen Ideen. (Was kann man wie anders machen als alle Anderen da draußen. Wann sollte man es genau so machen, wie die restliche Welt.)

## Programmiersprachen und Bibliotheken

* C++
* [Juce](https://juce.com/) als Framework (Support von Plug-In Schnittstellen und Stand-Alone Entwicklungen von Audio-Software)
* [Eigen](http://eigen.tuxfamily.org/) als Matrix und Lineare Algebra Lösung (Benötigt für bestimmte Filterdesign Lösungen)

## Bisher entwickelte Werkzeuge (Audio Processing unabhängig von JUCE)

* Noise Generator (White Noise mit veränderlicher Amplitudenverteilung (gleich bis super-gaussian ==> von weich über rauh zu cracklig))
* LFO (Sin, Rect, Saw, Tri und S&H mit Slope-Time + viele neue Formen) (Einfach erweiterbar und erweitert durch Studierende)
* LFO (Sin, Tri, Saw) mit vielen Parametern (PulseWidth, CycleLength, Phase, Smoothing, Delay, FormMorphing)
* Envelope (Delay, Attack, Hold, Decay, Sustain, Release)
* Färbungsfilter (TP, HP) mit einstellbarer Ordnung (auch als fractional, z. B. 0.5 um rosa Rauschen zu erzeugen)
* Oszillatoren (Aliasing freie Wavetables mit beliebiger Wellenform der Länge 512), pitchbending moeglich, glide/portamento moeglich
* Moog Ladder filter (inspiriert von [Will Pirkle](https://www.willpirkle.com/) ohne Nicht-Linearität.
* First order filter design and filter routine (everything in one header)
* Helper Header for lowpass->lowpass / lowpass->highpass transforms and bilinear transform for first and second order filter.

## Synthesizer in JUCE

* nano (1 Oscillator, 1 Envelope, 1 Filter (noch ohne Modulation)): Demonstriert das Stimmenhandling von JUCE in der Synthesizerklasse. Grundgerüst für weitere Synth. Bisher nur 2 Parameter (cutoff und Resonance mit Slidern (Erläuterung Konzept Parameter)) 
* JadeMiniSynth (1 Oszi (Ziel 2), 1 Envelope (Ziel 2), 1 Filter, (Ziel 2 LFOs), (Ziel 1 NoiseGen)): Ziel ist ein klassischer subtraktiver Synth mit ein paar kleineren Eigenheiten als Basis für Feature Verbesserungen. Alle Elemente sind sehr eigenständige Module (leichte Vervielfältigung und Wartung) mit dazugehöriger GUI.

## Effekt Plugins in JUCE

* GUI Basics (Slider, Combo, Button) in JUCE (nur im Editor die Hintergrundfarbe ändern mit diesen drei Elementen)
* Filtarbor (Filter with arbitrary order).  Ein einfaches Filter (Tief und Hochpass) mit frei wählbarer Ordnung, so dass zB rosa Rauschen erzeugt werden kann (order = 0.5 und Eingang weiß).  
* Filtarbor mit GUI im Jade Stil. Nutzung von LookAndFeel ![FiltarborMitGUI](/img/FiltarborJadeGui.jpg)
* JadeChorus: EIn klassischer Chorus Effekt, der auch den warmen Sound des Juno-Chorus kreieren kann.

## Concepts (HowTo in JUCE)

* How to use the MidiKeyboardComponent
* How to build your own components (display and mouse input)
* LevelMeter (GUI), Konzept des regelmäßigen GUI updates)
* GUI (Einbinden von Images)
* PresetHandler

## Utilities (kleine Helfer)

* DebugAudioWriter: Schreibt die letzten 10 Sekunden als File in C:\Temp. Filename ist die Zeit seit 1970 in ms (Es ging nur darum einen einzigartigen File-Namen zu haben). Zusätzlich lässt sich das letzte Sample eines Blockes auf 1 setzen. Dies hilft bei der Fehlersuche, wenn der Fehler an den Blockgrenzen vermutet wird. Die Idee ist, jederzeit beim herumspielen und seltsamen Klangergebnissen, diese schnell speichern zu können für eine spätere Analyse. Der Source Code ist nicht sehr schön, aber es zeigt wie man ein FiFo Buffer mit einer Queue lösen kann.

## ToDo (nach Priorität sortiert)

* Mini (erster echter klassischer Synth (2 Osc + Noise -> Filter + 2 ADSR + 1-2 LFO + Chorus)) (In Arbeit)
* Modulationsmatrix
* LFO (Random LFO mit hoher Parametrisierbarkeit)
* Effekte (Delay)
* Formant Filter (mal in richtig)
* Oversamping für Wave-shaper (Distortion)

Achtung: Work in Progress.

### issues

* Filterübergänge mit den Wechsel pro Block zu sprunghaft.==> Entweder AUdioblock in kleine Blöcke zerlegen (feste interne updaterate zb 100Hz)  oder Audiosmoothing im Filter (zwei Filter und blenden)
* oscxFade (und alles was über die Voices stabil sein muss) müssen parameter sein und in Sound gespeichert werden, da sonst die Stimmen bei gleichem Anschlag unterschiedlich klingen
* Envelope knackt (bei schnellen Einstellungen 1ms Attackzeit) (Über Smoothing mit Besselfilter nachdenken (siehe LFO), beim Osc nachsehen, ob im Nulldurchgang  oder im Maximum gestartet wird (auch überlegen, ob osc nicht mit freier Phase analoger ist))

