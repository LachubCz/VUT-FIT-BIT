Analyzátor paketů
====================
Popis programu
-----------
Konzolová aplikace pro offline analýzu síťového provozu, umožňující agregaci, řazení a filtraci.

**Podporované protokoly:**

Vrstva síťového rozhraní:
* Ethernet
* IEEE 802.1Q včetně IEEE 802.1ad

Síťová vrstva:
* IPv4
* IPv6
* ICMPv4
* ICMPv6

Transportní vrstva:
* TCP
* UDP

Použití
-----
##### isashark [-h] [-a aggr-key] [-s sort-key] [-l limit] [-f filter-expression] file ..
###### Parametry: 

    -h  |   Vypíše nápovědu a ukončí program.
    -a aggr-key   |   Zapnutí agregace podle klíče aggr-key, což může být srcmac značící zdrojovou MAC adresu, dstmac značící cílovou MAC adresu, srcip značící zdrojovou IP adresu, dstip značící cílovou IP adresu, srcport značící číslo zdrojového transportního portu nebo dstport značící číslo cílového transportního portu.
    -s sort-key |   Zapnutí řazení podle klíče sort-key, což může být packets (počet paketů) nebo bytes (počet bajtů). Řadit lze jak agregované tak i neagregované položky. Řadí se vždy sestupně.
    -l limit    |    Nezáporné celé číslo v desítkové soustavě udávající limit počtu vypsaných položek.
    -f filter-expression    |    Program zpracuje pouze pakety, které vyhovují filtru danému řetězcem filter-expression. 
    file    |    Cesta k souboru ve formátu pcap (čitelný knihovnou libpcap). Možné je zadat jeden a více souborů.

Příklady použití
-----------------
    ./isashark --h
    ./isashark -a dstmac filename.pcap
    ./isashark -s bytes filename1.pcap filename2.pcap
    ./isashark -l 3 -s packets -a dstport filename.pcap
    ./isashark -f "src host 2001:db8::1" filename.pcap

Makefile
--------
`make`        - přeloží zdrojový kód programu

`make reassemble`    - smaže přeložený zdrojový kód a přeloží ho znovu

Implementace
-------------------------------------
Detaily implementace jsou popsány v souboru manual.pdf.

Seznam odevzdaných souborů:
---------------------------
isashark.c

Makefile

manual.pdf

readme.md

****

###### Vytvořil: Petr Buchal (xbucha02)