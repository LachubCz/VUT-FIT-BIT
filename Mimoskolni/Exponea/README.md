Models for churn analyzation (data mining)
====================
Script description
-----------
Script processes customers data and tries to predict, which customers are gonna be lost.

**Data models:**

* Logistic Regression
* Decision Tree
* Random Forest

Usage
-----
##### python main.py [-lr] [-dt] [-rf][-d dataset] [-train] [-test] [-eval] [-train_test] [-s] [-lrm lrmodel] [-dtm dtmodel] [-rfm rfmodel] [-dic dictionary]
###### Parameters: 

    -h  |   Vypíše nápovědu a ukončí program.
    -a aggr-key   |   Zapnutí agregace podle klíče aggr-key, což může být srcmac značící zdrojovou MAC adresu, dstmac značící cílovou MAC adresu, srcip značící zdrojovou IP adresu, dstip značící cílovou IP adresu, srcport značící číslo zdrojového transportního portu nebo dstport značící číslo cílového transportního portu.
    -s sort-key |   Zapnutí řazení podle klíče sort-key, což může být packets (počet paketů) nebo bytes (počet bajtů). Řadit lze jak agregované tak i neagregované položky. Řadí se vždy sestupně.
    -l limit    |    Nezáporné celé číslo v desítkové soustavě udávající limit počtu vypsaných položek.
    -f filter-expression    |    Program zpracuje pouze pakety, které vyhovují filtru danému řetězcem filter-expression. 
    file    |    Cesta k souboru ve formátu pcap (čitelný knihovnou libpcap). Možné je zadat jeden a více souborů.

Examples
-----------------
    ./isashark --h
    ./isashark -a dstmac filename.pcap
    ./isashark -s bytes filename1.pcap filename2.pcap
    ./isashark -l 3 -s packets -a dstport filename.pcap
    ./isashark -f "src host 2001:db8::1" filename.pcap


Source code:
---------------------------
data_worker.py

main.py

models.py

tools.py

****

###### Created by: Petr Buchal
