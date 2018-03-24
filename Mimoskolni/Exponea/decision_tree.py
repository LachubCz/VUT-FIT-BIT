"""
    methods for working with decision trees
"""
import math
import os
from collections import deque
import numpy as np
from sklearn import tree

def get_types(attribute):
    """
    method returns unique class values with their freqency
    """
    return np.unique(attribute, return_counts=True)

def get_entropy(attribute):
    """
    method returns entropy (float) from all class values
    """
    types = get_types(attribute)
    entropy = 0
    for i, item in enumerate(types[1]):
        entropy = entropy - (item / len(attribute)) * math.log((item / len(attribute)), 2)

    return entropy

def get_attribs_classes(dataset, number_of_attribute, attribute):
    """
    method returns array of class values of certain attribute value
    """
    attribute_array = np.array([])
    for i, item in enumerate(dataset): 
        if item[number_of_attribute] == attribute:
            attribute_array = np.append(attribute_array, item[len(dataset[0]) - 1])
    return attribute_array

def get_info_gain(dataset, number_of_attribute, class_entropy):
    """
    method returns information gain from certain atribute
    """
    types = get_types(get_attrib_array(dataset, number_of_attribute))
    length = len(dataset)
    info_gain = class_entropy
    att_class_entrop = np.array([])
    for i, item in enumerate(types[0]): 
        att_class_entrop = np.append(att_class_entrop, get_entropy(get_attribs_classes(dataset, number_of_attribute, item)))
    for i, item in enumerate(types[1]): 
        info_gain = info_gain - ((item / length) * att_class_entrop[i])
    return info_gain

def get_info_gain_array(dataset):
    """
    method returns information gain for all attributes
    """
    length = len(dataset[0]) - 1
    class_entropy = get_entropy(get_attrib_array(dataset, length)) 
    info_gain_array = np.array([])
    for number_of_attribute in range(length):
        info_gain = get_info_gain(dataset, number_of_attribute, class_entropy)
        info_gain_array = np.append(info_gain_array, info_gain)

    return info_gain_array

def get_attrib_array(dataset, number_of_attribute):
    """
    method returns array of attribute or class values
    """
    attribute_array = np.array([])
    for i, item in enumerate(dataset):
        attribute_array = np.append(attribute_array, item[number_of_attribute])
    return attribute_array

def get_num_of_attr(dataset):
    """
    method returns number of attributes
    """
    return (len(dataset[0]) - 1)

def get_entropies(dataset):
    """
    method returns array of entropies for all attribute's value
    """
    attr_entropy_array = np.array([])
    for number_of_attribute in range(get_num_of_attr(dataset)):
        attr_entropy_array = np.append(attr_entropy_array, get_entropy(get_attrib_array(dataset, number_of_attribute)))

    return attr_entropy_array

def tree_nodes(dataset, number_of_attribute):
    """
    method returns values of attribute and its entropies
    """
    types = get_types(get_attrib_array(dataset, number_of_attribute))
    att_class_entrop = np.array([])
    att_nodes = np.array([])
    for i, item in enumerate(types[0]): 
        att_nodes = np.append(att_nodes, item)
        att_class_entrop = np.append(att_class_entrop, get_entropy(get_attribs_classes(dataset, number_of_attribute, item)))
    return att_nodes, att_class_entrop

def remove_done_val(dataset, number_of_attribute, value):
    """
    method removes elements with certain value from specific attribute
    """
    elem_to_rem = np.array([])
    for i, item in enumerate(dataset):
        if item[number_of_attribute] == value:
            elem_to_rem = np.append(elem_to_rem, i)
    for i, elem_to_rem in enumerate(elem_to_rem):
        del dataset[int(elem_to_rem)-i]
    return dataset

def remove_done_attr(dataset, number_of_attribute):
    """
    method removes elements with certain value from specific attribute
    """
    for i, item in enumerate(dataset):
        dataset[i] = np.delete(item, number_of_attribute, 0)
    return dataset


def corr_discrete(dataset, number_of_attribute):
    #number_of_attributes = get_num_of_attr(dataset) + 1
    dictionary = dict() #for i in range(len(dataset[0]))]
    #for number_of_attribute in range(number_of_attributes):
    value_array = get_attrib_array(dataset, number_of_attribute)
    types = get_types(value_array)
        
    for e in range(len(types[0])):
        types[1][e] = e
        dictionary.update({types[0][e]: e})

    for e, item in enumerate(dataset):
        for i in range(len(types[0])):
            if dataset[e][number_of_attribute] == types[0][i]:
                dataset[e][number_of_attribute] = types[1][i]

    return dataset, dictionary

