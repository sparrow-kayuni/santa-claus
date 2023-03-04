import threading as th
from enum import Enum
from random import random

class Reindeer (th.Thread):
    class ReindeerState(Enum):
        ON_VACATION = 1
        IN_SHED = 2
        HITCHED = 3
        TERMINATED = 4
    
    global np

    def __init__(self, rid, np):
        super().__init__()
        self.state = self.ReindeerState.ON_VACATION
        self.reindeer_id = rid
        self.report = ""

    def set_state(self, state):
        self.state = state

    def run(self):
        while(True):
            if(self.state == self.ReindeerState.TERMINATED):
                break
    
            if(self.state == self.ReindeerState.ON_VACATION):
                self.report = "Reindeer on Vacation"
                if(np.is_december()):
                    if(random() < 0.2):
                        self.state = State.IN_SHED
                        np.reindeerCount += 1
            
            elif(self.state == self.ReindeerState.IN_SHED):
                self.report = "Reindeer is back in the shed"
                if(np.reindeerCount == 9):
                    np.santa.wakeSanta()

            elif(self.state == self.ReindeerState.HITCHED):
                self.report = "Reindeer is hitched"
        
        print("reindeer done")
    
    def print_report(self):
        # np.print_sem.acquire()
        print(self.report)
        # np.print_sem.release()

from main import np