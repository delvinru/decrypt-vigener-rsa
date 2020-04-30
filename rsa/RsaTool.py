#!/usr/bin/env python3
import os
import sys
import argparse
from lib.utils import *
from lib.attack import *

def encrypt(args):
	print("[+] Encrypt file started")

	# compute modulus n
	args.n = args.p * args.q

	# open input file
	if args.istr is None:
		uncipher = []
		try:
			with open(args.ifile, "rb") as src:
				uncipher.append(src.readlines())
		except OSError:
			print(f"[-] File {args.ifile} not found or can't be read")
			exit(0)
		args.istr = uncipher
	
	args.istr = [s2n(m) for m in args.istr]

	# Calculate Euler function
	phi = (args.p - 1)*(args.q - 1)
	
	# Calculate secret exponent
	d = inverse(args.e, phi)

	# Calculate cipher text/textx
	cipher = [hex(pow(m, e, d)) for m in args.istr]
	print("[+] ...")
	
	try:
		path = "encrypt_result"
		if not os.path.exists(path):
			os.mkdir(path)

		with open(os.path.join(path, args.ofile), "w") as out:
			out.write(hex("\n".join(cipher)))

		with open(os.path.join(path, "key.pub"), "w") as out:
			out.write(f"e:{args.e}\nn:{args.n}")

		with open(os.path.join(path, "key.pem"), "w") as out:
			out.write(f"e:{args.e}\nn:{args.n}\np:{args.p}\nq:{args.q}")

	except OSError:
		print(f"[-] File {args.ofile} not found or can't be read")
		exit(0)
	
	print(f"[+] Encrypt complete. Check folder {path}/")
	print("[+] Cipher: " + " ".join(cipher))

def decrypt(args):
	print("[+] Decrypt started")
	if args.n is None:
		args.n = args.p * args.q

	if args.istr is None:
		try:
			with open(args.ifile, "r") as src:
				args.istr = src.readlines()
		except OSError:
			print(f"[-] File {args.ifile} not found or can't be read")
			exit(0)

	# Calculate necessary components
	print("[+] ...")
	phi = (args.p - 1)*(args.q - 1)
	d = inverse(args.e, phi)

	# message in int type
	message = [pow(get_num(c), d, args.n) for c in args.istr]
	message = [n2s(m).decode("utf-8") for m in message]
	try:
		path = "decrypt_result"
		if not os.path.exists(path):
			os.mkdir(path)

		with open(os.path.join(path, args.ofile), "w") as out:
			out.write("".join(message))
		
		print(f"[+] Decrypt complete. Check folder: {path}/")
		print(f"[+] p: {args.p}\n[+] q: {args.q}")
		print(f"[+] Message: " + "".join(message))
	except OSError:
		print(f"[-] File {arg.ofile} not found or can't be read")

def attack(args):
	if args.attack == "factordb":
		print("[+] Attack with factordb started")
		args.p, args.q = factordb(args.n)
		decrypt(args)
		
	elif args.attack == "fermat":
		print("[+] Attack with Fermat factorization started")
		args.p, args.q = fermat(args.n)
		decrypt(args)

	print("[+] Attack complete")

def main(args):
	if args.mode == "enc":
		encrypt(args)
	elif args.mode == "dec":
		decrypt(args)
	elif args.attack is not None:
		attack(args)

if __name__ == "__main__":
	if sys.version_info[0] < 3:
		raise Exception("Python version higher than 3.7 is required")

	# Initiate parser
	desc = "This program was developed by students of the KKSO-01-19 group.\nKolesnikov Alexey - main developer\nKazarin Anton - contributor"
	parser = argparse.ArgumentParser(epilog=desc, formatter_class=argparse.RawTextHelpFormatter)

	parser.add_argument(
		"-m", "--mode", 
		choices=["enc", "dec"], 
		required=("--attack" not in sys.argv),
		help="select the program mode"
	)
	parser.add_argument(
		"--attack",
		choices=["fermat", "factordb"],
		help="Choose type of attack"
	)
	parser.add_argument(
		"-e", 
		type=int,
		metavar="<num>", 
		help="input the exponent of RSA"
	)
	parser.add_argument(
		"-n", 
		type=int,
		metavar="<num>",
		help="input the modulus N of RSA"
	)
	parser.add_argument(
		"-p", 
		type=int,
		metavar="<num>",
		help="input prime p"
	)
	parser.add_argument(
		"-q", 
		type=int,
		metavar="<num>",
		help="input prime q"
	)
	parser.add_argument(
		"-if", "--ifile", 
		metavar="<input file>",
		help="input file with text"
	)
	parser.add_argument(
		"-is", "--istr",
		metavar="<str>",
		help="input text without file"
	)
	parser.add_argument(
		"-of", "--ofile", 
		default="out.txt",
		metavar="<output file>",
		help="input file for encrypt/decrypt message"
	)
	parser.add_argument(
		"--pdf",
		metavar="<file>",
		help="extract all information from PDF file"
	)
	parser.add_argument(
		"-v", "--version", 
		action="version", 
		version="%(prog)s (version 0.1)",
		help="show program version"
	)
	
	if len(sys.argv) == 1:
		parser.print_help()
		sys.exit(1)

	# Read arguments from the command line
	args = parser.parse_args()

	if args.pdf is not None:
		args.n, args.e, args.istr = parse_pdf(args.pdf)

	# Run main function
	main(args)