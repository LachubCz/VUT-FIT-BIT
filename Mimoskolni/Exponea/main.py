import random
import sys
import os
import numpy as np
from tools import get_dataset, print_custmr_info, print_smllrdata_info
from decision_tree import *
from sklearn import tree
from sklearn.ensemble import RandomForestClassifier
from sklearn import linear_model
from sklearn.tree import DecisionTreeRegressor

dataset = get_dataset("churn.all")


dataset = remove_done_attr(dataset, 3)

dataset = remove_done_attr(dataset, 0)



train_set, test_set, train_labels, test_labels = split_dataset(dataset)



train_set, dictionary_list = correct_dataset(train_set)

dec_tree = get_tree(train_set, train_labels)

test_set = make_test_set(test_set, dictionary_list)

predict_labels1 = dec_tree.predict(test_set)

print("DecisionTreeClassifier: ", dec_tree.score(test_set, test_labels))

export_graphviz(dec_tree)
#################################
clf = RandomForestClassifier(n_estimators=128)
clf.fit(train_set, train_labels)
predict_labels2 = clf.predict(test_set)
print("RandomForestClassifier: ", clf.score(test_set, test_labels))
#export_graphviz(clf)

#####################################



#dataset = get_dataset("churn.all")

#dicta = dict()
#for i, item in enumerate(array):
#    dicta.update({i: item})
#print(array)
#print(dataset[0])
#print(dicta)
#print(dataset[0])

#train_set, test_set, train_labels, test_labels = split_dataset(dataset)

#train_set, dictionary_list = correct_dataset1(train_set)
#dataset = get_dataset("churn.all")


logreg = linear_model.LogisticRegression(C=1024, solver="lbfgs",dual=False,max_iter=200)

logreg.fit(train_set, train_labels)
predict_labels3 = logreg.predict(test_set)
print("LogisticRegression: ", logreg.score(test_set, test_labels))

dataset = get_dataset("churn.all")




train_set, test_set, train_labels, test_labels = split_dataset(dataset)



train_set, dictionary_list = correct_dataset1(train_set)


test_set = make_test_set1(test_set, dictionary_list)




#test_set = make_test_set1(test_set, dictionary_list)

#print(test_set)
logreg = linear_model.LogisticRegression(C=1024, solver="lbfgs",dual=False,max_iter=200)

logreg.fit(train_set, train_labels)
predict_labels3 = logreg.predict(test_set)
print("LogisticRegression: ", logreg.score(test_set, test_labels))
    
correct = 0
elm = None
for i in range(len(predict_labels1)):
    if (predict_labels1[i]+ predict_labels2[i]+predict_labels3[i]) == 2 or (predict_labels1[i]+predict_labels2[i]+predict_labels3[i]) == 3:
        elm = 1
    else: 
        elm = 0
    if elm == test_labels[i]:
        correct = correct + 1
print("all: ", correct / len(test_labels))

#################################
def func():
    dataset = get_dataset("churn.all")

    train_set, test_set, train_labels, test_labels = split_dataset(dataset)

    train_set, dictionary_list, bin_edges_list = correct_dataset1(train_set, int(sys.argv[1]))

    test_set = make_test_set1(test_set, dictionary_list, bin_edges_list)

    regr = DecisionTreeRegressor()
    #print(train_set)
    regr.fit(train_set, train_labels)
    #print(test_set)
    print("DecisionTreeRegressor: ", regr.score(test_set, test_labels))

    predict_labels = regr.predict(test_set)


    correct = 0
    for i in range(len(predict_labels)):
        if predict_labels[i] == test_labels[i]:
            correct = correct + 1
    print(correct / len(predict_labels))
