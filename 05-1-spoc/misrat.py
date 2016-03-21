import numpy

ACCESS_COUNT = 20
WINDOW_SIZE = 2
PAGE_NUM = 6
MEM_SIZE = 4

if __name__ == '__main__':
    accessList = numpy.random.randint(0, PAGE_NUM, ACCESS_COUNT)

    D = dict()
    last = -1
    for current in xrange(ACCESS_COUNT):
        idx = accessList[current]
        if idx in D:
            D[idx] = current
            print "access", idx, "hit"
        else:
            if current - last <= WINDOW_SIZE:
                D[idx] = current
                print "access", idx, "miss", "load", D.keys()
                last = current
                if (len(D) > MEM_SIZE):
                    tmp = D.items()
                    tmp.sort(lambda x, y: x[1] - y[1])
                    D = dict(tmp[1:])
            else:
                tmp = filter(lambda x: x[1] >= last, D.items())
                D = dict(map(lambda x: (x[0], 0), tmp))
                D[idx] = current
                print "access", idx, "miss", "filter & load", D.keys()
                last = current
        assert(len(D) <= MEM_SIZE)
