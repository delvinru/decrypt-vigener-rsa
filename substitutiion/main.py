#!/usr/bin/env python3
from collections import OrderedDict
from words import *

# Частота встречаемости букв расположенных в порядке убывания
referens = ['о','е','а','и','н','т','с','р','в','л','к','м','д','п','у','я','ы','ь','г','з','б','ч','й','х','ж','ш','ю','ц','щ','э','ф','ъ','ё']
# referens = ['о','е','а','и','н','т','с','л','р','в','к','м','д','п','у','я','ы','ь','з','г','б','ч','й','ж','х','ш','ю','э','щ','ц','ф','ъ','ё']

# bi_reference = ['ст','но','ен','то','на','ов','ни','ра','во','ко']

def attack(source, output):
    # Словарь, в который будет записывать частота встречаемости буквы, количество этой буквы и предполагаемая подстановка
    # Пример данного словаря
    # sub = {
    #     'value':{
    #         'freq': float,
    #         'count': int,
    #         'sub': char
    #     }
    # }

    substitution = dict()
    print("Начинаем анализ...:")
    # Первичный анализ текста, получили первичную подстановку
    substitution = analyze(source, substitution)
    pprint(substitution)

    sub_keys = list(substitution.keys())
    for key in sub_keys:
        print("%s : %.2f : %s" % (key, substitution[key]["freq"], substitution[key]["sub"]))
   
    bi_sub = dict()
    # Рассмотрим биграммы для первой буквы часто встречающейся буквы
    bi_sub = analyze_bigramms(source, bi_sub, substitution)
    # print(bi_sub)
    # Теперь рассмотрим триграммы
    three_sub = dict()
    three_sub = analyze_threegramms(source, three_sub, substitution)
    # print(three_sub)

    candidate = dict()
    chance = 0.85
    for i in range(len(sub_keys) - 1):
        if substitution[sub_keys[i]]["freq"] - chance < substitution[sub_keys[i+1]]["freq"]:
            candidate.update({sub_keys[i]: sub_keys[i+1]})
    
    # print("Может есть какое-либо предположение?")
    # while True:
    #     print(to_sub(string, substitution))
    #     print(string)
    #     choice = input("From to:").split(" ")
    #     if choice[0] == '0':
    #         pprint(substitution)
    #         break
    #     else:
    #         change(substitution, choice[0], choice[1])

    # Мы получили первоначальную подстановку, основанную на частотном анализе текста
    # Теперь попробуем поменять случайным образом элементы из candidate и посмотрим на рейтинг биграмм
    bi_keys = bi_sub.keys()
    for key in bi_keys:
        bi_sub.update({key: {"sub": to_sub(key, substitution), "count": bi_sub[key]["count"]}})
    print(bi_sub)

    for cand in candidate.keys():
        
    

    
def change_sub(substituion, fr, sec):
    tmp = substituion[fr]["sub"]
    substituion[fr]["sub"] = substituion[sec]["sub"]
    substituion[sec]["sub"] = tmp
    return substituion

def change(substituion, key, letter):
    orig_sub = substituion[key]["sub"]
    tmp = ""
    for a in substituion.keys():
        if substituion[a]["sub"] == letter:
            tmp = a
    substituion[key]["sub"] = letter
    substituion[tmp]["sub"] = orig_sub
    return substituion

def analyze_threegramms(source, three_sub, substitution):
    source.seek(0)
    sub_keys = list(substitution.keys())
    while True:
        a = source.read(1).lower()
        if not a:
            break
        if a in referens:
            b = source.read(1).lower()
            if b in referens:
                c = source.read(1).lower()
                if c in referens:
                    if a+b+c not in three_sub:
                        three_sub.update({a+b+c: {"count": 1}})
                    else:
                        three_sub[a+b+c]["count"] += 1

    possible_rigth = []
    for i in range(len(sub_keys)):
        if i < 7:
            possible_rigth.append(sub_keys[i])
    
    three_sub_keys = list(three_sub.keys())
    for key in three_sub_keys:
        check = 0
        for a in possible_rigth:
            if a in key:
                check += 1
        if check == 0:
            del three_sub[key]
    
    three_sub = OrderedDict(sorted(three_sub.items(), key=lambda x: x[1]["count"], reverse=True))
    return three_sub

def analyze_bigramms(source, bi_sub, substituion):
    sub_keys = list(substituion.keys())
    letter = sub_keys[0]
    source.seek(0)
    while True:
        a = source.read(1).lower()
        if not a:
            break
        if a in referens:
            b = source.read(1).lower()
            if letter not in a+b:
                continue
            if b not in referens:
                continue
            if a+b not in bi_sub:
                bi_sub.update({a+b: {"count": 1}})
            else:
                bi_sub[a+b]["count"] += 1
    
    bi_sub = OrderedDict(sorted(bi_sub.items(), key=lambda x:x[1]["count"], reverse=True))

    # Выбираем возможно верные биекции
    possible_rigth = []
    for i in range(len(sub_keys)):
        if i > 0 and i < 15:
            possible_rigth.append(sub_keys[i])
    
    for key in list(bi_sub.keys()):
        check = 0
        for a in possible_rigth:
            if a in key:
                check += 1
        if check == 0:
            del bi_sub[key]

    return bi_sub

# Вывовод перестановки - pretty print
def pprint(substitution):
    sub_keys = list(substitution.keys())
    sub_keys.sort()
    print("---------------------------------")
    for key in sub_keys:
        print(key, end='')
    print()
    for key in sub_keys:
        print(substitution[key]["sub"], end='')
    print("\n---------------------------------")

# Получить частотные характеристики
def analyze(source, substitution):
    source.seek(0)
    # Переменная, которая считает количество символов в тексте
    letters_count = 0
    # Считаем количество символов каждой буквы
    while True:
        letter = source.read(1).lower()
        if not letter:
            break
        if letter in referens:
            if letter not in substitution:
                substitution.update({letter : {"count" : 1}})
            else:
                substitution[letter]["count"] += 1
            letters_count += 1

    print("Количество русских букв в тексте:", letters_count)
    # Теперь посчитаем частоту каждой буквы
    for letter in referens:
        try:
            substitution[letter]["freq"] = substitution[letter]["count"] / letters_count * 100
        except KeyError:
            substitution.update({letter: {"freq": 0, "count": 0, "sub": "#"}})
        
    # Отсортируем этот словарь по частоте, чтобы сделать биективное отображение букв
    substitution = OrderedDict(sorted(substitution.items(), key=lambda x: x[1]["freq"], reverse=True))
    # Сделаем первоначальную биекцию

    # Ключи, т.е. буквы в нашем словаре
    for i, key in enumerate(list(substitution.keys())):
        if substitution[key]["freq"] == 0:
            substitution[key]["sub"] = "#"
        else:
            substitution[key]["sub"] = referens[i]

    return substitution
    
# Расшифровать весь файл
def decrypt_file(source, output, substitution):
    while True:
        letter = source.read(1).lower()
        if not letter:
            break
        if letter in referens:
            output.write(substitution[letter]["sub"])
        else:
            output.write(letter)    

# Сделать подстановку на строке
def to_sub(string, substitution):
    decrypt = ""
    for char in string:
        if char in referens:
            decrypt += substitution[char]["sub"]
        else:
            decrypt += char
    return decrypt

if __name__ == '__main__':
    source = open("texts/ed_cipher.txt", "r")
    output = open("decrypt.txt", "w")
    print("[+] Открыт файл", source.name)

    attack(source, output)

    print("[+] Атака на текст завершена")

    source.close()
    output.close()
