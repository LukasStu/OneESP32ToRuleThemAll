# Aktuelle Berechnung der Tagesenergie

Diese Beschreibung bezieht sich auf das Template
[wp_daily_energy_combined.yaml](../yaml/wp_daily_energy_combined.yaml), nicht auf
einen geplanten Umbau. Ziel ist, zunächst die **Tageszähler** und ihre
Berechnung zu verstehen; eine spätere Optimierung ist hier nicht beschrieben.
Das Template [wp_generic_combined.yaml](../yaml/wp_generic_combined.yaml)
kombiniert dagegen unter anderem **Gesamtzähler** und hat keinen
Mitternachtsoffset.

## Welche Zähler werden gelesen?

Bei der WPL 13 ACS Classic wird das Template für vier unabhängige Zähler
verwendet: elektrische Energie und Wärmeenergie, jeweils für Heizung und
Warmwasser. Die Anfragen gehen an den CAN-Teilnehmer `Manager`:
[wpl13acs_classic.yaml](../yaml/wpl13acs_classic.yaml).

Für jede Kombination fragt der ESP zwei CAN-Properties ab: eine mit dem Namen
`*_TAG_KWH` und eine mit dem Namen `*_TAG_WH`. Der Wh-Wert wird also ebenfalls
von der Wärmepumpe gelesen; nur der ESPHome-Sensor für diesen Wert ist
`internal` und wird nicht als eigene Entität angezeigt. Die beiden Werte
bilden den kWh- und den Wh-Anteil des Tageszählers.

Beispiel aus dem [CAN-Log](../esphome-web-42e025-logs.txt): Für die elektrische
Energie Heizung werden `EL_AUFNAHMELEISTUNG_HEIZ_TAG_WH` mit Rohwert 745 und
`EL_AUFNAHMELEISTUNG_HEIZ_TAG_KWH` mit Rohwert 7 beantwortet. Der veröffentlichte
Wert beträgt anschließend 7,06 kWh; in ihn fließt auch der gespeicherte Offset
ein.

## Einheiten und Auflösung

- **Tageszähler:** Der Code kombiniert `*_TAG_KWH` mit `*_TAG_WH / 1000`.
  Ohne Offset ergeben beispielsweise die Rohwerte 7 und 745 rechnerisch
  7,745 kWh. Der veröffentlichte Sensor ist standardmäßig mit zwei
  Nachkommastellen konfiguriert; dies allein belegt nicht die tatsächliche
  Messauflösung der Wärmepumpe. Nach Angabe zur Anlage beträgt sie 0,01 kWh
  (10 Wh).
- **Gesamtzähler:** Für den Wärmeertrag kombiniert
  [wp_generic_combined.yaml](../yaml/wp_generic_combined.yaml) `*_SUM_MWH` mit
  `*_SUM_KWH / 1000` und gibt MWh aus. Die Rohwerte 9 und 911 ergeben
  9,911 MWh beziehungsweise 9.911 kWh. Nach dieser Interpretation entspricht
  ein Schritt des kWh-Rohwerts 1 kWh, nicht 0,001 kWh. Die Gesamtzähler sind
  von den Tageszählern zu unterscheiden.

## Abfrage und Berechnung

1. Beim ESP-Start werden beide Properties in die Anfragewarteschlange gestellt.
2. Danach geschieht dies alle fünf Minuten, jeweils zur Minute 4, 9, 14 usw.
3. Der Wh-Wert wird mit 0,001 multipliziert und der gespeicherte Offset
   abgezogen.
4. Beim Empfang des kWh-Werts wird dieser mit dem internen Wert addiert und als
   Energiesensor veröffentlicht:

   ```text
   Sensorwert in kWh = TAG_KWH + TAG_WH / 1000 - Offset
   ```

Der Code summiert keine Differenzen zwischen aufeinanderfolgenden vollständigen
Zählerständen.

## Was passiert um Mitternacht?

Der ESP führt um 00:00 Uhr anhand seiner SNTP-Zeit für jeden Zähler aus:

1. Den zuletzt empfangenen **rohen Wert von `*_TAG_WH`** durch 1000 teilen und
   als Offset speichern. `*_TAG_KWH` wird dabei nicht berücksichtigt.
2. Den veröffentlichten Energiesensor auf `0.0` setzen.

Dabei wird keine CAN-Anfrage ausgelöst und kein Reset an die Wärmepumpe
gesendet. Der Offset wird erst beim nächsten Empfang von `*_TAG_WH` auf den
internen Sensor angewendet. Der Reset des WP-Tageszählers ist davon unabhängig.

Beispiel: Der letzte Rohwert von `*_TAG_WH` vor Mitternacht beträgt 780. Der ESP
speichert `0,780 kWh` als Offset. Liefert die Wärmepumpe nach ihrem Reset für
`*_TAG_KWH` den Wert 0 und für `*_TAG_WH` den Wert 20, ergibt die nächste
Berechnung `0 + 0,020 - 0,780 = -0,760 kWh`.
Ein solcher Rückgang wird aktuell nicht erkannt oder korrigiert.

## Persistenz und Grenzen

- Pro Zähler ist nur der Offset mit `restore_value: yes` zur Wiederherstellung
  vorgesehen. Vollständiger letzter Zählerstand, Messzeitpunkt und kumulierte
  Tagesenergie werden nicht persistent gespeichert.
- Nach einem Neustart müssen die beiden CAN-Werte erneut eintreffen; ein
  Vergleich mit einem gespeicherten vollständigen Messwert findet nicht statt.
- Die Implementierung prüft weder auf WP-Resets noch auf negative Ergebnisse.
  Sie kann Energie zwischen Messungen nicht exakt einem Kalendertag zuordnen.
- Die veröffentlichten Sensoren sind als `total_increasing` deklariert; das
  verhindert im ESP selbst weder negative Werte noch einen Rückgang.
