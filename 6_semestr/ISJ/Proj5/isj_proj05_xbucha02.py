#!/usr/bin/env python3
"""
    ISJ - Skriptovaci jazyky
    Projekt 5
    xbucha02, Petr Buchal
"""

class Polynomial:
    """
    docstring
    """
    def __init__(self,*arguments, x0=0, x1=0, x2=0, x3=0, x4=0, x5=0, x6=0, x7=0, x8=0, x9=0):
        """
        docstring
        """
        self.args = []
        self.args.extend([x0, x1, x2, x3, x4, x5, x6, x7, x8, x9])

        if len(arguments) != 0:
            if len(arguments) == 1:
                for i, item in enumerate(arguments[0]):
                    self.args[i] = item
                    if i == 9:
                        break
            else:
                for i, item in enumerate(arguments):
                    self.args[i] = item
                    if i == 9:
                        break

    def __str__(self):
        """
        docstring
        """
        expression = ""

        for i, item in enumerate (self.args):
            if item != 0:
                break
            if i == 9:
                return "0"

        for i, item in enumerate (self.args):
            if item != 0:
                if i > 1:
                    if item == 1 or item == -1:
                        if item > 0:
                            expression =  " + x^" + str(i) + expression
                        else:
                            expression =  " - x^" + str(abs(i)) + expression
                    else:
                        if item > 0:
                            expression =  " + " + str(item) + "x^" + str(i) + expression
                        else:
                            expression =  " - " + str(abs(item)) + "x^" + str(i) + expression
                elif i == 1:
                    if item == 1 or item == -1:
                        if item > 0:
                            expression =  " + x" + expression
                        else:
                            expression =  " - x" + expression
                    else:
                        if item > 0:
                            expression =  " + " + str(item) + "x" + expression
                        else:
                            expression =  " - " + str(abs(item)) + "x" + expression
                elif i == 0:
                    if item > 0:
                        expression =  " + " + str(item) + expression
                    else:
                        expression =  " - " + str(abs(item)) + expression

        if expression[0] == " ":
            expression = expression[1:]
        if expression[0] == "+":
            expression = expression[1:]
        if expression[0] == " ":
            expression = expression[1:]

        return expression
        
    def __add__(self, other):
        """
        docstring
        """
        new_lst = [];
        for i, j in zip(self.args, other.args):
            new_lst.append(i+j)
        return Polynomial(new_lst)

    def __eq__(self, other):
        """
        docstring
        """
        for i, item in enumerate(self.args):
            if item != other.args[i]:
                return 0
        return 1
    
    def power(self,pol1, pol2):
        """
        docstring
        """
        matrix = []

        for i in range(len(pol2)):
            matrix.append([])
        for i, item in enumerate(pol2):
            for e, elem in enumerate(pol1):
                matrix[i].append(item * elem)

        result = []
        for i in range((len(matrix[0])*2)-1):
            result.append(0)

        for i, item in enumerate(matrix):
            for e, elem in enumerate(item):
                result[i + e] = result[i + e] + elem
        
        return result
    
    def __pow__(self, power):
        """
        docstring
        """
        array = []
        array.extend(self.args)
        if power == 0:
            return Polynomial(x0=1)
        elif power == 1:
            return Polynomial(self.args)
        for i in range(power-1):
            array = self.power(array, self.args)
        
        return Polynomial(array)
    
    def derivative(self):
        """
        docstring
        """
        array = []
        array.extend(self.args)
        for i, item in enumerate (array):
            if i != 0:
                array[i-1] = item * i
            else:
                array[i] = 0

        return Polynomial(array)
                
    def at_value(self, x, *y):
        """
        docstring
        """
        x_result = 0
        y_result = 0

        for i, item in enumerate (self.args):
            x_result = x_result + item * (x ** i)
        if len(y) is 0:
            return x_result
        else:
            for i, item in enumerate (self.args):
                y_result = y_result + item * (y[0] ** i)
            return (y_result - x_result)

def test():
    assert str(Polynomial(0,1,0,-1,4,-2,0,1,3,0)) == "3x^8 + x^7 - 2x^5 + 4x^4 - x^3 + x"
    assert str(Polynomial([-5,1,0,-1,4,-2,0,1,3,0])) == "3x^8 + x^7 - 2x^5 + 4x^4 - x^3 + x - 5"
    assert str(Polynomial(x7=1, x4=4, x8=3, x9=0, x0=0, x5=-2, x3= -1, x1=1)) == "3x^8 + x^7 - 2x^5 + 4x^4 - x^3 + x"
    assert str(Polynomial(x2=0)) == "0"
    assert str(Polynomial(x0=0)) == "0"
    assert Polynomial(x0=2, x1=0, x3=0, x2=3) == Polynomial(2,0,3)
    assert Polynomial(x2=0) == Polynomial(x0=0)
    assert str(Polynomial(x0=1)+Polynomial(x1=1)) == "x + 1"
    assert str(Polynomial([-1,1,1,0])+Polynomial(1,-1,1)) == "2x^2"
    pol1 = Polynomial(x2=3, x0=1)
    pol2 = Polynomial(x1=1, x3=0)
    assert str(pol1+pol2) == "3x^2 + x + 1"
    assert str(pol1+pol2) == "3x^2 + x + 1"
    assert str(Polynomial(x0=-1,x1=1)**1) == "x - 1"
    assert str(Polynomial(x0=-1,x1=1)**2) == "x^2 - 2x + 1"
    pol3 = Polynomial(x0=-1,x1=1)
    assert str(pol3**4) == "x^4 - 4x^3 + 6x^2 - 4x + 1"
    assert str(pol3**4) == "x^4 - 4x^3 + 6x^2 - 4x + 1"
    assert str(Polynomial(x0=2).derivative()) == "0"
    assert str(Polynomial(x3=2,x1=3,x0=2).derivative()) == "6x^2 + 3"
    assert str(Polynomial(x3=2,x1=3,x0=2).derivative().derivative()) == "12x"
    pol4 = Polynomial(x3=2,x1=3,x0=2)
    assert str(pol4.derivative()) == "6x^2 + 3"
    assert str(pol4.derivative()) == "6x^2 + 3"
    assert Polynomial(-2,3,4,-5).at_value(0) == -2
    assert Polynomial(x2=3, x0=-1, x1=-2).at_value(3) == 20
    assert Polynomial(x2=3, x0=-1, x1=-2).at_value(3,5) == 44
    pol5 = Polynomial([1,0,-2])
    assert pol5.at_value(-2.4) == -10.52
    assert pol5.at_value(-2.4) == -10.52
    assert pol5.at_value(-1,3.6) == -23.92
    assert pol5.at_value(-1,3.6) == -23.92

if __name__ == '__main__':
    test()
