# DAFx2020 (SoeSe 2020)
Dies ist die korrespondierende Seite zum Kurs Digitale Audio Effekte / Digitale Signalverarbeitung an der Jade Hochschule für die Studiengänge Hörtechnik und Audiologie (DAFx) und Assistive Technologien (DSP)

## Ziele
* Rauschen und Wellenformen als Signalquellen 
* Filterdesign und Implementierung im Kontext von Audio
* Kennenlernen von typischen Plug-In Schnittstellen (VST, AU)
* Aufbau von subtraktiven Synthesizern 
* GUI Programmierung 
* Probleme und Lösungen für "semi-professionelle" Plug-Ins (z.B. Vermeidung von Zipper-Noise, Parameterhandling, Presets, Portabilität)
* Finden und Umsetzen von kreativen Ideen. (Was kann man wie anders machen als alle anderen da draußen und wann sollte man es genau so machen wie alle anderen.) 

## Programmiersprachen und Bibliotheken
* C++
* Juce als Framework (Support von Plug-In Schnittstellen und Stand-Alone Entwicklungen von Audio-Software)
* Eigen als Matrix und Lineare Algebra Lösung (Benötigt für bestimmte Filterdesign Lösungen)

## Bisher entwickelte Werkzeuge
* Noise Generator (White Noise mit veränderlicher Amplitudenverteilung (gleich bis super-gaussian ==> von weich über rauh zu cracklig))
* LFO (Sin, Rect, Saw, S&H mit Slope-Time) (Einfach erweiterbar)
* Envelope (Delay, Attack, Hold, Decay, Sustain, Release)
* Färbungsfilter (TP, HP) mit einstellbarer Ordnung (auch als fractional, z. B. 0.5 um rosa Rauschen zu erzeugen)

## ToDo (nach Priorität sortiert)
* Oszillatoren
* GUI (Look and Feel, eigene GUI Elemente)
* VCF nachbauten (inspiriert von Will Pirkle)
* Formant Filter (mal in richtig)
* Effekte (Chorus, Delay, Distotion)
* Modulationsmatrix

Achtung: Work in Progress. 
