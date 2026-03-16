# Smart Home – SOČ projekt

Tento repozitár obsahuje zdrojové kódy k projektu Smart Home, ktorý bol vytvorený v rámci Stredoškolskej odbornej činnosti (SOČ).  
Projekt predstavuje model inteligentnej domácnosti, ktorý využíva rôzne senzory a mikrokontroléry Arduino na monitorovanie prostredia a automatické ovládanie zariadení.

## Funkcie systému

Projekt obsahuje viacero senzorov a automatických reakcií:

- DHT11 – meranie teploty a vlhkosti
- ventilátor zapínaný podľa teploty
- senzor vlhkosti pôdy + čerpadlo (automatické zavlažovanie)
- fotorezistor + LED (automatické osvetlenie)
- fingerprint senzor + servo (simulácia inteligentného zámku)
- flame senzor + buzzer (detekcia plameňa)
- gas senzor (detekcia plynov)
- ultrazvukový senzor + semafor (detekcia vzdialenosti)
- OLED displej (lokálne zobrazovanie údajov)

## Vizualizácia dát

Namerané údaje zo senzorov sú zobrazované:
- v mobilnej aplikácii Blynk
- lokálne priamo v modeli domu pomocou displeja

## Obsah repozitára

FINAL_R4.ino  
Kód pre Arduino R4 – WiFi komunikácia a odosielanie dát do aplikácie Blynk.

FINAL_UNO1.ino  
Kód pre Arduino UNO – riadenie senzorov, OLED displej a ultrazvukový senzor.

FINAL_UNO2.ino  
Kód pre Arduino UNO – fingerprint senzor, servo, senzor vlhkosti pôdy, čerpadlo a osvetlenie.

enroll.ino  
Kód na registráciu odtlačku prsta do fingerprint senzora.

## Použité technológie

- Arduino UNO
- Arduino UNO R4 WiFi
- Blynk
- rôzne senzory a akčné prvky (servo, ventilátor, čerpadlo)

## Autor

Projekt bol vytvorený v rámci Stredoškolskej odbornej činnosti (SOČ) v oblasti Internet vecí (IoT).
