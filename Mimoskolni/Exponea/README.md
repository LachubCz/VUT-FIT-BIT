Models for churn analyzation (data mining)
====================
Script description
-----------
Script processes customers data and tries to predict, which customers are gonna be lost.

**Data models:**

* Logistic Regression (~ 0.8664 %)
* Decision Tree (~ 0.9224 %)
* Random Forest (~ 0.9524 %)

Usage
-----
##### python main.py [-lr] [-dt] [-rf] [-d d] [-train] [-test] [-eval] [-train_test] [-s] [-lrm lrm] [-dtm dtm] [-rfm rfm] [-dic dic]
###### Parameters: 

    -lr             |   script will use Logistic Regression model (it's possible to run multiple modes simultaneously)
    -dt             |   script will use Decision Tree model (it's possible to run multiple modes simultaneously)
    -rf             |   script will use Random Forest model (it's possible to run multiple modes simultaneously)
    -d dataset      |   dataset name (location) to load
    -train          |   script will run in train mode, model will train on hole dataset, no output
    -test           |   script will run in test mode, model will process hole dataset and compare own predictions with correct labels, output is performace
    -eval           |   script will run in eval mode, model will predict labels for hole dataset and return them, if you want to use this mode, you have to load taugh models, output is array of predicted labels
    -train_test     |   script will run in train_test mode, first half of dataset will be used as training data and second half as a testing data, output is performace
    -s              |   script will save models
    -lrm lrmodel    |   name of Logistic Regression model (location) to load
    -dtm dtmodel    |   name of Decision Tree model (location) to load
    -rfm rfmodel    |   name of Random Forest model (location) to load
    -dic dictionary |   name of dictionary (location) to load

Examples
-----------------
    ./isashark --h
    ./isashark -a dstmac filename.pcap
    ./isashark -s bytes filename1.pcap filename2.pcap
    ./isashark -l 3 -s packets -a dstport filename.pcap
    ./isashark -f "src host 2001:db8::1" filename.pcap

Libraries
-----------------
**Standard libraries:**
* os
* sys
* math
* argparse
* collections
* pickle

**Third party libraries:**
* numpy
* sklearn

Files:
---------------------------
**./:**

* data_worker.py
* main.py
* models.py
* readme.md
* tools.py

**./datasets:**

* churn.ALL
* churn.NAMES
* churn_for_eval.ALL
* churn_for_test.ALL
* churn_for_train.ALL

**./models:**

* decision_tree.dot
* decision_tree.pkl
* dictionary.pkl
* log_regression.pkl
* rmd_forest.pkl

****

###### Created by: Petr Buchal
