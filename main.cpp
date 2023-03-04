#include <iostream>
#include <conio.h>
#include <stdlib.h>
#include <thread>
#include <semaphore.h>


enum SantaState {SLEEPING, HELPING_ELVES, READY_FOR_CHRISTMAS, HITCHING_REINDEER, GONE_FOR_CHRISTMAS};
enum Month {NOT_DECEMBER, DECEMBER};

void santa_process(std::string &santa_message, SantaState &state){
    while(true){
        switch(state){
            case SLEEPING:
                // acquire santa_message semaphore (block others from changing it)
                santa_message = "Santa is Snoring...";
                break;
            case HELPING_ELVES:
                santa_message = "Santa is Helping Elves";
                break;
            case HITCHING_REINDEER:
                santa_message = "Santa is Hitching Reindeer";
                break;
            case READY_FOR_CHRISTMAS:
                santa_message = "Santa is Ready for Christmas";
                break;
            case GONE_FOR_CHRISTMAS:
                santa_message = "Santa has gone for Christmas";
        }
    }
}

void init_santa(std::string &message, SantaState &state){
    std::thread santa = std::thread(&santa_process, message);
    state = SantaState(SLEEPING);
}

void end_santa(std::thread &santa){
    santa.join();
}

void init_reindeers(){
    //
}

void print_messages(std::string santa){
    system("cls");
    std::cout << santa;
}

void north_pole(){
    int day = 0;
    Month month = Month(NOT_DECEMBER);
    std::string santa_message = "";

    SantaState santa_state = SantaState(SLEEPING);
    init_santa(santa_message, santa_state);

    while(true){
        if(day > 365) day = 0;
        else if(day > 365 - 31) month = Month(DECEMBER);

        switch(month){
            case NOT_DECEMBER:
                break;
            case DECEMBER:
                init_reindeers();
                if(day > 345) santa_state = SantaState(READY_FOR_CHRISTMAS);
                break;
        }

        print_messages(santa_message);
        day++;
    }
}

int main(){
    north_pole();
    return 0;
}