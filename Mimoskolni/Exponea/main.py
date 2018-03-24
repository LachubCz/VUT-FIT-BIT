import random
import sys
import os
import numpy as np
from tools import get_dataset, print_custmr_info, print_smllrdata_info
from decision_tree import *
from sklearn import tree

dataset = get_dataset("churn.all")

dataset = remove_done_attr(dataset, 3)



train_set, test_set, train_labels, test_labels = split_dataset(dataset)
"""
for i in range(20):
    alf = get_types(get_attrib_array(train_set, i))
    print(len(alf[0]))
"""
train_set, dictionary_list, bin_edges_list = correct_dataset(train_set, 8, int(sys.argv[1]))

print( dictionary_list)

dec_tree = get_tree(train_set, train_labels)

test_set = make_test_set(test_set, dictionary_list, bin_edges_list)

predict_labels = dec_tree.predict(test_set)

print("score: ", estimate_score(predict_labels, test_labels))

export_graphviz(dec_tree)