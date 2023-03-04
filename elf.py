import threading as th
from enum import Enum
from random import randrange
from time import sleep


class Elf(th.Thread):
    class ElfState(Enum):
        WORKING = 1
        NEEDS_HELP = 2
        READY_FOR_HELP = 3
        HELPED = 4
        TERMINATED = 5
    
    global np

    def __init__(self, eid, np):
        super().__init__()
        self.elfNum = eid
        self.state = self.ElfState.WORKING
        self.report = ""
        self.elf_q_length = 0
    
    def set_state(self, state):
        self.state = state

    def get_help(self):
        print(f"{self.elfNum} gotten help")
        np.elf_sem.release()
    
    def run(self):
        while(True):

            if(self.state == self.ElfState.TERMINATED):
                break
            
            if(self.state == self.ElfState.READY_FOR_HELP):
                self.report = f"Elf {self.elfNum} is ready for help"

            if(self.state == self.ElfState.NEEDS_HELP):
                self.report = f"Elf {self.elfNum} Needs help"
                
                np.elf_count_sem.acquire()
                np.elf_count += 1
                np.elf_count_sem.release()

                if(np.elf_count == 3):
                    self.state = self.ElfState.READY_FOR_HELP
                    np.santa.awaken_santa()
            
            if(self.state == self.ElfState.WORKING):
                self.report = f"Elf {self.elfNum} is working"

                sleep(randrange(0, 10))


                self.state = self.ElfState.NEEDS_HELP

                # wake santa up

        print("elf done")
    
    def print_report(self):
        # np.print_sem.acquire()
        print(self.report)
        # np.print_sem.release()

from main import np