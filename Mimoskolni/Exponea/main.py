#!/usr/bin/env python3
"""
    docstring
"""
from tools import get_dataset
from data_worker import DataWorker as dt
from models import Model

def main():
    """
    docstring
    """
    #load dataset
    dataset = get_dataset("churn.all")

    #remove phone number and state for better accuracy (tested)
    dataset = dt.remove_attribute(dataset, 3)
    dataset = dt.remove_attribute(dataset, 0)

    #get data for training and evaluating
    train_data, test_data, train_labels, test_labels = dt.split_dataset(dataset)

    #transform categorical values into numerical values
    train_data, dictionary_list = dt.correct_dataset(train_data)

    #transform categorical values into numerical values according to dictionary from training data
    test_data = dt.make_test_set(test_data, dictionary_list)

    #models inicialization
    models = Model(log_regression=True, decision_tree=True, rnd_forest=True)
    #models.load_model("log_regression", "log_regression.pkl")
    models.train_on_batch(train_data, train_labels)

    print(models.score_on_batch(test_data, test_labels))
    #models.save_models()

    #graphviz export of decision tree
    models.export_graphviz("decision_tree.dot")
