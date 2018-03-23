import numpy as np
from tools import get_dataset, print_custmr_info, print_smllrdata_info
from decision_tree import get_entropy, get_types, get_info_gain_array, get_entropies, tree_nodes, remove_done_val, get_attrib_array, remove_done_attr, correct_dataset
from sklearn import tree

def func(dataset, labels):
    X = dataset
    Y = labels
    clf = tree.DecisionTreeClassifier()
    clf = clf.fit(X, Y)

    return clf


dataset = get_dataset("small_dataset.txt")

testset = get_dataset("test_dataset.txt")
#print(testset)

dataset, labels, dictionary = correct_dataset(dataset)
print(dataset)
dataset1, labels1, dictionary1 = correct_dataset(testset)
print(dictionary1)

clf = func(dataset, labels)
#print(dataset1)
print(clf.predict(dataset1))


def func():
    tree = np.ndarray(shape=(len(dataset[0]), 2), dtype=object)
    print(tree)
    tree[0][1] = 1
    print(tree)

    array =  np.zeros(shape=(2,2))
    #array = np.append(array, ("alement" : "as"))
    array = np.append(array, np.array(["alem"]))
    print(array)
    #print_smllrdata_info(dataset[0])

    def train_tree():
        global dataset
        info_gain = get_info_gain_array(dataset)

        first_selected = np.argmax(info_gain)

        dataset = remove_done_attr(dataset, first_selected)
        train_tree(dataset)

    #dataset = remove_done_val(dataset, first_selected, "Standart")


    def search_tree(element):
        pass


    #print(dataset)
    #print(get_attrib_array(dataset, first_selected))
