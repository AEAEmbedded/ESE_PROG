# Zephyr Workshop - Live Demo Runbook (90 min)

Draaiboek voor de docent. Doelgroep: tweedejaars, géén RTOS-voorkennis.
Alles draait op QEMU (`qemu_cortex_m3`), niemand heeft hardware nodig.

---

## Voorbereiding

**Minimaal een week vooraf:** studenten de Prerequisites-sectie uit README.md
mailen. De setup is ~2 GB download; wie dat in de les moet doen, doet niet
mee. Benadruk: **pad zonder spaties** (`~/zephyrproject`), dus niet in
OneDrive/iCloud.

**Zelf (avond ervoor):**
```bash
cd ~/zephyrproject && source .venv/bin/activate
cp -r <repo>/Workshops/Zephyr/Source/0* apps/
for a in apps/0*; do (cd $a && west build -p -b qemu_cortex_m3); done
```
Terminal groot font. QEMU verlaten: **Ctrl-A, dan X** (schrijf op het bord,
iedereen vergeet het).

---

## Lesverloop

### Blok 1 - Waarom een RTOS? (10 min, whiteboard)

- Start bij wat ze kennen: Arduino `loop()`, de cyclic executive uit de
  patterns-workshop. Eén lus, alles om de beurt.
- Vraag: wat als taak A elke 100 ms móet en taak B 1 seconde duurt?
  Laat ze het zelf zien aankomen.
- Definieer: RTOS = scheduler + threads + prioriteiten + preemption.
  "Een chef die de belangrijkste taak altijd voorrang geeft."
- Medische context: een hartslagmeting die een seconde stilvalt is geen
  cosmetisch probleem.

### Blok 2 - Wat is Zephyr? (10 min)

- Linux Foundation, open source, vendor-neutraal; Nordic, ST, NXP, veel
  medical devices.
- De brug naar de CMake-workshop: `west build` = CMake + Ninja met
  superkrachten (zie de Zephyr-preview-slide uit dat deck).
- QEMU vandaag: zelfde code, geëmuleerde ARM Cortex-M3, geen flasher nodig.

### Blok 3 - 01_hello: de toolflow (15 min)

```bash
cd ~/zephyrproject/apps/01_hello
west build -b qemu_cortex_m3
west build -t run                  # Ctrl-A, X om te stoppen
```

- Laat de drie bestanden zien: `CMakeLists.txt` (4 regels! vergelijk met
  vorige workshop), `prj.conf` (leeg), `src/main.c`.
- Wijs in de build-output aan: CMake configure, Ninja build, één firmware-
  image met de hele RTOS én jouw app erin.
- Studenten bouwen mee. Wie vastloopt: venv actief? `west build -p`?

### Blok 4 - Het probleem en de oplossing (20 min) ← kern van de les

**02_superloop:** run en laat de output staan. De fast-tick valt ruim een
seconde stil zodra het zware werk draait (gaten van ~1100 ms). "Dit is jullie
Arduino-loop onder druk."

**03_threads:** zelfde twee taken, nu als threads met prioriteiten. Run:
de heartbeat tikt dwars door het zware werk heen. Laat de code zien:
`K_THREAD_DEFINE`, prioriteit = laag getal wint.

**Experiment (studenten zelf, 5 min):** wissel de prioriteiten om en run
opnieuw. Nu verhongert de heartbeat. Les: prioriteiten zijn een
ontwerpbeslissing, geen detail.

### Blok 5 - 04_sensor_queue: threads die samenwerken (10 min)

- Producer/consumer: thermometer-thread → message queue → monitor-thread
  met FEVER ALARM boven 38 °C (knipoog naar de eindopdracht).
- Kernboodschap: geen gedeelde variabelen, geen race conditions; alle data
  via de queue. Zo blijven delen van een systeem losgekoppeld.

### Blok 6 - Devicetree en Kconfig (15 min)

**05_devicetree:** run eerst, output verklaart zichzelf: console-adres en
fever-threshold komen uit de devicetree, niet uit de code.

- Twee vragen uit elkaar houden (bord): devicetree = "welke hardware, waar?";
  Kconfig = "welke software-features compileren we mee?"
