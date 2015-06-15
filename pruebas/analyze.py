# coding: utf-8
import time
import re

POSITIVE = 1

tiempo_inicial = time.time()

with open("labeledTrainData.tsv") as data_file:
    data_file.readline()
    words = {}
    line = True
    reviews = {}
    while line:
        line = data_file.readline()
        line_list = line.split('\t')
        try:
            review = line_list[2]
            review = re.sub("<.*?>", "", review)
            review = review.lower()
            # review = review.replace('"', '')
            sentiment = int(line_list[1])
            reviews[review] = sentiment
            #sacar html
            for i in review.split():
                values = words.get(i, [0,0])
                if sentiment == POSITIVE:
                    values[0] +=1
                else:
                    values[1] +=1
                words[i] = values
        except IndexError:
            break

print 'terminé de cargar los datos y procesar', time.time() - tiempo_inicial

def check_positive_negative(review, data):
    total = 0
    review = re.sub("<.*?>", "", review)
    review = review.lower()
    # review = review.replace('"', '')
    for w in review.split():
        proba =  float(data[w][0])/(float(data[w][0]) + float(data[w][1]))
        total += proba
    return 1 if total/float(len(review.split())) >= 0.5 else 0

good = bad = 0

for r in reviews.keys():
    analisis =  check_positive_negative(r, words)
    if analisis == reviews[r]:
        good += 1
    else:
        # print r, 'mi review: ', analisis, 'real: ', reviews[r]
        bad += 1

print 'terminé de analizar reviews', time.time() - tiempo_inicial
print 'good', good
print 'bad', bad