"""
    docstring
"""
import os
import pickle
import numpy as np
from sklearn import tree
from sklearn import linear_model
from sklearn.ensemble import RandomForestClassifier

class Model():
    """
    docstring
    """
    def __init__(self, log_regression=False, decision_tree=False, rnd_forest=False):
        if log_regression:
            self.log_regression = linear_model.LogisticRegression(C=1024, solver="lbfgs",
                                                                  dual=False, max_iter=200)
        else:
            self.log_regression = None
        if decision_tree:
            self.decision_tree = tree.DecisionTreeClassifier()
        else:
            self.decision_tree = None
        if rnd_forest:
            self.rnd_forest = RandomForestClassifier(n_estimators=128)
        else:
            self.rnd_forest = None

        self.dictionary = None

    def train_on_batch(self, batch, labels):
        """
        docstring
        """
        if self.log_regression != None:
            self.log_regression.fit(batch, labels)

        if self.decision_tree != None:
            self.decision_tree.fit(batch, labels)

        if self.rnd_forest != None:
            self.rnd_forest.fit(batch, labels)

    def predict_on_batch(self, batch):
        """
        docstring
        """
        labels_lr = None
        if self.log_regression != None:
            labels_lr = self.log_regression.predict(batch)

        labels_dt = None
        if self.decision_tree != None:
            labels_dt = self.decision_tree.predict(batch)

        labels_rf = None
        if self.rnd_forest != None:
            labels_rf = self.rnd_forest.predict(batch)

        return labels_lr, labels_dt, labels_rf

    def score_on_batch(self, batch, labels):
        """
        docstring
        """
        score_lr = None
        if self.log_regression != None:
            score_lr = self.log_regression.score(batch, labels)

        score_dt = None
        if self.decision_tree != None:
            score_dt = self.decision_tree.score(batch, labels)

        score_rf = None
        if self.rnd_forest != None:
            score_rf = self.rnd_forest.score(batch, labels)

        return score_lr, score_dt, score_rf

    def export_graphviz(self, filename):
        """
        docstring
        """
        if self.decision_tree != None:
            labels = np.array(["account length", "area code", "international plan", "voice mail plan",
                               "number vmail messages", "total day minutes", "total day calls",
                               "total day charge", "total eve minutes", "total eve calls", "total eve charge",
                               "total night minutes", "total night calls", "total night charge",
                               "total intl minutes", "total intl calls", "total intl charge",
                               "number customer service calls"])

            file = open(os.getcwd()+"\\" + filename, "w")
            tree.export_graphviz(self.decision_tree, out_file=file, feature_names=labels)
            file.close()

    def save_models(self):
        """
        docstring
        """
        if self.log_regression != None:
            with open("log_regression.pkl", "wb") as output:
                pickle.dump(self.log_regression, output, pickle.HIGHEST_PROTOCOL)

        if self.decision_tree != None:
            with open("decision_tree.pkl", "wb") as output:
                pickle.dump(self.decision_tree, output, pickle.HIGHEST_PROTOCOL)

        if self.rnd_forest != None:
            with open("rnd_forest.pkl", "wb") as output:
                pickle.dump(self.rnd_forest, output, pickle.HIGHEST_PROTOCOL)

    def load_model(self, model_type, filename):
        """
        docstring
        """
        if model_type == "log_regression":
            with open(filename, "rb") as input:
                self.log_regression = pickle.load(input)

        elif model_type == "decision_tree":
            with open(filename, "rb") as input:
                self.decision_tree = pickle.load(input)

        elif model_type == "rnd_forest":
            with open(filename, "rb") as input:
                self.rnd_forest = pickle.load(input)

    def save_dictionary(self):
        """
        docstring
        """
        if self.rnd_forest != None:
            with open("dictionary.pkl", "wb") as output:
                pickle.dump(self.dictionary, output, pickle.HIGHEST_PROTOCOL)

    def load_dictionary(self, filename):
        """
        docstring
        """
        with open(filename, "rb") as input:
            self.dictionary = pickle.load(input)
