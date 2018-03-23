import math
import numpy as np

def get_types(attribute):
    return np.unique(attribute, return_counts=True)

def get_entropy(attribute):
    types = get_types(attribute)
    entropy = 0
    for i, item in enumerate(types[1]):
        entropy = entropy - (item / len(attribute)) * math.log((item / len(attribute)), 2)

    return entropy

def get_attribs_classes(dataset, number_of_attribute, attribute):
    attribute_array = np.array([])
    for i, item in enumerate(dataset): 
        if item[number_of_attribute] == attribute:
            attribute_array = np.append(attribute_array, item[len(dataset[0])-1])
    return attribute_array

def get_info_gain(dataset, number_of_attribute, class_entropy):
    types = get_types(get_attrib_array(dataset, number_of_attribute))

    length = len(dataset)

    info_gain = class_entropy
    att_class_entrop = np.array([])
    for i, item in enumerate(types[0]): 
        att_class_entrop = np.append(att_class_entrop, get_entropy(get_attribs_classes(dataset, number_of_attribute, item)))
    for i, item in enumerate(types[1]): 
        info_gain = info_gain - ((item/length) * att_class_entrop[i])
    return info_gain

def get_info_gain_array(dataset):
    length = len(dataset[0]) - 1
    class_entropy = get_entropy(get_attrib_array(dataset, length)) 
    info_gain_array = np.array([])
    for number_of_attribute in range(length):
        info_gain = get_info_gain(dataset, number_of_attribute, class_entropy)
        info_gain_array = np.append(info_gain_array, info_gain)

    return info_gain_array

def get_attrib_array(dataset, number_of_attribute):
    attribute_array = np.array([])
    for i, item in enumerate(dataset):
        attribute_array = np.append(attribute_array, item[number_of_attribute])
    return attribute_array