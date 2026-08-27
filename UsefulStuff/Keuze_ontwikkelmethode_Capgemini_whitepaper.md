# Keuze ontwikkelmethode

> *Application Services the way we see it*
>
> *in collaboration with —* **Insert partner logo**

---

## Auteurs

**Jan Klabbers** — een ervaren requirementsspecialist en softwareontwikkelproces-engineer bij Capgemini, met de meeste ervaring in maatwerksoftwareontwikkeling. Actief voor de overheid, het openbaar vervoer, de gezondheidszorg, productiebedrijven en verzekeraars. Binnen Capgemini is hij trekker van de requirements‑specialisten community om te komen tot een verder gestandaardiseerde werkwijze.

**Erik Hoolt** — projectmanager bij Capgemini met meer dan tien jaar ervaring in projectmanagement en meer dan vijftien jaar in maatwerksoftwareontwikkeling. Hij heeft vele projecten uitgevoerd met verschillende ontwikkelaanpakken, zoals Waterval, RUP en Scrum. Actief voor klanten in de financiële sector, de overheid en de private sector. Offshore‑samenwerking (zoals in India) is een van zijn specialiteiten en hij werkt continu aan het optimaliseren van de manier van werken binnen projecten.

---

## Voorwoord

De directe aanleiding voor het schrijven van de eerste versie van deze whitepaper was een klantvraag: geef een beknopte richtlijn voor het kiezen van een goede ontwikkelmethode. En natuurlijk liever morgen dan volgende week.

Dit artikel is daarmee vooral de weerslag van de praktijkervaring van twee Capgemini‑consultants met ruime ervaring in maatwerk softwareprojecten. Deze mening is getoetst door collega’s waardoor nog meer praktijkervaring en theoretische kennis is ingebracht in de huidige versie. En gezien de reacties is het wel een onderwerp dat leeft.

Dit document is de weerslag van discussie, die ons een handleiding geeft in de beslissing welke ontwikkelmethode te hanteren op een project.

Een belangrijke observatie die aan deze whitepaper ten grondslag ligt, is dat bedrijven vaak al te gemakkelijk één ontwikkelmethode omarmen en deze vervolgens geforceerd op al hun projecten willen toepassen. Vaak met de nodige problemen en weerstand in de organisatie. Ons uitgangspunt — gevoed door Capgemini’s Technovision 2011 — is dat elke ontwikkelmethode zijn sterke en zwakke punten heeft en dat er factoren zijn die een methode meer of minder geschikt maken voor een project.

Het inschatten van de genoemde criteria vergt de nodige praktijkkennis. Pas bij doorvragen op bepaalde onderwerpen zal de benodigde informatie boven tafel komen. Deze whitepaper en bijbehorende tool zijn de basis waarop deskundigen gezamenlijk met de klant kunnen onderzoeken welke methode het beste past.

---

## Inhoud