def get_labels(dataset):
    labels = np.array([])

    length = len(dataset[0]) - 1

    for i in range(len(dataset)):
        labels = np.append(labels, dataset[i][-1])
        dataset[i] = np.delete(dataset[i], length, 0)

    return dataset, labels

def get_tree(dataset, labels):
    clf = tree.DecisionTreeClassifier()
    clf = clf.fit(dataset, labels)

    return clf

def corr_continuous(dataset, number_of_attribute, bins):
    values = get_attrib_array(dataset, number_of_attribute)
    hist, bin_edges = np.histogram(values, bins=bins)
    bin_edges = np.insert(bin_edges, 0, float("-inf"))
    bin_edges = np.append(bin_edges, float("inf"))

    for i in range(len(values)):
        for e in range(len(bin_edges) - 1):
            if values[i] >= bin_edges[e] and values[i] < float(bin_edges[e+1]):
                values[i] = e

    for i, elm in enumerate(dataset):
        elm[number_of_attribute] = int(values[i])

    return dataset, bin_edges

def correct_dataset(dataset):
    dictionary_list = list()
    """
    dataset, dictionary = corr_discrete(dataset, 0)
    dictionary_list.append(dictionary)
    dataset, dictionary = corr_discrete(dataset, 3)
    dictionary_list.append(dictionary)
    dataset, dictionary = corr_discrete(dataset, 4)
    dictionary_list.append(dictionary)
    """
    dataset, dictionary = corr_discrete(dataset, 2)
    dictionary_list.append(dictionary)
    dataset, dictionary = corr_discrete(dataset, 3)
    dictionary_list.append(dictionary)
    
    return dataset, dictionary_list

def split_dataset(dataset):
    train_set = deque()
    test_set = deque()
    train_labels = np.array([])
    test_labels = np.array([])

    for i in range(int(len(dataset) / 2)):
        train_set.append(dataset[i])

    train_set, train_labels = get_labels(train_set)

    for i in range(int(len(dataset) / 2), len(dataset)):
        test_set.append(dataset[i])

    test_set, test_labels = get_labels(test_set)

    return train_set, test_set, train_labels, test_labels

def make_dict_corr(dataset, dictionary_list, dic_num, data_num):
    for i in range(len(dataset)):
        try:
            dataset[i][data_num] = dictionary_list[dic_num][dataset[i][data_num]]
        except:
            dataset[i][data_num] = len(dictionary_list[dic_num])
    return dataset


def make_array_corr(dataset, bin_edges_list, be_num, data_num):
    for i in range(len(dataset)):
        for e in range(len(bin_edges_list[be_num]) - 1):
            if dataset[i][data_num] >= float(bin_edges_list[be_num][e]) and dataset[i][data_num] < float(bin_edges_list[be_num][e+1]):
               dataset[i][data_num] = e
    return dataset

def make_test_set(test_set, dictionary_list):
    #test_set = make_dict_corr(test_set, dictionary_list, 0, 0)
    #test_set = make_dict_corr(test_set, dictionary_list, 1, 3)
    #test_set = make_dict_corr(test_set, dictionary_list, 2, 4)

    test_set = make_dict_corr(test_set, dictionary_list, 0, 2)
    test_set = make_dict_corr(test_set, dictionary_list, 1, 3)

    return test_set

def export_graphviz(dec_tree):
    labels = np.array([ "account length", "area code", 
                      "international plan", "voice mail plan", "number vmail messages", 
                      "total day minutes", "total day calls", "total day charge", 
                      "total eve minutes", "total eve calls", "total eve charge", 
                      "total night minutes", "total night calls", "total night charge", 
                      "total intl minutes", "total intl calls", "total intl charge", 
                      "number customer service calls"])
    dotfile = open(os.getcwd()+"\\" + "model.dot", "w")
    tree.export_graphviz(dec_tree, out_file = dotfile, feature_names = labels)
    dotfile.close()

def make_test_set1(test_set, dictionary_list):
    test_set = make_dict_corr(test_set, dictionary_list, 0, 0)
    test_set = make_dict_corr(test_set, dictionary_list, 1, 3)
    test_set = make_dict_corr(test_set, dictionary_list, 2, 4)
    test_set = make_dict_corr(test_set, dictionary_list, 3, 5)


    return test_set

def correct_dataset1(dataset):
    dictionary_list = list()
    dataset, dictionary = corr_discrete(dataset, 0)
    dictionary_list.append(dictionary)
    dataset, dictionary = corr_discrete(dataset, 3)
    dictionary_list.append(dictionary)
    dataset, dictionary = corr_discrete(dataset, 4)
    dictionary_list.append(dictionary)
    dataset, dictionary = corr_discrete(dataset, 5)
    dictionary_list.append(dictionary)


    return dataset, dictionary_list