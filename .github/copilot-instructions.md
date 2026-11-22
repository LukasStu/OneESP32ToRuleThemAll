# Regelungsprinzip der Stiebel Eltron WPL 13 ACS Classic mit WPM 3

## 1. Überblick

Die Heizungsregelung besteht aus drei zusammenwirkenden Regelmechanismen:

1. Heizkurve: Bestimmt die benötigte Heiztemperatur abhängig von der Außentemperatur.
2. Raumeinfluss: Korrigiert den Temperatursollwert anhand der tatsächlichen Raumtemperatur.
3. Spreizungsregelung: Passt den Volumenstrom an, um die eingestellte Temperaturdifferenz zwischen WP-Vorlauf und WP-Rücklauf einzuhalten.

Die ersten beiden Mechanismen bestimmen den Temperaturbedarf. Die Spreizungsregelung beeinflusst über den Volumenstrom die Temperaturdifferenz zwischen Vorlauf und Rücklauf.

## 2. Heizkurve – Bestimmung des Temperaturbedarfs

Die Heizkurve beschreibt den Zusammenhang zwischen Außentemperatur und benötigter Heiztemperatur.

Je niedriger die Außentemperatur, desto höher muss die Heiztemperatur sein, um die Wärmeverluste des Gebäudes auszugleichen.

Bei der WPM-3-Regelung für den Heizkreis 1 wird die Heizkurve auf Basis der Rücklauftemperatur berechnet.

Die Heizkurve wird durch zwei wesentliche Parameter beeinflusst:

- Heizkurvensteigung: Bestimmt, wie stark die Heiztemperatur bei sinkender Außentemperatur ansteigt.
- Raum-Solltemperatur (Komforttemperatur): Verschiebt die Heizkurve parallel nach oben oder unten.

Eine höhere Komforttemperatur führt somit zu einer höheren Heizanforderung.

## 3. Raumeinfluss – Korrektur anhand der Raumtemperatur

Der Raumeinfluss ergänzt die witterungsgeführte Regelung um die tatsächlich gemessene Raumtemperatur.

Dafür benötigt der WPM 3 eine angeschlossene Fernbedienung FE7 mit integriertem Raumtemperaturfühler.

Die Regelung berücksichtigt die Abweichung zwischen Raum-Solltemperatur und Raum-Isttemperatur.

Die Berechnung erfolgt nach folgender Formel:

[
\Delta T_R =
(T_{Raum,Soll}-T_{Raum,Ist})\cdot S\cdot K
]

Dabei gilt:

- (T_{Raum,Soll}): Gewünschte Raumtemperatur
- (T_{Raum,Ist}): Gemessene Raumtemperatur
- (S): Heizkurvensteigung
- (K): Eingestellter Raumeinfluss

Beispiel

- Raum-Solltemperatur: 22,5 °C
- Raum-Isttemperatur: 21,5 °C
- Heizkurvensteigung: 0,6
- Raumeinfluss: 5

[
\Delta T_R = (22,5-21,5)\cdot0,6\cdot5=3,K
]

Der errechnete Temperatursollwert wird um 3 K erhöht.

Bei einer zu hohen Raumtemperatur erfolgt die entsprechende Korrektur nach unten.

Der Raumeinfluss verändert somit den Temperatur-Sollwert der Heizungsregelung und nicht unmittelbar den Volumenstrom.

Besonderheit bei Fußbodenheizungen:

Fußbodenheizungen reagieren aufgrund ihrer thermischen Masse sehr träge. Ein zu hoher Raumeinfluss kann deshalb zu starken Sollwertkorrekturen führen, bevor die Auswirkungen im Raum tatsächlich messbar werden.

## 4. Spreizungsregelung – Anpassung des Volumenstroms

Die Spreizungsregelung verfolgt ein anderes Ziel: Sie soll eine definierte Temperaturdifferenz zwischen WP-Vorlauf und WP-Rücklauf erreichen.

Bei unserer Anlage ist eine Sollspreizung von 4 K eingestellt.

Die Berechnung lautet:

[
\Delta T = T_{WP,Vorlauf}-T_{WP,Rücklauf}
]

Die Regelung beeinflusst diese Temperaturdifferenz über die Umwälzpumpe und den Volumenstrom.

Dabei gilt grundsätzlich:

- Höherer Volumenstrom führt zu einer kleineren Spreizung.
- Niedrigerer Volumenstrom führt zu einer größeren Spreizung.

Beispiel

Messgröße| Temperatur
WP-Vorlauf| 33 °C
WP-Rücklauf| 29 °C
Spreizung| 4 K

Die Wärmepumpe passt den Volumenstrom so an, dass die eingestellte Sollspreizung möglichst eingehalten wird.

