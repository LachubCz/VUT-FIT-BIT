"""
    File name: data_worker.py
    Author: Petr Buchal
    Date created: 23/03/2013
    Date last modified: 26/03/2013
    Python Version: 3.6
    Description:
        methods for working with dataset
"""
import math
from collections import deque
import numpy as np

def get_types(attribute):
    """
    method returns unique attribute values with their freqency
    """
    return np.unique(attribute, return_counts=True)

def get_entropy(attribute):
    """
    [unused] method returns entropy for all attribute values
    """
    types = get_types(attribute)
    entropy = 0
    for i, item in enumerate(types[1]):
        entropy = entropy - (item / len(attribute)) * math.log((item / len(attribute)), 2)

    return entropy

def get_attribs_labels(dataset, number_of_attribute, attribute):
    """
    [unused] method returns array of label values of certain attribute value
    """
    attribute_array = np.array([])
    for i, item in enumerate(dataset):
        if item[number_of_attribute] == attribute:
            attribute_array = np.append(attribute_array, item[len(dataset[0]) - 1])
    return attribute_array

def get_attrib_array(dataset, number_of_attribute):
    """
    method returns array of attribute values
    """
    attribute_array = np.array([])
    for i, item in enumerate(dataset):
        attribute_array = np.append(attribute_array, item[number_of_attribute])
    return attribute_array

def get_info_gain(dataset, number_of_attribute, class_entropy):
    """
    [unused] method returns information gain from certain attribute
    """
    types = get_types(get_attrib_array(dataset, number_of_attribute))
    length = len(dataset)
    info_gain = class_entropy
    att_class_entrop = np.array([])
    for i, item in enumerate(types[0]):
        att_class_entrop = np.append(att_class_entrop, get_entropy(get_attribs_labels(dataset, number_of_attribute, item)))
    for i, item in enumerate(types[1]):
        info_gain = info_gain - ((item / length) * att_class_entrop[i])
    return info_gain

def get_info_gain_array(dataset):
    """
    [unused] method returns information gain for all attributes
    """
    length = len(dataset[0]) - 1
    class_entropy = get_entropy(get_attrib_array(dataset, length))
    info_gain_array = np.array([])
    for number_of_attribute in range(length):
        info_gain = get_info_gain(dataset, number_of_attribute, class_entropy)
        info_gain_array = np.append(info_gain_array, info_gain)

    return info_gain_array

def get_num_of_attr(dataset):
    """
    [unused] method returns number of attributes (dataset - labels)
    """
    return len(dataset[0]) - 1

def get_entropies(dataset):
    """
    [unused] method returns array of entropies for all attribute's value
    """
    attr_entropy_array = np.array([])
    for number_of_attribute in range(get_num_of_attr(dataset)):
        attr_entropy_array = np.append(attr_entropy_array, get_entropy(get_attrib_array(dataset, number_of_attribute)))

    return attr_entropy_array

def rem_elm_w_val(dataset, number_of_attribute, value):
    """
    [unused] method removes elements with certain value from specific attribute from dataset
    """
    elem_to_rem = np.array([])
    for i, item in enumerate(dataset):
        if item[number_of_attribute] == value:
            elem_to_rem = np.append(elem_to_rem, i)
    for i, elem_to_rem in enumerate(elem_to_rem):
        del dataset[int(elem_to_rem)-i]
    return dataset

def remove_attribute(dataset, number_of_attribute):
    """
    method removes attribute from dataset
    """
    for i, item in enumerate(dataset):
        dataset[i] = np.delete(item, number_of_attribute, 0)
    return dataset

def get_labels(dataset):
    """
    method removes last attribute from dataset assuming it's a label
    and returns it together with dataset
    """
    labels = np.array([])

    length = len(dataset[0]) - 1

    for i in range(len(dataset)):
        labels = np.append(labels, dataset[i][-1])
        dataset[i] = np.delete(dataset[i], length, 0)

    return dataset, labels

def split_dataset(dataset):
    """
    method splits data dataset into training part and testing part + labels
    """
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

def str_to_category(dataset, number_of_attribute):
    """
    method transforms string values into numeric values
    and creates dictionary
    """
    dictionary = dict()
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

def num_to_range_cat(dataset, number_of_attribute, bins):
    """
    [unused] method divides numerical values into range categories
    and creates edges list
    """
    values = get_attrib_array(dataset, number_of_attribute)
    _, bin_edges = np.histogram(values, bins=bins)
    bin_edges = np.insert(bin_edges, 0, float("-inf"))
    bin_edges = np.append(bin_edges, float("inf"))

    for i in range(len(values)):
        for e in range(len(bin_edges) - 1):
            if values[i] >= bin_edges[e] and values[i] < float(bin_edges[e+1]):
                values[i] = e

    for i, elm in enumerate(dataset):
        elm[number_of_attribute] = int(values[i])

    return dataset, bin_edges

def dict_category(dataset, dictionaries_list, dic_num, data_num):
    """
    method transforms string values into numeric values
    according to dictionary
    """
    for i in range(len(dataset)):
        try:
            dataset[i][data_num] = dictionaries_list[dic_num][dataset[i][data_num]]
        except:
            dataset[i][data_num] = len(dictionaries_list[dic_num])
    return dataset


def range_category(dataset, bin_edges_list, be_num, data_num):
    """
    [unused] method divides numerical values into range categories
    in bin_edges_list
    """
    for i in range(len(dataset)):
        for e in range(len(bin_edges_list[be_num]) - 1):
            if dataset[i][data_num] >= float(bin_edges_list[be_num][e]) and dataset[i][data_num] < float(bin_edges_list[be_num][e+1]):
                dataset[i][data_num] = e
    return dataset

def transform_to_dict(dataset):
    """
    method transforms input data and save pattern into dictionary
    """
    dictionaries_list = list()
    dataset, dictionary = str_to_category(dataset, 2)
    dictionaries_list.append(dictionary)
    dataset, dictionary = str_to_category(dataset, 3)
    dictionaries_list.append(dictionary)

    return dataset, dictionaries_list

def transform_from_dict(input_data, dictionaries_list):
    """
    method transforms input data according to dictionaries list
    """
    input_data = dict_category(input_data, dictionaries_list, 0, 2)
    input_data = dict_category(input_data, dictionaries_list, 1, 3)

    return input_data
