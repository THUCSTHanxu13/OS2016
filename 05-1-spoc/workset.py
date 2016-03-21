def calc(lists):
	res = []
	for i in lists:
		if not i in res:
			res.append(i)
	return res

memory = ['e', 'd', 'a']
a = ['e', 'd', 'a']
addr = ['c', 'c', 'd', 'b', 'c', 'e', 'c', 'e', 'a', 'd']
t = 4
for i in addr:
	if not i in memory:
		print "miss\t",
		print i
		memory.append(i)
	else:
		print "hit\t",
		print i
	if (len(a) == t):
		a.append(i)
		a = a[1:]
	else:
		a.append(i)
	for i in memory:
		if not i in a:
			print "filter\t",
			print i
			memory.remove(i)
			



