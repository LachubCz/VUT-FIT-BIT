"""
    File name: models.py
    Author: Petr Buchal
    Date created: 23/03/2013
    Date last modified: 26/03/2013
    Python Version: 3.6
    Description:
        methods for file parsing and dataset making
"""
import os
from collections import deque
import numpy as np

def corr_type(string):
    """
    method casts proper type from string
    """
    if string.isdigit():
        return int(string)
    elif string.replace(".", "", 1).isdigit():
        return float(string)
    elif string == "False.\n" or string == "False.":
        return False
    elif string == "True.\n":
        return True
    elif string == "None.\n":
        return None
    else:
        return string[:-1] if string[-1] == "\n" else string

def split_custmr(customer):
    """
    method parses string "customer" and return numpy array of it's splitted parts
    """
    return np.array([corr_type(string) for string in customer.split(", ")], dtype=object)

def get_dataset(filename):
    """
    method opens file, processes it line by line and returns dataset in deque
    """
    dataset = deque()

    file = open(os.getcwd()+"\\" + filename, "r")

    while True:
        customer = file.readline()
        if customer == "":
            return dataset

        dataset.append(split_custmr(customer))

def print_custmr_info(customer):
    """
    method prints customer data
    """
    print("state:                 {} [discrete]      {}" .format(customer[0], type(customer[0])))
    print("account length:        {} [continuous]    {}".format(customer[1], type(customer[1])))
    print("area code:             {} [continuous]    {}" .format(customer[2], type(customer[2])))
    print("phone number:          {} [discrete]      {}" .format(customer[3], type(customer[3])))
    print("international plan:    {} [discrete]      {}" .format(customer[4], type(customer[4])))
    print("voice mail plan:       {} [discrete]      {}" .format(customer[5], type(customer[5])))
    print("number vmail messages: {} [continuous]    {}" .format(customer[6], type(customer[6])))
    print("total day minutes:     {} [continuous]    {}" .format(customer[7], type(customer[7])))
    print("total day calls:       {} [continuous]    {}" .format(customer[8], type(customer[8])))
    print("total day charge:      {} [continuous]    {}" .format(customer[9], type(customer[9])))
    print("total eve minutes:     {} [continuous]    {}" .format(customer[10], type(customer[10])))
    print("total eve calls:       {} [continuous]    {}" .format(customer[11], type(customer[11])))
    print("total eve charge:      {} [continuous]    {}" .format(customer[12], type(customer[12])))
    print("total night minutes:   {} [continuous]    {}" .format(customer[13], type(customer[13])))
    print("total night calls:     {} [continuous]    {}" .format(customer[14], type(customer[14])))
    print("total night charge:    {} [continuous]    {}" .format(customer[15], type(customer[15])))
    print("total intl minutes:    {} [continuous]    {}" .format(customer[16], type(customer[16])))
    print("total intl calls:      {} [continuous]    {}" .format(customer[17], type(customer[17])))
    print("total intl charge:     {} [continuous]    {}" .format(customer[18], type(customer[18])))
    print("number customer service calls: {} [continuous]    {}" .format(customer[19], type(customer[19])))
    print("state: {}" .format(customer[20]))
