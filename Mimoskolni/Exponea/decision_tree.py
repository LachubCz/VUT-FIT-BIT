"""
    methods for working with decision trees
"""
import math
import numpy as np

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

def correct_dataset(dataset):
    number_of_attributes = get_num_of_attr(dataset) + 1

    for number_of_attribute in range(number_of_attributes):
        value_array = get_attrib_array(dataset, number_of_attribute)
        types = get_types(value_array)
        
        for e in range(len(types[0])):
            types[1][e] = e

        for e, item in enumerate(dataset):
            for i in range(len(types[0])):
                if dataset[e][number_of_attribute] == types[0][i]:
                    dataset[e][number_of_attribute] = types[1][i]

    labels = np.array([])
    
    for i in range(len(dataset)):
        labels = np.append(labels, dataset[i][-1])
        dataset[i] = np.delete(dataset[i], len(dataset[0])-1, 0)

    return dataset, labels