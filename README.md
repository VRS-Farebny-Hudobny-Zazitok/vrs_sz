# Farebný hudobný zážitok
Semestrálne zadanie z predmetu VRS\
<b>Členovia:</b> Jakub Mihálik, Aleš Melichar, Michal Lúčny, Martin Berki 

---

Užívateľ bude môcť pomocou klávesnice hrať rôzne tóny. Vstup z klávesnice bude pomocou timer-ou generovať PWM signál.
Výstupny upravený signál bude konvertovaný pomocou D/A prevodníka na spojitý signál a posielaný na reproduktor.
Podľa zahraného tónu bude taktiež svietiť 8-segmentové svetlo príšlušnou farbou, ktorá bude závisieť od hranej frekvencie.

---
### Úlohy
- Vytvorenie projektu a základná konfigurácia (GPIO, DMA, TIMERs, ...)
- Vytvorenie obslužného kódu pre klávesnicu
- Vytvorenie databázy tónov a farieb
- Implementácia generovania signálov pomocou timer-ou
- Vytvorenie obslužného kódu pre 8-segmentové svetlo
