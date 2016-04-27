import threading
import random
import time

n1 = 5
n2 = 5
m = 3
msgIn = []
msgOut = []


class Producer(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)
    
    def run(self):
        global msgIn, msgOut
        while True:
            for i in range(n2):
                msgIn[i].acquire()
            print "A(%s): Send a Msg" %(self.name)
            for i in range(n2):
                msgOut[i].release()
        time.sleep(random.random()*2)

class Consumer(threading.Thread):
    index = 0
    def __init__(self, index):
        threading.Thread.__init__(self)
        self.index = index
    
    def run(self):
        global msgIn, msgOut
        while True:
            msgOut[self.index].acquire()
            print "B(%s): Received a Msg" %(self.name)
            msgIn[self.index].release()
            time.sleep(random.random()*2)


for i in range(0, n2):
    msgIn.append(threading.Semaphore(m))
    msgOut.append(threading.Semaphore(0))

for i in range(0, n1):
    p = Producer()
    p.start()

for i in range(0, n2):
    c = Consumer(i)
    c.start()

