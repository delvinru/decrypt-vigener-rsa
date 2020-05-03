# decrypt-vizhener-rsa

# RsaTool

Tool for encrypt/decrypt/attack RSA algorithm

## Usage
```
usage: RsaTool.py [-h] -m {enc,dec} [--attack {fermat,factordb}] [-e <num>]
                  [-n <num>] [-p <num>] [-q <num>] [-if <input file>]
                  [-is <str>] [-of <output file>] [--pdf <file>] [-v]

optional arguments:
  -h, --help            show this help message and exit
  -m {enc,dec}, --mode {enc,dec}
                        select the program mode
  --attack {fermat,factordb}
                        Choose type of attack
  -e <num>              input the exponent of RSA
  -n <num>              input the modulus N of RSA
  -p <num>              input prime p
  -q <num>              input prime q
  -if <input file>, --ifile <input file>
                        input file with text
  -is <str>, --istr <str>
                        input text without file
  -of <output file>, --ofile <output file>
                        input file for encrypt/decrypt message (default: out.txt)
  --pdf <file>          extract all information from PDF file
  -v, --version         show program version
```
---

### Available attacks
- Fermat factorization
- FactorDB for factorizing prime(preferably)
---

### Example:
1. Traditional
	`./RsaTool.py -m enc -p 61 -q 53 -is "secret"`

2. You can specify attack method and input encrypted message
	When you specify `--attack` you should not use `-m {enc, dec}`

	`./RsaTool.py --attack fermat -n <num> -e <num> -if <file_with_message>`

3. Special formatted input for our task\
	How it works?\
	You just input special formatted pdf file after `--pdf` and programm use this source


	`./RsaTool.py --attack factordb --pdf src/my_var.pdf`	
---
### Reqiremenets
- Argparse
- PyPDF2
---
### Ubuntu 19.10 install instruction
```bash
git clone https://github.com/delvinru/decrypt-vigener-rsa.git
cd decrypt-vigener-rsa/rsa
pip3 install -r requirements.txt
python3 RsaTool.py
```
---

## License
[MIT](https://choosealicense.com/licenses/mit/)
