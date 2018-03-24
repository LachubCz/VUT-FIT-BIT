import random
import sys
import numpy as np
from tools import get_dataset, print_custmr_info, print_smllrdata_info
from decision_tree import *

dataset = get_dataset("churn.all")

train_set, test_set, train_labels, test_labels = split_dataset(dataset)

train_set, dictionary_list, bin_edges_list = correct_dataset(train_set, int(sys.argv[1]))

dec_tree = get_tree(train_set, train_labels)

test_set = make_dict_corr(test_set, dictionary_list, 0, 0)
test_set = make_array_corr(test_set, bin_edges_list, 0, 1)
test_set = make_array_corr(test_set, bin_edges_list, 1, 2)
test_set = make_dict_corr(test_set, dictionary_list, 1, 3)
test_set = make_dict_corr(test_set, dictionary_list, 2, 4)
test_set = make_dict_corr(test_set, dictionary_list, 3, 5)
test_set = make_array_corr(test_set, bin_edges_list, 2, 6)
test_set = make_array_corr(test_set, bin_edges_list, 3, 7)
test_set = make_array_corr(test_set, bin_edges_list, 4, 8)
test_set = make_array_corr(test_set, bin_edges_list, 5, 9)
test_set = make_array_corr(test_set, bin_edges_list, 6, 10)
test_set = make_array_corr(test_set, bin_edges_list, 7, 11)
test_set = make_array_corr(test_set, bin_edges_list, 8, 12)
test_set = make_array_corr(test_set, bin_edges_list, 9, 13)
test_set = make_array_corr(test_set, bin_edges_list, 10, 14)
test_set = make_array_corr(test_set, bin_edges_list, 11, 15)
test_set = make_array_corr(test_set, bin_edges_list, 12, 16)
test_set = make_array_corr(test_set, bin_edges_list, 13, 17)
test_set = make_array_corr(test_set, bin_edges_list, 14, 18)
test_set = make_array_corr(test_set, bin_edges_list, 15, 19)

predict_labels = dec_tree.predict(test_set)

def estimate_score(predict_labels, true_labels):
    correct = 0
    for i in range(len(predict_labels)):
        if predict_labels[i] == true_labels[i]:
            correct = correct + 1

    return correct / len(predict_labels)

print("bins: ", int(sys.argv[1]), "score: ", estimate_score(predict_labels, test_labels))