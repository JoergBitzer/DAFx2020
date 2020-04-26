# DAFx2020 (SoeSe 2020)

Dies ist die korrespondierende Seite zum Kurs Digitale Audio Effekte / Digitale Signalverarbeitung an der [Jade Hochschule](https://www.jade-hs.de/) für die Studiengänge [Hörtechnik und Audiologie](https://www.jade-hs.de/tgm/studium/ha/) (DAFx) und [Assistive Technologien](https://www.jade-hs.de/tgm/studium/at/) (DSP)

## Ziele

* Rauschen und Wellenformen als Signalquellen
* Filterdesign und Implementierung im Kontext von Audio
* Kennenlernen von typischen Plug-In Schnittstellen (VST, AU)
* Aufbau von subtraktiven Synthesizern
* GUI Programmierung
* Probleme und Lösungen für "semi-professionelle" Plug-Ins (z.B. Vermeidung von Zipper-Noise, Parameterhandling, Presets, Portabilität)
* Finden und Umsetzen von kreativen Ideen. (Was kann man wie anders machen als alle anderen da draußen. Wann sollte man es genau so machen wie alle anderen.)

## Programmiersprachen und Bibliotheken

* C++
* [Juce](https://juce.com/) als Framework (Support von Plug-In Schnittstellen und Stand-Alone Entwicklungen von Audio-Software)
* [Eigen](http://eigen.tuxfamily.org/) als Matrix und Lineare Algebra Lösung (Benötigt für bestimmte Filterdesign Lösungen)

## Bisher entwickelte Werkzeuge

* Noise Generator (White Noise mit veränderlicher Amplitudenverteilung (gleich bis super-gaussian ==> von weich über rauh zu cracklig))
* LFO (Sin, Rect, Saw, S&H mit Slope-Time + viele neue Formen) (Einfach erweiterbar und erweitert durch Studierende)
* Envelope (Delay, Attack, Hold, Decay, Sustain, Release)
* Färbungsfilter (TP, HP) mit einstellbarer Ordnung (auch als fractional, z. B. 0.5 um rosa Rauschen zu erzeugen)
* Oszillatoren (Aliasing freie Wavetables mit beliebiger Wellenform der Länge 512)

## Synthesizer in JUCE

* TBD

## Plugins in JUCE

* GUI Basics (Slider, Combo, Button) in JUCE (nur im Editor die Hintergrundfarbe ändern mit diesen drei Elementen)
* Filtarbor (Filter with arbitrary order).  Ein einfaches Filter (Tief und Hochpass) mit frei wählbarer Ordnung, so dass zB rosa Rauschen erzeugt werden kann (order = 0.5 und Eingang weiß).  

## ToDo (nach Priorität sortiert)

* Synthesizer Handling in JUCE
* GUI (Look and Feel, eigene GUI Elemente, Zeichnen von Funktionen)
* Preset Handling
* VCF nachbauen (inspiriert von [Will Pirkle](https://www.willpirkle.com/))
* Formant Filter (mal in richtig)
* Effekte (Chorus, Delay, Distortion)
* Modulationsmatrix

Achtung: Work in Progress.