- Open `app.overlay`: de `/zephyr,user`-node met `fever-threshold-mdeg`.
  Verander de waarde, rebuild, run: config aangepast zonder C-code te raken.
  "Herken je de band-tabellen uit de challenge? Zelfde Open-Closed-principe."
- Open `build/zephyr/zephyr.dts`: het samengevoegde resultaat, de waarheid.
- Kconfig live: `west build -t menuconfig`, zoek met `/` naar PRINTK.

### Blok 7 - AI/LLM-tips bij deze tooling (5-10 min)

Loop de vijf regels uit de README langs, met als demo-waardige punten:
1. Versie + board altijd in je prompt (Zephyr v4.2.0, qemu_cortex_m3).
2. Plak de volledige foutmelding; DT-fouten (`__device_dts_ord_17`) zijn
   cryptisch en juist daar is een LLM sterk in.
3. `CONFIG_`-symbolen checken in menuconfig: LLM's verzinnen plausibele
   symbolen die niet bestaan.
4. DT-beweringen checken tegen `build/zephyr/zephyr.dts`.
5. Vraag "waarom", niet alleen "fix het": je moet je code zelf kunnen
   uitleggen in het eindgesprek.

### Afronding - en de klapper: echt hardware (5 min demo)

Elke groep heeft een XIAO nRF52840 en een Pi Pico 2 W. Sluit af met
`06_blinky` live op de XIAO:

```bash
cd ~/zephyrproject/apps/06_blinky
west build -p -b xiao_ble/nrf52840
```

Dubbel-tik op de reset-knop → XIAO verschijnt als USB-drive → sleep
`build/zephyr/zephyr.uf2` erheen → rode LED knippert. Geen flasher, geen
drivers, dertig seconden.

- Zelfde workflow als QEMU, alleen een ander `-b` argument. Dát is de
  belofte van Zephyr en devicetree.
- Wijs op de Pico-2-W-twist in de README: de onboard LED hangt aan de
  wifi-chip, dus `led0` wordt daar via `boards/*.overlay` een externe LED
  op GP15. Board-verschil opgelost in data, nul regels C gewijzigd.
- **Groepsopdracht voor na de les:** blinky op beide boards aan de praat;
  op de XIAO de LED groen laten knipperen (led1, één woord wijzigen) en op
  de Pico de LED naar een andere pin verplaatsen door alléén de overlay te
  wijzigen. Plus: experimenten uit 03 afmaken; in 04 een derde thread
  toevoegen die het aantal alarmen telt.

---

## Reserve / FAQ

- **QEMU-klok loopt sneller dan echte tijd.** De ms-stempels kloppen
  onderling, maar 25 "virtuele" seconden kunnen in 6 echte seconden voorbij
  zijn. Geen bug; zo nodig benoemen.
- **Student zit vast in QEMU:** Ctrl-A, dan X.
- **`west: command not found`:** venv niet actief.
- **Rare build-fouten na wisselen van app:** `west build -p` (pristine), het
  Zephyr-equivalent van `rm -rf build` uit de vorige workshop.
- **`cmsis_core.h: No such file`:** `west update` niet (volledig) gedraaid.
- **Pad met spaties** (OneDrive!): workspace verplaatsen naar
  `~/zephyrproject`.
- Setup bij een student mislukt helemaal → laat meekijken bij de buurman;
  de concepten zijn het leerdoel, niet de installatie.
- **Pico flashen:** BOOTSEL ingedrukt houden tíjdens het insteken van de
  USB-kabel, dan `zephyr.uf2` naar de verschenen drive slepen. Geen drive?
  Kabel is waarschijnlijk charge-only.
- **XIAO flashen:** dubbel-tikken op reset moet vlot (binnen ~0,5 s),
  anders reboot hij gewoon. Drive verschijnt als "XIAO-SENSE" o.i.d.
- **Blinky bouwt niet voor de Pico zonder overlay:** klopt, de kale
  `rpi_pico2`-devicetree heeft geen `led0`-alias. De overlay in
  `boards/` levert die - dat is precies het lespunt.
