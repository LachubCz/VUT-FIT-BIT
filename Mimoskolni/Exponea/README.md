Models for churn analyzation (data mining)
====================
Script description
-----------
Script processes customers data and tries to predict, which customers are gonna be lost.

**Data models:**

* Logistic Regression (86.64 %)
* Decision Tree (92.24 %)
* Random Forest (95.24 %)

Usage
-----
##### python main.py [-lr] [-dt] [-rf] [-d d] [-train] [-test] [-eval] [-train_test] [-s] [-lrm lrm] [-dtm dtm] [-rfm rfm] [-dic dic]
###### Parameters: 

    -lr         |   script will use Logistic Regression model (it's possible to run multiple modes simultaneously)
    -dt         |   script will use Decision Tree model (it's possible to run multiple modes simultaneously)
    -rf         |   script will use Random Forest model (it's possible to run multiple modes simultaneously)
    -d d        |   dataset name (location) to load
    -train      |   script will run in train mode, model will train on hole dataset, no output
    -test       |   script will run in test mode, model will process hole dataset and compare own predictions with correct labels, output is performace
    -eval       |   script will run in eval mode, model will predict labels for hole dataset and return them, if you want to use this mode, you have to load taugh models, output is array of predicted labels
    -train_test |   script will run in train_test mode, first half of dataset will be used as training data and second half as a testing data, output is performace
    -s          |   script will save models
    -lrm lrm    |   name of Logistic Regression model (location) to load
    -dtm dtm    |   name of Decision Tree model (location) to load
    -rfm rfm    |   name of Random Forest model (location) to load
    -dic dic    |   name of dictionary (location) to load

Examples
-----------------
    python main.py -d datasets/churn.all -lr -rf -dt -train_test -s
    python main.py -d datasets/churn_for_train.all -lr -train -s
    python main.py -d datasets/churn_for_test.all -lr -test -lrm models/log_regression.pkl -dic models/dictionary.pkl
    python main.py -d datasets/churn_for_eval.all -lr -dt -eval -lrm models/log_regression.pkl -dtm models/decision_tree.pkl -dic models/dictionary.pkl

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
* presentation.pdf
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
