import binascii
import PyPDF2
import re

def s2n(s):
	if not len(s):
		return 0
	return int(binascii.hexlify(s), 16)

def n2s(n):
	s = hex(n)[2:].rstrip("L")
	if len(s) % 2 != 0:
		s = "0" + s
	return binascii.unhexlify(s)

def egcd(a, b):
	if a == 0:
		return (b, 0, 1)
	else:
		g, y, x = egcd(b%a, a)
		# coefficients for the extended Euclid algorithm
		return (g, x-(b//a)*y, y)

def inverse(a, m):
	g, x, y = egcd(a, m)
	if g != 1:
		raise Exception("Modular inverse doesn't exist")
	else:
		return x%m

def get_num(c):
	# Check hex or not 
	if c.startswith("0x"):
		c = int(c, 16)
	else:
		c = int(c)
	return c

def parse_pdf(file):
	with open(file, "rb") as pdfFileObj:
		# initiate pdf
		pdfReader = PyPDF2.PdfFileReader(pdfFileObj)
		raw = str(pdfReader.getPage(0).extractText().encode("utf-8"))

		# parse n and e
		regex = re.compile(r"=[0-9]+")
		numbers = regex.findall(raw)
		n, e = numbers[0].replace("=", ""), numbers[1].replace("=", "")

		# parse text
		regex = re.compile(r"0x[a-z0-9]{12,}")
		ciphers = "".join(regex.findall(raw)).split("0x")[1:]
		ciphers = ["0x" + c for c in ciphers]
		
		print(f"[+] Parsed n: {n}")
		print(f"[+] Parsed e: {e}")
		print(f"[+] Parsed text")
		return (int(n), int(e), ciphers)