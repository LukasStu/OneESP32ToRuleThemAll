# Temperatur- und Spreizungsregelung der Wärmepumpe

Diese Beschreibung bezieht sich auf die Stiebel Eltron WPL 13 ACS Classic
mit WPM 3. Sie trennt die Bildung des Temperatur-Sollwerts von der Regelung
der Temperaturdifferenz zwischen Wärmepumpen-Vorlauf und -Rücklauf.

## Temperaturregelung im normalen Heizbetrieb

Im normalen Heizbetrieb berechnet der WPM 3 anhand der Außentemperatur, der
Heizkurvensteigung und der Raum-Solltemperatur einen Sollwert für den
Wärmepumpen-Rücklauf im Heizkreis 1. Der Raumeinfluss korrigiert diesen
Sollwert zusätzlich anhand der Abweichung zwischen Raum-Soll- und
Raum-Isttemperatur.

## Festwertbetrieb

Im Festwertbetrieb wird die Heizkurve durch einen festen
Rücklauf-Sollwert ersetzt. Damit ist der Rücklauf-Sollwert vorgegeben,
anstatt anhand der Außentemperatur über die Heizkurve berechnet zu werden.

## Spreizungsregelung

Unabhängig von der Berechnung des Rücklauf-Sollwerts regelt der WPM 3 die
Temperaturdifferenz zwischen WP-Vorlauf und WP-Rücklauf auf die eingestellte
Sollspreizung. Bei dieser Anlage beträgt sie 4 K:

```text
WP-Spreizung = WP-Vorlauf - WP-Rücklauf
```

Dazu passt der WPM 3 den Volumenstrom der Umwälzpumpe automatisch an. Die
Vorlauftemperatur ergibt sich somit aus dem Zusammenspiel von Rücklauftemperatur
und Spreizung; sie wird nicht unmittelbar durch die Heizkurve vorgegeben.

## Zusammenspiel und Einordnung

Die Heizkurve (oder im Festwertbetrieb der feste Rücklauf-Sollwert) bestimmt
den Rücklauf-Sollwert. Der Raumeinfluss korrigiert diesen Sollwert im normalen
Heizbetrieb. Die Spreizungsregelung verfolgt eine getrennte Aufgabe: Sie passt
den Volumenstrom an, um die Sollspreizung zwischen WP-Vorlauf und WP-Rücklauf
einzuhalten.

Die Herstellerunterlagen beschreiben Temperatur-Sollwertbildung und
Spreizungsregelung getrennt. Dass die Spreizungsregelung auch im Festwertbetrieb
aktiv bleibt, ist technisch plausibel, wird aber nicht ausdrücklich als
gemeinsamer Betriebszustand dokumentiert.
