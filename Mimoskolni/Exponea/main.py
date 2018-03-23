import numpy as np
from tools import get_dataset, print_custmr_info, print_smllrdata_info
from decision_tree import get_entropy, get_types, get_info_gain_array

dataset = get_dataset("small_dataset.txt")

print_smllrdata_info(dataset[0])
print(dataset)
length = len(dataset[0])
print(length)

print(get_info_gain_array(dataset))

def get_class(attribute, number_of_attribute, dataset):
    attribute_array = np.array([])
    for i, item in enumerate(dataset): 
        if item[number_of_attribute] == attribute:
            attribute_array = np.append(attribute_array, item[len(dataset[0])-1])
    return attribute_array

#print(get_class("Male",0, dataset))

def get_entropies():
    for x in xrange(1,10):
        pass

    attribute_array = np.array([])
    for i, item in enumerate(dataset):
        attribute_array = np.append(attribute_array, item[4])

    get_entropy(attribute_array)
#print(attribute_array)

