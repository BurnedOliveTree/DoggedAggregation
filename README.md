# Project for PSI subject
## Project description
System agregacji dokumentów.
Zaprojektuj i zaimplementuj system bramy komunikacyjnej dla podsieci do 1024 urządzeń sensorycznych. Brama odbiera od urządzeń krótkie binarne paczki danych, agreguje je i rozsyła do predefiniowanego zbioru serwerów (do 32). Urządzenia wysyłają dane w określonych interwałach czasowych. Brama opatruje zagregowane paczki stemplem czasowym i podpisem cyfrowym. Komunikacja odbywa się po UDP. Co określony czas brama wysyła do urządzeń komunikat synchronizujący czas i wariację opóźnienia (jitter) dla wysyłanych paczek danych.
