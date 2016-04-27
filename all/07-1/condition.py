import threading
import time
import random

condition = threading.Condition()
n1 = 5
n2 = 5
m = 3
msgIn = [m] * n2
msgOut = [0] * n2

class Producer(threading.Thread):
    def __init__(self):
        threading.Thread.__init__(self)
    
    def run(self):
        global condition, msgIn, msgOut
        while True:
            if condition.acquire():
                flag = 0
                for i in range(n2):
                    if msgIn[i] == 0:
                        flag = 1
                if flag == 0:
                    for i in range(n2):
                        msgIn[i] -= 1
                    print "A(%s): Send a Msg" %(self.name)
                    condition.notify()
                    for i in range(n2):
                        msgOut[i] += 1
                else:
                    print "A(%s): Failed" %(self.name)
                    condition.wait();
                condition.release()
                time.sleep(random.random()*2)

class Consumer(threading.Thread):
    index = 0
    def __init__(self, c):
        self.index = c
        threading.Thread.__init__(self)
    
    def run(self):
        global condition, msgIn, msgOut
        index = self.index
        while True:
            if condition.acquire():
                if msgOut[index] >= 1:
                    msgOut[index] -= 1
                    print "B(%s): Receive a Msg, %s Msg Left" %(self.name, msgOut[index])
                    condition.notify()
                    msgIn[index] += 1
                else:
                    print "B(%s): No msg" %(self.name)
                    condition.wait();
                condition.release()
                time.sleep(random.random()*2)

if __name__ == "__main__":
    for i in range(0, n1):
        p = Producer()
        p.start()
    
    for i in range(0, n2):
        c = Consumer(i)
        c.start()