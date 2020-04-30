from math import ceil
import time
import requests
import re

def fermat(n):
	tic = time.perf_counter()
	m = int(ceil(n**0.5))
	k = 1
	print(m)
	while not perfect_square((m + k)**2 - n):
		k += 1
	print(k)
	x = m+k
	y = int(((m + k)**2 - n)**0.5)
	p = x - y
	q = x + y
	toc = time.perf_counter()
	print(f"Time:{toc - tic:.4f}")
	return p, q

def perfect_square(x):
	return (int(x**0.5))**2 == x

def factordb(n):
	url = "http://factordb.com/index.php?query="
	s = requests.Session()
	response = s.get(url+str(n)).text
	regex = re.compile(r"index\.php\?id\=([0-9]+)", re.IGNORECASE)
	ids = regex.findall(response)
	return int(ids[1]), int(ids[2])