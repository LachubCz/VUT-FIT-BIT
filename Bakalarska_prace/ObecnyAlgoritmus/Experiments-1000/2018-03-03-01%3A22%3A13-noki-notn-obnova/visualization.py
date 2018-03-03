import copy
import matplotlib.pyplot as plt

def analysis(scores, episodesList):
    score1 = copy.copy(scores)
    score2 = copy.copy(scores)
    score3 = copy.copy(scores)

    for i in range (len(scores)):
        if i > 1 and i < (len(scores)-2):
            score1[i] = (scores[i - 2] + scores[i - 1] + scores[i] + scores[i + 1] + scores[i + 2])/5

    for i in range (len(scores)):
        if i > 4 and i < (len(scores)-5):
            score2[i] += scores[i - 5] + scores[i - 4] + scores[i - 3] + scores[i - 2] + scores[i - 1]
            score2[i] += scores[i + 5] + scores[i + 4] + scores[i + 3] + scores[i + 2] + scores[i + 1]
            score2[i] = score2[i]/11

    for i in range (len(scores)):
        if i > 49 and i < (len(scores) - 50):
            for e in range (1,50):
                score3[i] += scores[i - e] + scores[i + e] 
            score3[i] = score3[i]/101      

    plt.plot(episodesList, scores, 'ro')
    plt.ylabel("Score")
    plt.xlabel("Episodes")
    plt.title("Vysledky")
    plt.show()

    plt.plot(scores)
    plt.ylabel("Score")
    plt.xlabel("Episodes")
    plt.title("Funkce interpolace vysledku")
    plt.show()

    plt.plot(score1)
    plt.ylabel("Score")
    plt.xlabel("Episodes")
    plt.title("Funkce interpolace vysledku (filtr - prumer 5-ti prvku)")
    plt.show()

    plt.plot(score2)
    plt.ylabel("Score")
    plt.xlabel("Episodes")
    plt.title("Funkce interpolace vysledku  (filtr - prumer 11-cti prvku)")
    plt.show()

    plt.plot(score3)
    plt.ylabel("Score")
    plt.xlabel("Episodes")
    plt.title("Funkce interpolace vysledku  (filtr - prumer 101 prvku)")
    plt.show()