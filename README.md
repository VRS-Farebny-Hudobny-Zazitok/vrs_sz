# Farebný hudobný zážitok
Semestrálne zadanie z predmetu VRS\
<b>Členovia:</b> Jakub Mihálik, Aleš Melichar, Michal Lúčny, Martin Berki 

---

Užívateľ bude môcť pomocou klávesnice hrať rôzne tóny.  
Na základe vstupu z klávesnice bude generovaný zvukový signál, ktorý bude D/A prevodníkom posielaný na reproduktor.
Podľa zahraného tónu bude taktiež svietiť 8-segmentové svetlo príšlušnou farbou, ktorá bude závisieť od hranej frekvencie.

---
### Úlohy
- ☑ Vytvorenie projektu a základná konfigurácia (GPIO, DMA, TIMERs, ...) 
- ☑ Vytvorenie obslužného kódu pre klávesnicu
- ☑ Vytvorenie databázy tónov a farieb
- ☑ Implementácia generovania signálov pomocou timer-ov
- ☑ Vytvorenie obslužného kódu pre 8-segmentové svetlo

---
#### Zapojenie
<b>Klávesnica</b>\
C3 - PB0\
C2 - PB7\
C1 - PB6\
C0 - PB1\
R0 - PA8\
R1 - PA11\
R2 - PB5\
R3 - PB4\
<b>Reproduktor</b>\
PA4\
<b>Svetlo</b>\
PA10