1. [Managementsamenvatting](#managementsamenvatting)
2. [Inleiding](#inleiding)
3. [Ontwikkelmethoden](#ontwikkelmethoden)
4. [Aspecten en criteria](#aspecten-en-criteria)
   - [Opdrachtgevereigenschappen](#opdrachtgevereigenschappen)
   - [Opdrachtnemereigenschappen](#opdrachtnemereigenschappen)
   - [Contract](#contract)
   - [Productaspecten](#productaspecten)

---

## Managementsamenvatting

Het kiezen van een ontwikkelmethode passend bij opdrachtgever, opdrachtnemer, contractvorm en het type product dat gemaakt moet worden, is een complexe keuze. Op basis van ervaringen uit de praktijk geeft deze whitepaper een handleiding die helpt bij het maken van een keuze.

We onderscheiden drie hoofdgroepen met elk hun methodische kenmerken:

- **Lineair** — geen herhaling van processtappen.
- **Iteratief** — vooraf gedefinieerde set van herhalingen.
- **Agile** — vooraf gelimiteerde set van korte herhalingen.

Hybride vormen zijn ook mogelijk; stel je een schaal voor met links *lineair*, in het midden *iteratief* en rechts *agile*. Elk project is ergens op deze schaal te plotten.

**Beoordeling per aspect**

Afhankelijk van de eigenschappen van opdrachtgever, opdrachtnemer, contract en product wordt een advies gegeven over de toepasbaarheid van elke groep ontwikkelmethoden:

- **A** — is een voorwaarde voor een succesvol project.
- **B** — verhoogt de kans op een succesvol project.
- **C** — geen invloed op het succes.
- **D** — beperkte maatregelen noodzakelijk om de kans op succes op peil te houden.
- **E** — uitgebreide maatregelen noodzakelijk om de kans op succes op peil te houden.

**Voorbeeld** — lage beschikbaarheid van stakeholders (bijv. de product owner in Scrum):

| Methode  | Score | Betekenis |
|---|---|---|
| Lineair  | D | Beperkte maatregelen nodig om het project op peil te houden. |
| Iteratief| D | Beperkte maatregelen nodig om het project op peil te houden. |
| Agile    | E | Uitgebreide maatregelen noodzakelijk om het project op peil te houden. |

Bij lage beschikbaarheid moet dit risico gemanaged worden; voor een agile aanpak zijn vaak bewuste maatregelen nodig (bijv. een hybride vorm tussen agile en iteratief). Uiteraard moeten alle criteria in ogenschouw worden genomen.

> Een geschikte aanpak scoort veel **A/B/C**, beperkt **D**, en bij voorkeur geen **E**. Eigenschappen met **D/E** moeten als risico worden gemanaged; **E**‑scores hebben vaak grote impact en hoge kans, en leiden vaak tot een hybride aanpak.

---

## Inleiding

In de wereld van maatwerksoftware is een groot aantal ontwikkelmethoden beschikbaar (LAD, IAD, DSDM, RUP, Scrum, Smart, etc.). De keuze wordt vaak beïnvloed door voorkeuren van klant, leverancier, projectleider of mode in de bedrijfstak.

In dit document verzamelen we de best practices binnen Capgemini om tot een meer gefundeerde keuze te komen op basis van eigenschappen van:

- **opdrachtgevende organisatie**
- **opdrachtnemer**
- **contractvorm**
- **op te leveren product**

Op basis van deze criteria is een tool gemaakt die grafisch de geschiktheid van methoden weergeeft.

> *Voorbeeldgrafieken (niet opgenomen in deze Markdown).*  
> *Area scores per method type* en *Total score per method type*.

**Opbouw van dit document:**

1. Definitie van de drie groepen (lineair, iteratief, agile).
2. Criteria voor het toepassen van een ontwikkelmethode.

---

## Ontwikkelmethoden

Er bestaan veel verschillende ontwikkelmethoden. We onderscheiden drie hoofdsoorten: **lineair**, **iteratief** en **agile**. Het onderscheid tussen iteratief en agile is belangrijk, omdat de succesfactoren verschillen. Hybride vormen zijn mogelijk; projecten kunnen op de schaal *lineair—iteratief—agile* schuiven afhankelijk van omstandigheden.

**Voorbeeld:** In een agile project blijkt dat de product owner niet het beslismandaat heeft (bijv. door de bedrijfscultuur). Dan kan een wekelijks stakeholderberaad ingericht worden om beslissingen te nemen. Dit beperkt de agility en schuift het project richting iteratief.

### Kenmerken per hoofdsoort

**Lineair**

- Geen herhaling van stappen in het ontwikkelproces.
- Vast gedefinieerd eindproduct per fase (deliverable‑gedreven).
- Gefixeerde requirements; documentatie vooraf.
- Document‑gedreven kennisoverdracht.
- Hoge klantdeelname vooraf/in eerste fase; later review/goedkeuring.
- Beperkte aandacht voor uitsluiten technologierisico’s.
- ‘Volgens specificatie’ aanpak.
- Door manager aangestuurde teams; vaste rollen met eigen producten.

**Iteratief**

- In de eerste fase: set iteraties en inhoud bepalen; evaluatie en bijsturing per iteratie.
- Vroeg in het traject ~80% van de requirements definiëren; wijzigingen via change‑proces.
- Documentatie tijdens ontwikkeling; document‑gedreven + mondelinge sessies.
- Geregelde klantdeelname.
- Mitigeren van (technologie)risico’s als drijfveer.
- Combinatie van ‘volgens specificatie’ en ‘waarde voor de business’.
- Door manager aangestuurde teams; vaste rollen met eigen producten.

**Agile**

- Korte sprints (max. 1 maand); sprintinhoud bij sprintstart bepaald.
- Dagelijkse mogelijkheid om werkwijze aan te passen.
- Geordende product backlog; continu bijwerken op basis van klant/marktbehoefte.
- Hoogste prioriteit: maximaal actuele businesswaarde opleveren.
- Documentatie: alleen wat nodig is; nadruk op samenwerking en mondelinge overdracht.
- ‘Waarde voor de business’ aanpak.
- Multidisciplinaire, zelfsturende teams; verantwoordelijkheid voor opgeleverde producten.
- Hoge klantdeelname (product owner).

> Technieken (Use Cases, UML, prototyping, daily stand‑ups, etc.) zijn niet bepalend voor de methode; ze kunnen in meerdere methoden waardevol zijn.

---

## Aspecten en criteria

Voor de keuze van ontwikkelmethoden spelen eigenschappen van **opdrachtgever**, **opdrachtnemer**, **contract** en **product** een rol. Een eigenschap bepaalt het succes als volgt:

- **A** — voorwaarde voor succes
- **B** — verhoogt kans op succes
- **C** — geen invloed
- **D** — beperkte maatregelen nodig
- **E** — uitgebreide maatregelen nodig

Een geschikte aanpak scoort veel **A/B/C**, beperkt **D**, en liefst geen **E**. **D/E** zijn risico’s en leiden vaak tot een hybride aanpak.

### Opdrachtgevereigenschappen

#### 4.1.1 Beschikbaarheid van stakeholders

Deelnameduur verschilt per methode: lineair vraagt kort en intensief op momenten; agile vraagt constante deelname van de product owner. Indien hoge constante beschikbaarheid niet mogelijk is, kies iteratiever werken met agile technieken.

| Beschikbaarheid | Lineair | Iteratief | Agile |
|---|---:|---:|---:|
| Laag — maandelijks enige dagen | D | D | E |
| Gemiddeld — wekelijks een dag | D | B | D |
| Hoog — dagelijks uren | C | B | A |

#### 4.1.2 Cultuur van delegeren

Wanneer beslissingsbevoegdheid niet laag in de organisatie ligt (of informeel hoger wordt beslist), wordt dagelijks besluiten nemen lastig.

| Beslissingslaag | Lineair | Iteratief | Agile |
|---|---:|---:|---:|
| Hoger management | B | E | E |
| Middel management | C | B | E |
| Gemandateerd aan projectleden | C | B | A |

Maatregel bij **E**: organiseer bijv. wekelijks een besluitvormend overleg (beperkt agility).

#### 4.1.3 Beslisvaardigheid

Is de cultuur snel beslissen, of organisatiebreed afstemmen?

| Beslisvaardigheid | Lineair | Iteratief | Agile |
|---|---:|---:|---:|
| Laag — organisatiebreed afstemmen | B | D | E |
| Gemiddeld — op afdelingsniveau | C | B | E |
| Hoog — medewerkers nemen beslissingen | C | B | A |

Maatregel: wekelijkse stakeholderbijeenkomst.

#### 4.1.4 Stabiliteit van scope en requirements

Weet de business vooraf goed wat nodig is?

| Stabiliteit | Lineair | Iteratief | Agile |
|---|---:|---:|---:|
| Hoog — 100% | A | B | C |
| Gemiddeld — 80% bekend | D | B | C |
| Laag — eindresultaat ligt open | E | D | B |

#### 4.1.5 Omgaan met onzekerheid

Kan de organisatie omgaan met onzekerheid in uitkomst? Een visie die de bandbreedte van wijzigingsruimte afbakent helpt.

| Omgang met onzekerheid | Lineair | Iteratief | Agile |
|---|---:|---:|---:|
| Gaan voor 100% zekerheid | B | D | E |
| Kunnen leven met 80% | C | B | E |
| Minder mag ook | D | B | A |

#### 4.1.6 Mogelijke deploymentfrequentie

Hoe vaak kunnen nieuwe versies worden uitgerold (test/acceptatie/producent)? Projectteam kan eigen omgevingen inrichten; lage frequentie schuift aanpak naar iteratief of lineair.

| Deploymentfrequentie | Lineair | Iteratief | Agile |
|---|---:|---:|---:|
| Laag — max. eens per kwartaal | B | B | E |
| Gemiddeld — maandelijks | C | B | B |
| Hoog — wekelijks of vaker | C | C | A |

#### 4.1.7 Businessvolwassenheid in ICT

Hoe ervaren is de business in ICT‑projecten?

| Ervarenheid in projecten | Lineair | Iteratief | Agile |
|---|---:|---:|---:|
| Laag — hooguit enkele projecten | D | B | D |
| Gemiddeld — één of twee per jaar | B | B | D |
| Hoog — meerdere parallel per jaar | D | C | A |

> Opmerking: lineair wordt negatief beïnvloed bij weinig ICT‑ervaring; volledig ontwerp vooraf vraagt veel voorstellingsvermogen.

### Opdrachtnemereigenschappen

#### 4.2.1 Senioriteit van het team

Team is mix van senioriteit; teamgrootte beïnvloedt inzet van junioren.

| Senioriteit | Lineair | Iteratief | Agile |
|---|---:|---:|---:|
| Laag — veel junioren/medioren | C | D | E |
| Gemiddeld — mix met begeleiding | C | B | C |
| Hoog — nauwelijks junioren | C | B | A |

#### 4.2.2 Medewerkers multidisciplinair

Multidisciplinariteit verhoogt snelheid en flexibiliteit; essentieel in agile.

| Multidisciplinaire teamleden | Lineair | Iteratief | Agile |
|---|---:|---:|---:|
| Weinig — ~10% | B | D | E |
| Gemiddeld — 25–50% | C | B | D |
| Hoog — >75% | C | C | A |

### Contract

#### 4.3.1 Verwachting t.a.v. resultaat

Hoe wordt resultaat beoordeeld?

| Resultaatverwachting | Lineair | Iteratief | Agile |
|---|---:|---:|---:|
| Conform vooraf vastgestelde specificaties | A | C | D |
| Conform specificaties die gedurende het project gemanaged worden | D | B | D |
| Open binnen grove scope, tijd & geldbeperking | E | D | B |

#### 4.3.2 Afrekenmethoden

Contractuele afrekenvormen:

| Afrekenmethode | Lineair | Iteratief | Agile |
|---|---:|---:|---:|
| Fixed — na aftekenen requirements | B | B | D |
| Fixed — na aftekenen requirements + risicoafdekking | B | B | E |
| Fixed — budget box | C | B | B |
| Time & Material | B | B | C |

> Maatregel voor agile bij mismatch: andere contractvorm bespreken die beter aansluit en meer waarde levert.

#### 4.3.3 Afspraken over oplevermoment

| Vaststellen oplevermoment | Lineair | Iteratief | Agile |
|---|---:|---:|---:|
| Open | B | C | D |
| Na aftekenen requirements | B | D | D |
| Na aftekenen requirements + risicoafdekking | B | B | D |
| Time box | D | B | A |

#### 4.3.4 Functionele scope in het contract

Kan verschillen van de werkelijke requirements‑stabiliteit.

| Functionele scope | Lineair | Iteratief | Agile |
|---|---:|---:|---:|
| Fixed | A | B | D |
| Gemiddeld — ~80% bekend | D | B | D |
| Open | E | D | B |

### Productaspecten

#### 4.4.1 Soort toegevoegde waarde

| Toegevoegde waarde | Lineair | Iteratief | Agile |
|---|---:|---:|---:|
| Compliancy | B | B | C |
| Mix van compliancy en businesswaarde | D | B | C |
| Businesswaarde | D | D | B |

#### 4.4.2 Bekendheid technologie

| Technologie | Lineair | Iteratief | Agile |
|---|---:|---:|---:|
| Bewezen | A | C | B |
| Nieuw — expliciet doel van project | E | B | C |
| Nieuw — geen doel van project | E | B | D |

Werken met nieuwe technologie in agile is mogelijk als het *bewijzen van de technologie* het expliciete doel is (met eigen product owner). Het uiteindelijke productproject kan apart met een andere product owner worden uitgevoerd.

#### 4.4.3 Time‑to‑market

| Time‑to‑market | Lineair | Iteratief | Agile |
|---|---:|---:|---:|
| Lang — ~1 jaar | A | B | D |
| Gemiddeld — ~1 kwartaal | D | B | C |
| Hoog — < 1 maand | E | E | B |

#### 4.4.4 Releasecyclus

| Releasecyclus | Lineair | Iteratief | Agile |
|---|---:|---:|---:|
| Jaar | A | B | C |
| Kwartaal | D | B | C |
| Maand of minder | E | E | B |

---

## Over Capgemini

Met ruim 120.000 mensen in 40 landen is Capgemini wereldwijd een van de meest vooraanstaande aanbieders van consulting‑, technology‑ en outsourcingdiensten. In 2011 rapporteerde Capgemini Group een omzet van 9,7 miljard euro. Samen met zijn klanten creëert en realiseert Capgemini resultaatgerichte business‑ en technology‑oplossingen, toegesneden op de klantbehoefte. Als een cultureel diverse organisatie heeft Capgemini zijn eigen onderscheidende manier van werken, de *Collaborative Business Experience™*. Hierbij maakt Capgemini gebruik van het wereldwijde leveringsmodel Rightshore®. Meer informatie via **www.nl.capgemini.com**.

Rightshore® is een handelsmerk van Capgemini.

**Capgemini Nederland B.V.**  
Papendorpseweg 100  
Postbus 2575 — 3500 GN Utrecht  
[jan.klabbers@capgemini.com](mailto:jan.klabbers@capgemini.com) — Tel +31 30 689 84 54  
[erik.hoolt@capgemini.com](mailto:erik.hoolt@capgemini.com) — Tel +31 30 689 53 44

---

> *Adresblokken en stock‑credit uit de PDF zijn niet inhoudelijk opgenomen. Logo/beeldmateriaal en grafieken zijn in deze Markdown weggelaten.*
