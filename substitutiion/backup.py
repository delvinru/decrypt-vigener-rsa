#!/usr/bin/env python3

from collections import OrderedDict

referens = {
    'о' : 10.97,
    'е' : 8.45,
    'а' : 8.01,
    'и' : 7.35,
    'н' : 6.70,
    'т' : 6.26,
    'с' : 5.47,
    'р' : 4.73,
    'в' : 4.54,
    'л' : 4.40,
    'к' : 3.49,
    'м' : 3.21,
    'д' : 2.98,
    'п' : 2.81,
    'у' : 2.62,
    'я' : 2.01,
    'ы' : 1.90,
    'ь' : 1.74,
    'г' : 1.70,
    'з' : 1.65,
    'б' : 1.59,
    'ч' : 1.44,
    'й' : 1.21,
    'ч' : 0.97,
    'ж' : 0.94,
    'ш' : 0.73,
    'ю' : 0.64,
    'ц' : 0.48,
    'щ' : 0.36,
    'э' : 0.32,
    'ф' : 0.26,
    'ъ' : 0.04,
    'ё' : 0.01,
}

coefficients = dict() 

def attack(source, output):
    # Читаем посимвольно и заносим данные в таблицу
    letters_counter = 0
    while( True ):
        letter = source.read(1)
        if not letter:
            break
        if letter in referens:
            letters_counter += 1
            if letter not in coefficients:
                coefficients[letter] = 1
            else:
                coefficients[letter] += 1
    for letter in coefficients:
        coefficients[letter] = (coefficients[letter] / letters_counter) * 100
    
    print("Количество букв: %d\n" % letters_counter)
    # Сортировка массива коэффицентов по убыванию, чтобы сделать биективное отображение
   
    temp = sorted(coefficients.items(), key=lambda value: value[1], reverse=True)


    referens_key = list(referens.keys())
    substituion = dict()

    for i in range(len(temp)):
        print("%c : %.3f" % (temp[i][0], temp[i][1]))
        substituion[temp[i][0]] = referens_key[i]
    
    print("Candidate to permutation. Read first 100 letter to compare")

    chance = 0.2

    change_sub = dict()
    source.seek(0,0)

    first_100 = source.read(100)

    
    # Замена в исходном файле 
    source.seek(0,0)
    while( True ):
        letter = source.read(1).lower()
        if not letter:
            break
        if letter in substituion:
            output.write(substituion[letter])
        else:
            output.write(letter)
    print('complete')

if __name__ == '__main__':
    source = open("source.txt", "r")
    output = open("decoded.txt", "w")
    attack(source, output)
    source.close()
    output.close()

