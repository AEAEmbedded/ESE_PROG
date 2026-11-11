# CMake Workshop - Live Demo Runbook

Draaiboek voor de docent. Alle commando's zijn copy-paste klaar, uitgaande van
`Workshops/CMake/Source/` als werkmap.

---

## Voorbereiding (avond ervoor / ochtend)

```bash
cd Source/BiggerLibraryCMakeV2
./copy_sources.sh                      # Vult stage1..5 met de bronbestanden

cd ../ChallengeSkeleton
cmake -S . -B build && cmake --build build   # Warmt CppUTest-download op
ctest --test-dir build --output-on-failure   # Check: 1/1 Passed
```

Terminal: groot font, werkmap `Source/`. Keynote op tweede scherm.

---

## Lesverloop

### Blok 1 - De pijn (10 min, `BiggerLibrary/`)

```bash
cd BiggerLibrary
g++ main.cpp sht45.cpp i2c.cpp logger.cpp config.cpp filter.cpp \
    -I./include -std=c++17 -Wall -O2 -o sensor_app
./sensor_app
```

- Laat het lange commando zien; "vergeet" daarna live één `.cpp` → linker error.
- Vraag: wie wil dit elke build typen? En je teamgenoot op Windows?

### Blok 2 - Make helpt... half (10 min, `BiggerLibraryMake/`)

```bash
cd ../BiggerLibraryMake
make && make run
```

- Open de Makefile: wijs de herhaalde regels aan (één rule per `.cpp`).
- "Voeg een bestand toe"-oefening: tel samen hoeveel plekken je moet aanpassen (3+).

### Blok 3 - CMake (10 min, `BiggerLibraryCMake/`)

```bash
cd ../BiggerLibraryCMake
cmake -S . -B build
cmake --build build
./build/sensor_app
```

- Open `CMakeLists.txt` naast de Makefile: 35 vs 62 regels, geen object files,
  geen dependency rules.
- Nieuw bestand toevoegen = één regel in `SOURCES`.

### Blok 4 - De klassieke fout (5 min, zelfde map)

Studenten lopen hier gegarandeerd tegenaan; laat hem bewust zien:

```bash
cd build && cmake -G Ninja ..        # → "does not match the generator..."
cd .. && rm -rf build                # De oplossing is altijd: weg ermee
```

Boodschap: `build/` is wegwerpmateriaal, je source blijft schoon. Daarom
out-of-source builds (en `build/` in `.gitignore`!).

### Blok 5 - Modern CMake stages (15 min, keynote + `BiggerLibraryCMakeV2/`)

Keynote-slides stage 1 t/m 6. Live bouwen hoeft alleen stage 4 (FetchContent,
de wow-factor: dependency wordt automatisch gedownload):

```bash
cd ../BiggerLibraryCMakeV2/stage4
cmake -S . -B build && cmake --build build
```

Stage 6 (STM32) alleen op slides.

### Blok 6 - Finale: brug naar de eindopdracht (15 min, `ChallengeSkeleton/`)

```bash
cd ../../ChallengeSkeleton
cmake -S . -B build && cmake --build build   # Snel: alles staat al gecached
./build/app/skeleton_app
ctest --test-dir build --output-on-failure
```

Punten om te maken:
- Structuur = wat de eindopdracht vraagt: core-logica als library zonder I/O
  (testbaar), app ernaast, tests via ctest.
- `EarlyWarningScore` is bewust NIET de echte Dutch PEWS, maar laat wél zien:
  thresholds uit configuratie, niet hardcoded (hardcoden = OCP-schending,
  staat letterlijk in de opdracht).
- `CMakePresets.json`: hele team dezelfde build-instellingen.
- Teaser: volgende workshop (Unit Testing) hangen we mocks en test doubles
  in precies deze structuur.

### Blok 7 - Zelf aan de slag (rest)

Opdracht aan de teams: kopieer `ChallengeSkeleton` als start van jullie
challenge-repo, hernoem project en targets, en zorg dat `ctest` bij iedereen
groen is vóór de volgende workshop. Oefeningen 1 t/m 4 uit de README voor wie
eerder klaar is.

---

## Reserve / FAQ

- CppUTest-download traag op HAN-wifi → laat je eigen voorgebouwde `build/` zien.
- `cpputest_realloc_location`-compile-error op macOS: CppUTest's
  leak-detectie-macro's botsen met libc++; in het skelet staat daarom
  `CPPUTEST_MEM_LEAK_DETECTION_DISABLED ON` (zie `tests/CMakeLists.txt`).
- Cache-errors na verplaatsen van een map: altijd `rm -rf build`, nooit in de
  cache gaan poken.
