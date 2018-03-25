#!/usr/bin/env python3
"""
    File name: main.py
    Author: Petr Buchal
    Date created: 23/03/2013
    Date last modified: 26/03/2013
    Python Version: 3.6
    Description:
        script for churn data analyzation
        contains three models:
                    logistic regression
                    decision tree
                    random forrest
"""
import sys
import os.path
import argparse
import numpy as np
from tools import get_dataset
import data_worker as dt
from models import Model

#this will allow to print all numpy array for evaluation mode
np.set_printoptions(threshold=np.nan)

def err_print(*args, **kwargs):
    """
    method for printing to stderr
    """
    print(*args, file=sys.stderr, **kwargs)

def get_args():
    """
    method for parsing of arguments
    """
    parser = argparse.ArgumentParser()

    parser.add_argument('-lr', action="store_true", default=False) #use log_regression model
    parser.add_argument('-dt', action="store_true", default=False) #use decision_tree model
    parser.add_argument('-rf', action="store_true", default=False) #use rnd_forrest model

    parser.add_argument('-d', action="store", dest="d") #dataset file

    parser.add_argument('-train', action="store_true", default=False) #all data are for training
    parser.add_argument('-test', action="store_true", default=False) #all data are for testing
    parser.add_argument('-eval', action="store_true", default=False) #all data for evaluation
    parser.add_argument('-train_test', action="store_true", default=False) #half data for training, half for testing

    parser.add_argument('-s', action="store_true", default=False) #save models
    parser.add_argument('-lrm', action="store", dest="lrm") #load log_regression model
    parser.add_argument('-dtm', action="store", dest="dtm") #load decision_tree model
    parser.add_argument('-rfm', action="store", dest="rfm") #load rnd_forrest model
    parser.add_argument('-dic', action="store", dest="dic") #load dictionary

    args = parser.parse_args()

    if not args.lr and not args.dt and not args.rf:
        err_print("You haven't selected any model.")
        sys.exit(-1)

    if args.d is None:
        err_print("You haven't selected dataset file.")
        sys.exit(-1)

    mode = 0
    if args.train:
        mode = mode + 1
    if args.test:
        mode = mode + 1
    if args.eval:
        mode = mode + 1
    if args.train_test:
        mode = mode + 1
    if mode != 1:
        err_print("You have selected multiple or none mode.")
        sys.exit(-1)

    if args.lrm is not None:
        if not os.path.isfile(args.lrm):
            err_print("Model file doesn't exist.")
            sys.exit(-1)

    if args.dtm is not None:
        if not os.path.isfile(args.dtm):
            err_print("Model file doesn't exist.")
            sys.exit(-1)

    if args.rfm is not None:
        if not os.path.isfile(args.rfm):
            err_print("Model file doesn't exist.")
            sys.exit(-1)

    if args.test or args.eval:
        if args.lr and args.lrm is None:
            err_print("You've selected mode, which requires existing model.")
            sys.exit(-1)
        if args.dt and args.dtm is None:
            err_print("You've selected mode, which requires existing model.")
            sys.exit(-1)
        if args.rf and args.rfm is None:
            err_print("You've selected mode, which requires existing model.")
            sys.exit(-1)
        if args.dic is None:
            err_print("You've selected mode, which requires existing dictionary.")
            sys.exit(-1)

    return args

def main():
    """
    main method for data processing
    """
    args = get_args()

    #load dataset
    dataset = get_dataset(args.d)

    #remove phone number and state for better accuracy (tested)
    dataset = dt.remove_attribute(dataset, 3)
    dataset = dt.remove_attribute(dataset, 0)

    #models inicialization
    models = Model(log_regression=args.lr, decision_tree=args.dt, rnd_forest=args.rf)
    if args.lrm is not None: models.load_model("log_regression", args.lrm)
    if args.dtm is not None: models.load_model("decision_tree", args.dtm)
    if args.rfm is not None: models.load_model("rnd_forest", args.rfm)
    if args.lrm is not None or args.dtm is not None or args.rfm is not None:
        models.load_dictionary(args.dic)

    #training
    if args.train:
        #get data for training
        train_data, train_labels = dt.get_labels(dataset)
        #transform categorical values into numerical values
        train_data, dictionary_list = dt.correct_dataset(train_data)
        #copy dictionary to models.dictionary
        models.dictionary = dictionary_list
        #training
        models.train_on_batch(train_data, train_labels)

    #testing
    if args.test:
        #get data for testing
        test_data, test_labels = dt.get_labels(dataset)
        #transform categorical values into numerical values according to dictionary in models
        test_data = dt.make_test_set(test_data, models.dictionary)
        #get scores
        score_lr, score_dt, score_rf = models.score_on_batch(test_data, test_labels)
        print("Logistic Regression: {}\nDecision Tree: {}\nRandom Forest: {}\n"
              .format(score_lr, score_dt, score_rf))

    #evaluation
    if args.eval:
        #get data for evaluating
        eval_data, _ = dt.get_labels(dataset)
        #transform categorical values into numerical values according to dictionary in models
        eval_data = dt.make_test_set(eval_data, models.dictionary)
        #print predictions
        print(models.predict_on_batch(eval_data))

    #training and testing
    if args.train_test:
        #get data for training and testing
        train_data, test_data, train_labels, test_labels = dt.split_dataset(dataset)
        #transform categorical values into numerical values
        train_data, dictionary_list = dt.correct_dataset(train_data)
        #copy dictionary to models.dictionary
        models.dictionary = dictionary_list
        #transform categorical values into numerical values according to dictionary gained from training data
        test_data = dt.make_test_set(test_data, models.dictionary)
        #training
        models.train_on_batch(train_data, train_labels)
        #testing
        score_lr, score_dt, score_rf = models.score_on_batch(test_data, test_labels)
        print("Logistic Regression: {}\nDecision Tree: {}\nRandom Forest: {}\n"
              .format(score_lr, score_dt, score_rf))

    #save models
    if args.s:
        models.save_models()
        models.save_dictionary()
        models.save_dec_tree_graph("decision_tree.dot")

main()
