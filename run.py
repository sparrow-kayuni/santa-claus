from threading import BoundedSemaphore, Thread
import time
from random import random
import os
from enum import Enum

elf_sem = BoundedSemaphore(value = 3) # initialize
reindeer_sem = BoundedSemaphore(value = 9)
elf_count_sem = BoundedSemaphore(value = 1)
message_sem = BoundedSemaphore(value = 1)

elf_message = ""

elf_count = 0 # init elf count


class Elf:
    class State(Enum):
        WORKING = 1
        WAITING_FOR_HELP = 2
        READY_FOR_HELP = 3

    def __init__(self, id):
        self.state = self.State.WORKING
        self.id = id


def elf():
    global elf_count
    global elf_message

    while True:
        if(random() < 0.2): # randomly select elf that needs help 
            elf_sem.acquire() # decrement elf semaphore value
            
            elf_count_sem.acquire() # prevent other threads from changing elf count
            elf_count += 1 
            elf_count_sem.release()
        
        message_sem.acquire() # prevents other threads from printing
        elf_message = f"{args[0].id} is {args[0].state}"
        message_sem.release()


def elves():
    global elf_count
    global elf_message

    elves_list = [Elf(i) for i in range(1, 10)]

    # create elf threads in list
    elf_ths = [Thread(target=elf) for elf in elves_list]
    
    # start each elf thread
    for elf_th in elf_ths:
        elf_th.start() 

    while True:
        if(elf_count == 3):
            for i in range(3):
                elf_sem.release()

                elf_count_sem.acquire()
                elf_count -= 1
                elf_count_sem.release()
            
            # message_sem.acquire()
            # elf_message = "ready for help"
            # message_sem.release()
        
        message_sem.acquire()
        print(elf_message)
        message_sem.release()
    
elves()