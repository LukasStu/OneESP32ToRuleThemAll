# ESPHome – 24h Logging

## Aufzeichnung starten

Im Repository-Root `OneESP32ToRuleThemAll` in einer Linux-Shell ausführen:

```bash
i=1
while [ -e "./wp_log_$i.txt" ]; do
  i=$((i+1))
done
nohup timeout 86400 esphome logs /config/esphome/esphome-web-42e010.yaml > "./wp_log_$i.txt" 2>&1 < /dev/null &
pid=$!
echo "PID: $pid; Logdatei: wp_log_$i.txt"
```

Den gesamten Befehlsblock einschließlich der Zeilenumbrüche kopieren und
einfügen. Leere Zeilen zwischen Befehlen wären in der Shell unproblematisch.

- Laufzeit: 24 Stunden
- Ausgabedatei: `wp_log_1.txt`, `wp_log_2.txt` usw.
- Speicherort: Repository-Root
- Vorhandene Logs werden nicht überschrieben.
- Ausführung erfolgt im Hintergrund.
- Die ausgegebene Prozess-ID zum vorzeitigen Beenden notieren.

## Log kontrollieren

Dateien anzeigen:

```bash
ls -lh wp_log_*.txt
```

Letzte 20 Zeilen anzeigen:

```bash
tail -n 20 wp_log_1.txt
```

Live-Ausgabe verfolgen:

```bash
tail -f wp_log_1.txt
```

Mit `Strg+C` wird nur die Live-Anzeige beendet, nicht die Aufzeichnung.
Bei Bedarf `wp_log_1.txt` durch den Namen der aktuellen Logdatei ersetzen.

## Logging beenden

In derselben Shell wie beim Start ausführen:

```bash
kill "$pid"
```

Die Variable `pid` gilt nur in dieser Shell. Nach einem Shell-Neustart die
notierte Prozess-ID einsetzen; beispielsweise beendet `kill 12345` den
Prozess mit der ID 12345.

## Hinweise

- VS-Code-Server muss während der Aufzeichnung aktiv bleiben.
- ESPHome-Konfiguration wird unter `/config/esphome/` geladen.
- Logdateien liegen im Git-Repository und können committet und gepusht werden.
- Bei jedem neuen Start wird automatisch die nächste freie Dateinummer verwendet.
