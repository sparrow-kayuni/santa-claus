from threading import Thread, BoundedSemaphore
from queue import Queue
from enum import Enum
import os


class NorthPole:

    class Month(Enum):
        JAN = 1
        FEB = 2
        MAR = 3
        APR = 4
        MAY = 5
        JUN = 6
        JUL = 7
        AUG = 8
        SEP = 9
        OCT = 10
        NOV = 11
        DEC = 12

    def __init__(self):
        self.elf_waiting_queue = Queue()
        self.ready_elves = Queue()
        self.reindeer_queue = Queue()
        self.elf_threads = []
        self.reindeer_threads = []
        self.reindeer_count = 0
        self.elf_count = 0

        self.elf_sem = BoundedSemaphore(3)
        self.reindeer_sem = BoundedSemaphore(9)

        self.elves_waiting_q_sem = BoundedSemaphore(3)
        self.elf_count_sem = BoundedSemaphore(1)
        self.print_sem = BoundedSemaphore(1)

        self.month = self.Month.JAN
        
    
    def init_santa(self):
        self.santa = Santa(self)
        self.santa_th = Thread(target=self.santa.run)
        self.santa_th.start()
        

    def init_elves(self):
        self.all_elves = [Elf(i, self) for i in range(10)]

        self.elf_threads = [Thread(target=elf.run) for elf in self.all_elves]
        for elf in self.all_elves:
            elf.start()  

    def print_messages(self):
        self.santa.print_report()
        for elf in self.all_elves:
            elf.print_report()

        for reindeer in self.reindeers:
            reindeer.print_report()

    def init_reindeers(self):
        self.reindeers = [Reindeer(i, self) for i in range(8)]
        
        self.reindeer_threads = [Thread(target=rd.run) for rd in self.reindeers]
        for reindeer in self.reindeers:
            reindeer.start()
    
    def is_december(self):
        return (self.month == self.Month.DEC)

    def set_month(self, day):
        self.day = day
        if(self.day > 365):
            self.day = 0
        
        if(self.day > 365 - 31):
            self.month = self.Month.DEC

    def join_threads(self):
        for reindeer in self.reindeers:
            reindeer.set_state(Reindeer.ReindeerState.TERMINATED)
            reindeer.join()
        
        for elf in self.all_elves:
            elf.set_state(Elf.ElfState.TERMINATED)
            elf.join()

np = NorthPole()

from elf import Elf
from santa import Santa
from reindeer import Reindeer

def north_pole_process():

    global np
    
    np.init_santa() # start executing santa process
    np.init_elves() # start executing elf process
    np.init_reindeers() # start executing reindeer process

    try:
        day = 0
        while(day < 800):
            np.set_month(day)
        
            if(np.is_december()):
                # reindeers start coming
                pass
            
            os.system("cls")
            print(f"Day: {np.day} Month: {np.month}")
            
            print(f"Elves: {np.elf_count}")
            if(np.elf_count == 3):
                
                for elf in np.all_elves:
                    elf.get_help()

            # print messages
            np.print_messages()
            
            day += 1
    except KeyboardInterrupt:
        print("Interrupted...")
        np.join_threads()
        return

if __name__ == "__main__":
    north_pole_process()
    print("done")