#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <thread>
#include <semaphore.h>
#include <queue>
#include <chrono>
#include <vector>
#include <random>
#include <time.h>

struct p{
    int pid;
    std::string p_name;
};

sem_t job_q_sem;
sem_t ready_q_sem;
sem_t paused_q_sem;
sem_t elf_sem;
sem_t elf_count_sem;
sem_t reindeer_sem;
sem_t rd_count_sem;
sem_t message_sem;
sem_t done_list_sem;

std::queue<std::thread::id> job_q;
std::queue<std::thread::id> ready_q;
std::queue<std::thread::id> paused_q;
std::vector<std::thread::id> done_list;

int elf_count;
int rd_count;

void elf_working(int elf_id){
    std::cout << "Elf " << elf_id << " is working...\n";
}

void elf_process(){
    sem_wait(&message_sem);
    std::cout << "\tElf " << std::this_thread::get_id() << " Needs help!!\n";
    sem_post(&message_sem);
    
    int size = 0;

    sem_wait(&job_q_sem);
    job_q.push(std::this_thread::get_id());
    elf_count++;
    size = job_q.size();
    sem_post(&job_q_sem);
    
    sem_wait(&elf_sem);

    sem_wait(&message_sem);
    std::cout << "\tElf " << std::this_thread::get_id() << " is waiting for others\n";
    sem_post(&message_sem);

    while(elf_count < 3) {
        std::this_thread::yield();
    }

    sem_wait(&job_q_sem);
    job_q.pop();
    sem_post(&job_q_sem);

    sem_wait(&ready_q_sem);
    ready_q.push(std::this_thread::get_id());
    sem_post(&ready_q_sem);

    sem_wait(&message_sem);
    std::cout << "\tElf " << std::this_thread::get_id() << " is getting help from Santa\n";
    sem_post(&message_sem);

    sem_post(&elf_sem);

    sem_wait(&elf_count_sem);
    elf_count--;
    sem_post(&elf_count_sem);

    sem_wait(&ready_q_sem);
    ready_q.pop();
    sem_post(&ready_q_sem);

    sem_wait(&done_list_sem);
    done_list.push_back(std::this_thread::get_id());
    sem_post(&done_list_sem);
}

void reindeer_process(){
    sem_wait(&message_sem);
    std::cout << "\tReindeer " << std::this_thread::get_id() << " Has come back!!\n";
    sem_post(&message_sem);

    int rq_size = 0;
    sem_wait(&reindeer_sem);
    std::cout << "\tReindeer " << std::this_thread::get_id() << " is Waiting in the stall...\n";

    sem_wait(&rd_count_sem);
    rd_count++;
    
    sem_post(&rd_count_sem);

    //if not all reindeers are back, let other processes run
    while(rd_count < 9){
        std::this_thread::yield();
    }

    sem_wait(&ready_q_sem);
    rq_size = ready_q.size();
    sem_post(&ready_q_sem);

    while(rq_size < 3) std::this_thread::yield();

    sem_wait(&ready_q_sem);
    ready_q.push(std::this_thread::get_id());
    sem_push(&ready_q_sem);

    sem_post(&reindeer_sem);
}


int main(){
      
    int NUM_OF_ELVES = 100;
    int NUM_OF_REINDEER = 9;
    elf_count = 0;
    rd_count = 0;

    sem_init(&job_q_sem, 0, 1);
    sem_init(&ready_q_sem, 0, 1);
    sem_init(&paused_q_sem, 0, 1);
    sem_init(&elf_count_sem, 0, 1);
    sem_init(&rd_count_sem, 0, 1);
    sem_init(&message_sem, 0, 1);
    sem_init(&done_list_sem, 0, 1);
    sem_init(&elf_sem, 0, 3);

    std::cout << "Main thread: " << std::this_thread::get_id() << "\n";

    std::thread elf_ths[NUM_OF_ELVES];
    std::thread reindeer_ths[NUM_OF_REINDEER];

    int day = 0;

    int elf_index = 0;
    int rd_index = 0;

    srand(time(0));
    do{
        std::cout << "Day: " << day << "\n";

        if(day > 365) day = 0;

        if(day > 365 - 31){
            if(day == 365 - 30){
                sem_wait(&message_sem);
                std::cout << "Its December, the Reindeers start coming back!\n";
                sem_post(&message_sem);
            }
            
            if(rand() % 100 < day - 290 && rd_index < NUM_OF_REINDEER){
                reindeer_ths[rd_index] = std::thread(&reindeer_process);
                std::this_thread::sleep_for(std::chrono::milliseconds(1000));
                rd_index++;
            }

            if(day == 364){
                for(int i = 0; i < NUM_OF_REINDEER; i++){
                    reindeer_ths[i].join();
                }
            }
        }

        // elves randomly need help
        if(rand() % 100 < 20 && elf_index < NUM_OF_ELVES){
            elf_ths[elf_index] = std::thread(&elf_process);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            elf_index++;
        }
        
        // if elf process is done executing, join the thread
        for(int j = 0; j < done_list.size() && !done_list.empty(); j++){  
            for(int k = 0; k < NUM_OF_ELVES; k++){
                if(elf_ths[k].get_id() == done_list.at(j)){
                    sem_wait(&message_sem);
                    std::cout << "\tElf " << done_list.at(j) << " is done\n";
                    sem_post(&message_sem);
                    elf_ths[k].join();
                    break;
                }
            }
        }

        day++;
    }while(day < 1000);


    return 0;
}