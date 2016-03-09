#coding: utf-8
import numpy

phy = []
base = 0x220 / 32


def transfer(str1):
	number = 0
	str1 = str1.lower()
	for i in str1:
		if i <= '9':
			number = number * 16 + ord(i) - ord('0')
		else:
			number = number * 16 + ord(i) - ord('a') + 10
	return number

def insert():
	global phy
	f = open("data.txt", "r")
	while (True):
		content = f.readline()
		if (content == ""):
			break
		content = content.split()
		for i in content:
			phy.append(transfer(i))
	f.close()

def find(str1):
	print "Virtual Address\t" + str1 + ":"
	global base
	global phy
	number = transfer(str1)
	of = number % 32
	number = number / 32
	p2 = number % 32
	p1 = number / 32

	pdeid = phy[base  * 32 + p1]

	print "\t--> pde index:%02x pde contents:(valid %d, pfn %02x)" % (p1, pdeid / 128, pdeid % 128)

	if (pdeid / 128 == 0):
		return

	base1 = pdeid % 128


	pteid = phy[base1  * 32 + p2]

	print "\t\t--> pte index:%02x pte contents:(valid %d, pfn %02x)" % (p2, pteid / 128, pteid % 128)

	if (pteid / 128 == 0):
		print "\t\t\tFault (page table entry not valid)"
		return



	base2 = pteid % 128

	phy1 = phy[base2  * 32 + of]

	print "\t\t\t--> Translates to Physical Address %02x --> Value: %02x" % (base2  * 32 + of, phy1)



insert()
find("03DF")
find("69DC")
find("6c74")