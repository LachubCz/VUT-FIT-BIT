import glob
import os
import subprocess
import re
from collections import deque

def get_eps_count(string):
    regex = re.compile('(?<=Task solved after )\w*')
    value = regex.findall(string)
    if value == []:
        return 2499
    else:
        return int(regex.findall(string)[0])

def get_val_count(string):
    regex = re.compile('(?<=Task solved after 100 episodes with score )[-+]?[0-9]*\.?[0-9]*')
    value = regex.findall(string)
    if value == []:
        return 500
    else:
        return float(regex.findall(string)[0])

def get_last_line(filename):
    with open(filename) as f:
        data = f.readlines()
    lastline = data[-1]
    tail = data[-1:]
    return tail[0]

def insert_nw_val(dictionary, testname, new_value):
    if testname in dictionary:
        values = dictionary[testname]
    else:
        values = []

    values.append(new_value)
    dictionary.update({testname: values})
    return dictionary

def get_results(dictionary):
    keys = list(dictionary.keys())
    results = dict()

    for i, item in enumerate(keys):
        values = dictionary[item]
        value = sum(values) / len(values)
        results.update({item: value})

    return results

def get_eps_results():
    cwd = os.getcwd()
    dictionary = dict()

    for filename in glob.iglob('test_006_epsilon/**/*.out', recursive=True):
        testname = filename.split('\\')[-3]
        line = get_last_line(filename)
        new_value = get_eps_count(line)

        insert_nw_val(dictionary, testname, new_value)

    results = get_results(dictionary)

    for key in sorted(results):
        print ("{}: {}" .format(key, results[key]))

def get_val_results():
    cwd = os.getcwd()
    dictionary = dict()

    for filename in glob.iglob('test_006_epsilon/**/*.out', recursive=True):
        testname = filename.split('\\')[-3]
        line = get_last_line(filename)
        new_value = get_val_count(line)

        insert_nw_val(dictionary, testname, new_value)

    results = get_results(dictionary)

    for key in sorted(results):
        print ("{}: {}" .format(key, results[key]))

def get_data():    
    print("______________________")
    print("Number of episodes: ")
    print("______________________")
    get_eps_results()
    print("______________________")
    print("Score: ")
    print("______________________")
    get_val_results()

def parse_table(filename):
    dataset = deque()

    file = open(os.getcwd()+"\\" + filename, "r")

    while True:
        customer = file.readline()
        if customer == "":
            return dataset

        dataset.append(customer)

def get_min(time_str):
    h, m, s = time_str.split(':')
    return float(h) * 60 + float(m) + float(s) / 60

def get_times():
    dataset = parse_table("table.txt")
    for i, item in enumerate(dataset):
        dataset[i] = re.sub('\s+', ' ', item).strip()

    dictionary = dict()

    for i, item in enumerate(dataset):
        testname = item.split(' ')[-13]
        new_value = item.split(' ')[-10]
        new_value = get_min(new_value)

        insert_nw_val(dictionary, testname, new_value)

    dictionary = get_results(dictionary)

    for key in sorted(dictionary):
        print ("{}: {}" .format(key, dictionary[key]))