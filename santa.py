import threading as th
from enum import Enum


class Santa (th.Thread): #inherit from the thread class
    class SantaState(Enum):
        SLEEPING = 1
        HELPING_ELVES = 2
        AWOKEN_BY_ELVES = 3
        AWOKEN_BY_REINDEER = 4
        READY_FOR_CHRISTMAS = 5
        GONE_FOR_CHRISTMAS = 6
        TERMINATED = 7

    global np

    def __init__(self, np):
        super().__init__()
        self.state = self.SantaState.SLEEPING
        self.report = ""

    def set_state(self, state):
        self.state = state

    
    def run(self):
        
        while(True):
            if(self.state == self.SantaState.TERMINATED):
                break

            if(self.state == self.SantaState.SLEEPING):
                self.report = "Santa is snoring..."
            elif(self.state == self.SantaState.AWOKEN_BY_ELVES):
                self.report = "Santa awoken by elves..."
                pass
            elif(self.state == self.SantaState.HELPING_ELVES):
                self.report = "Santa is helping elves..."
                pass
            elif(self.state == self.SantaState.AWOKEN_BY_REINDEER):
                self.report = "Santa awoken by reindeer..."
                pass
            elif(self.state == self.SantaState.READY_FOR_CHRISTMAS):
                self.report = "Ready for christmas!"
                pass
            elif(self.state == self.SantaState.GONE_FOR_CHRISTMAS):
                self.report = "Gone for chrismas"
                pass
    
        print("santa done")

    def awaken_santa(source):
        self.state = self.SantaState.AWOKEN_BY_ELVES

    def print_report(self):
        # np.print_sem.acquire()
        print(self.report)
        # np.print_sem.release()

from main import np