Die Spreizungsregelung verändert dabei nicht direkt die Heizkurve, sondern beeinflusst die Temperaturverteilung zwischen Vorlauf und Rücklauf.

## 5. Zusammenspiel der drei Regelmechanismen

Die drei Regelmechanismen erfüllen unterschiedliche Aufgaben:

Regelmechanismus| Aufgabe
Heizkurve| Ermittelt den benötigten Temperatur-Sollwert anhand der Außentemperatur
Raumeinfluss| Korrigiert den Sollwert anhand der tatsächlichen Raumtemperatur
Spreizungsregelung| Passt den Volumenstrom an, um die gewünschte Temperaturdifferenz einzuhalten

Vereinfachtes Rechenbeispiel

Angenommen, die Heizkurve und der Raumeinfluss ergeben einen WP-Rücklauf-Sollwert von 29 °C.

Bei einer Sollspreizung von 4 K ergibt sich:

[
T_{WP,Vorlauf}=29+4=33,°C
]

Damit ergeben sich folgende Zielgrößen:

- WP-Rücklauf: 29 °C
- WP-Vorlauf: 33 °C
- Sollspreizung: 4 K

Die Heizkurve und der Raumeinfluss bestimmen den Temperaturbedarf. Die Spreizungsregelung passt den Volumenstrom an, damit die gewünschte Temperaturdifferenz erreicht wird.

Das Beispiel beschreibt den vereinfachten Regelzusammenhang und keine Garantie für die tatsächlichen Istwerte zu jedem Zeitpunkt.

## 6. Bedeutung für die Home-Assistant-Auswertung

Für die Integration in OneESP32ToRuleThemAll und die spätere Auswertung in Home Assistant sollten folgende Messgrößen getrennt erfasst werden:

Messgröße| Verwendung
Außentemperatur| Eingang für die Heizkurve
Raumtemperatur Soll| Eingang für den Raumeinfluss
Raumtemperatur Ist| Eingang für den Raumeinfluss
WP-Vorlauf Ist| Tatsächliche Vorlauftemperatur
WP-Rücklauf Ist| Tatsächliche Rücklauftemperatur
HK1 Ist| Rücklauftemperatur des Heizkreises
Volumenstrom| Kontrolle der Spreizungsregelung
Sollspreizung| Zielwert der Spreizungsregelung

Zwei unterschiedliche Spreizungen

WP-Spreizung:

[
\Delta T_{WP}=T_{WP,Vorlauf}-T_{WP,Rücklauf}
]

Diese Größe dient zur Kontrolle der eigentlichen Spreizungsregelung.

Heizkreis-Spreizung:

[
\Delta T_{HK1}=T_{WP,Vorlauf}-T_{HK1,Ist}
]

Diese Größe dient zur Beobachtung der Wärmeabgabe an die Fußbodenheizung.

Die beiden Rücklauftemperaturen sind nicht identisch und dürfen nicht ohne Weiteres gleichgesetzt werden.

Insbesondere während der Warmwasserbereitung kann die WP-Rücklauftemperatur deutlich von der Rücklauftemperatur des Heizkreises abweichen. Deshalb sind Spreizungsanalysen auf den regulären Heizbetrieb zu beschränken.

## 7. Zusammenfassung

Das Regelungskonzept lässt sich auf drei wesentliche Aufgaben reduzieren:

1. Temperaturbedarf bestimmen: Die Heizkurve legt abhängig von der Außentemperatur die benötigte Heiztemperatur fest.
2. Temperaturbedarf korrigieren: Der Raumeinfluss berücksichtigt die tatsächliche Raumtemperatur und korrigiert den Sollwert.
3. Temperaturdifferenz einstellen: Die Spreizungsregelung passt den Volumenstrom an, um die eingestellte Spreizung von 4 K zwischen WP-Vorlauf und WP-Rücklauf einzuhalten.

Für die Optimierung der Heizungsanlage sollten die Parameter getrennt betrachtet werden:

- Heizkurve anhand der Raumtemperatur und des Wärmebedarfs optimieren.
- Raumeinfluss zunächst zurückhaltend einstellen.
- Spreizungsregelung und Volumenstrom unabhängig davon überwachen.

Wichtig: Die Temperatur-Sollwertbildung und die Volumenstromregelung sind unterschiedliche Regelaufgaben. Eine Änderung der Heizkurve ist daher nicht mit einer Änderung der Sollspreizung gleichzusetzen.

## 8. Herstellerdokumentation

Stiebel Eltron – Bedienungs- und Installationsanleitung Wärmepumpen-Manager WPM 3.

Stiebel Eltron – Installationsanleitung WPL ACS Classic.

Die konkreten CAN-Property-Adressen der beschriebenen Messgrößen müssen separat anhand der Anlagenkommunikation verifiziert werden.
