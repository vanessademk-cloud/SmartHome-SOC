# Smart Home – SOČ projekt

SmartHome projekt – SOČ 2025/2026
Tento repozitár obsahuje zdrojové kódy k projektu **Smart Home**, ktorý bol vytvorený v rámci Stredoškolskej odbornej činnosti (SOČ).
Projekt predstavuje model inteligentnej domácnosti, ktorý využíva senzory a mikrokontroléry Arduino na monitorovanie prostredia a automatické ovládanie zariadení.

## Funkcie systému

Systém obsahuje viacero senzorov a automatizovaných reakcií:

* meranie **teploty a vlhkosti (DHT11)**
* **ventilátor**, ktorý sa zapne pri vyššej teplote
* **senzor vlhkosti pôdy + čerpadlo** na automatické zavlažovanie
* **fotorezistor + LED** na automatické zapnutie svetla pri tme
* **fingerprint senzor + servo** na simuláciu inteligentného zámku
* **flame senzor + buzzer** na detekciu plameňa
* **gas senzor** na detekciu plynov
* **ultrazvukový senzor + semafor** na detekciu vzdialenosti
* **OLED displej** na zobrazenie nameraných údajov

## Vizualizácia dát

Namerané údaje zo senzorov sú zobrazované:

* v mobilnej aplikácii **Blynk**
* lokálne priamo v modeli domu pomocou displeja

Používateľ tak môže sledovať aktuálny stav systému a jednotlivých zariadení.

## Obsah repozitára

* **FINAL_R4.ino** – kód pre Arduino R4 (WiFi komunikácia a Blynk)
* **FINAL_UNO1.ino** – kód pre Arduino UNO (OLED displej, ultrazvukový senzor a senzory)
* **FINAL_UNO2.ino** – kód pre Arduino UNO (fingerprint, servo, vlhkosť pôdy, čerpadlo, svetlo)
* **enroll.ino** – kód na registráciu odtlačku prsta do fingerprint senzora

## Použité technológie

* Arduino UNO
* Arduino UNO R4 WiFi
* Blynk
* rôzne senzory a akčné prvky (servo motor, ventilátor, čerpadlo)

## Autor Vanessa Demková

Projekt bol vytvorený v rámci **Stredoškolskej odbornej činnosti (SOČ)** v oblasti **Internet vecí (IoT) a inteligentných domácností**.

