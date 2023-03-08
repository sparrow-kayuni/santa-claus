// #include <iostream>
#include <cstdio>
#include <conio.h>
#include <thread>
#include <semaphore.h>
#include <queue>
#include <chrono>
#include <vector>
#include <random>
#include <time.h>

sem_t job_q_sem;
sem_t ready_q_sem;
sem_t paused_q_sem;
sem_t elf_sem;
sem_t elf_count_sem;
sem_t rd_sem;
sem_t rd_count_sem;
sem_t rd_shed_sem;
sem_t message_sem;
sem_t done_list_sem;
sem_t christmas_sem;

std::queue<std::thread::id> job_q;
std::queue<std::thread::id> ready_q;
std::queue<std::thread::id> paused_q;
std::vector<std::thread::id> done_list;
std::vector<std::thread::id> rd_shed;

const int NUM_OF_ELVES = 500;
const int NUM_OF_REINDEER = 9;
int elf_count;
int rd_count;
int day;
int year;
bool gone_for_christmas = false;
bool rd_are_ready = false;

void elf_process(){
    sem_wait(&message_sem);
    printf("\tElf %d Needs help!!\n", std::this_thread::get_id());
    sem_post(&message_sem);
    
    int size = 0;

    sem_wait(&job_q_sem);
    job_q.push(std::this_thread::get_id());
    elf_count++;
    size = job_q.size();
    sem_post(&job_q_sem);
    
    sem_wait(&elf_sem); // allows only 3 elves at a time

    sem_wait(&message_sem);
    printf("\tElf %d is waiting for others\n", std::this_thread::get_id());
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

    //if santa is one for christmas, let other threads execute
    while(gone_for_christmas) std::this_thread::yield();

    sem_wait(&message_sem);
    printf("\tElf %d is getting help from Santa\n", std::this_thread::get_id());
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

    sem_wait(&rd_count_sem);
    rd_count++;
    sem_post(&rd_count_sem);

    sem_wait(&message_sem);
    printf("\tReindeer %d is back and Waiting in the stall. Reindeer count %d\n", 
        std::this_thread::get_id(), rd_count);
    sem_post(&message_sem);

    sem_wait(&rd_shed_sem);
    rd_shed.push_back(std::this_thread::get_id());
    sem_post(&rd_shed_sem);

    //if not all reindeers are back, yield to other threads
    while(rd_shed.size() < 9) std::this_thread::yield();

    sem_wait(&message_sem);
    printf("\tReindeer %d is getting hitched!\n", std::this_thread::get_id());
    sem_post(&message_sem);

    sem_wait(&rd_shed_sem);
    rd_shed.pop_back();
    sem_post(&rd_shed_sem);

    sem_wait(&rd_count_sem);
    rd_count--;
    sem_post(&rd_count_sem);
}

void reindeers(){    
    std::thread reindeer_ths[NUM_OF_REINDEER];
    int urgency_factor = 0;
    sem_init(&message_sem, 0, 1);

    int i = 0;
    int rand_val;

    sem_wait(&message_sem);
    printf("\tIts December, the Reindeers start coming back!\n");
    sem_post(&message_sem);

    //days to christmas
    while(day < 357 && !gone_for_christmas){  

        rand_val = rand() % 100;

        //let reindeer come back at random
        if(i < NUM_OF_REINDEER){
            reindeer_ths[i] = std::thread(&reindeer_process);
            std::this_thread::sleep_for(std::chrono::milliseconds(100));
        }

        //when there are nine reindeer, end loop
        if(rd_count == 9) {
            sem_wait(&message_sem);
            printf("\tAll 9 reindeer are back!\n");
            sem_post(&message_sem);
            break;
        }
        i++;
    }

    while(rd_shed.size() != 0) std::this_thread::yield();

    rd_are_ready = true;

    sem_wait(&christmas_sem);

    for(int i = 0; i < NUM_OF_REINDEER; i++){
        sem_wait(&message_sem);
        printf("\tReindeer %d has gone for vacation!!\n", reindeer_ths[i].get_id());
        sem_post(&message_sem);
        reindeer_ths[i].join();
    }

    sem_post(&christmas_sem);
}

void init_semaphores(){
    sem_init(&job_q_sem, 0, 1);
    sem_init(&ready_q_sem, 0, 1);
    sem_init(&paused_q_sem, 0, 1);
    sem_init(&elf_count_sem, 0, 1);
    sem_init(&rd_count_sem, 0, 1);
    sem_init(&message_sem, 0, 1);
    sem_init(&done_list_sem, 0, 1);
    sem_init(&elf_sem, 0, 3);
    sem_init(&rd_sem, 0, 9);
    sem_init(&christmas_sem, 0, 1);
}

void north_pole(){
    std::thread elf_ths[NUM_OF_ELVES];
    std::thread reindeers_th;

    int elf_i = 0;

    srand(time(0));
    do{
        if(day == 0){
            sem_wait(&message_sem);
            printf("YEAR %d\n_______________________\n", year);
            sem_post(&message_sem);
        }

        sem_wait(&message_sem);
        printf("Day: %d\n", day);
        sem_post(&message_sem);

        if(day == 365 - 31){
            reindeers_th = std::thread(&reindeers);         
        }

        if(day == 354){
            sem_wait(&message_sem);
            printf("\tSanta is ready for christmas!!!\n");
            sem_post(&message_sem);

            sem_wait(&christmas_sem);

            gone_for_christmas = true;
        }

        if(day == 364){
            sem_wait(&message_sem);
            printf("\tSanta is back from christmas!!!\n");
            sem_post(&message_sem);

            gone_for_christmas = false;
        }

        if(day >= 365) {
            sem_post(&christmas_sem);
            day = 0;
            year++;
            reindeers_th.join();
            continue;
        }

        // elves randomly need help
        if(rand() % 100 < 20 && elf_i < NUM_OF_ELVES){
            elf_ths[elf_i] = std::thread(&elf_process);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            elf_i++;
        }
        
        // if elf process is done executing, join the thread
        for(int j = 0; j < done_list.size() && !done_list.empty(); j++){  
            for(int k = 0; k < NUM_OF_ELVES; k++){
                if(elf_ths[k].get_id() == done_list.at(j)){
                    sem_wait(&message_sem);
                    printf("\tElf %d is done\n", done_list.at(j));
                    sem_post(&message_sem);
                    elf_ths[k].join();
                    break;
                }
            }
        }

        day++;
    }while(day < 1000);
}

int main(){
    elf_count = 0;
    rd_count = 0;
    day = 300;
    year = 0;

    init_semaphores();

    printf("Main thread: %d\n", std::this_thread::get_id());

    north_pole();

    return 0;
}