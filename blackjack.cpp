#include <iostream>
#include <cstdlib>
#include <vector>
#include <ctime>
#include <sstream>

struct Card{
std::string num;
std::string type;
int value;
};

Card drawCards(){
    std::string cardNum[12] = {"Ace","Two","Three","Four","Five","Six","Seven","Eight","Nine","Jack","Queen","King"};
    std::string cardType[4] = {"Clubs","Spades","Hearts","Diamonds"};
    int values[12] = {11,2,3,4,5,6,7,8,9,10,10,10};

    std::string card;
    int randomInt=rand()%12;
    int randInt=rand()%4;

    Card cardd = {cardNum[randomInt],cardType[randInt],values[randomInt]};
    return cardd;
}

int handValue(std::vector<Card>&player){
    int value=0;
    int ace=0;

    for(Card card:player){
        value+=card.value;
        if(card.num=="Ace"){
            ace++;
        }
    }
        while(value>21 && ace>0){
            value-=10;
            ace--;
        }
        return value;
    }

void showHand(std::vector<Card>&player){
    for(Card card:player){
        std::cout<<card.num<<" of "<<card.type<<std::endl;
    }
}

void hit(std::vector<Card>&player){
    Card newCard = drawCards();
    player.push_back(newCard);
    std::cout<<"\nCard Drawn: "<<newCard.num<<" of "<<newCard.type<<std::endl;
}

bool playAgain(){
    std::string choice;
    while(true) {
        std::cout << "Would you like to play again? (Y/N)" << std::endl;
        std::cin >> choice;
        if(choice == "Y" || choice == "y") return true;
        else if(choice == "N" || choice == "n") return false;
        else if(choice == "Z" || choice == "z") return false;
        else std::cout << "Invalid input, please enter 'Y' or 'N'." << std::endl;
    }
}

void finalStats(int money,int games,int wins,int ties,int losses){
    int winrate = games > 0 ? static_cast<int>((static_cast<float>(wins) / games) * 100) : 0;
    std::cout<<"\nFinal Results\nTotal Money: "<<money<<"\nTotal Games: "<<games<<"\nTotal Wins: "<<wins<<"\nTotal Losses: "<<losses<<"\nTotal Ties: "<<ties<<"\nWinrate: "<<winrate<<"%"<<std::endl;
}

int main(){
    srand(time(0));
    bool game=true;
    std::string choice;
    int money=1000;
    int amountBet=0;
    int games=0;
    int wins=0;
    int ties=0;
    int losses=0;

    std::vector<Card> player;
    std::vector<Card> dealer;

    std::cout<<"Welcome to my blackjack game!"<<std::endl;
    std::cout<<"You have $1000 to start with"<<std::endl;
    std::cout<<"If you ever want to quit, press Z\n"<<std::endl;

    //The actual game
    while(game){
        if(money<=0){
            std::cout<<"You are out of money! Game over."<<std::endl;
            break;
        }

        bool playerBust=false;
        player.clear();
        dealer.clear();
        choice="";
        games++;
    
        std::string betInput;
        do {
            std::cout << "How much would you like to bet?" << std::endl;
            std::cin >> betInput;

            if (betInput == "Z" || betInput == "z") {
                std::cout << "Thank you for playing!" << std::endl;
                return 0;  // End the game
            }

            std::stringstream ss(betInput);
            if (!(ss >> amountBet) || amountBet <= 0 || amountBet > money) {
                std::cout << "Invalid bet. Please enter a valid amount between 1 and " << money << "." << std::endl;
                amountBet = 0;  // Reset amountBet for the next loop iteration
            }
        } while (amountBet > money || amountBet <= 0);

        money-=amountBet;
        std::cout<<"The amount of money you have right now is: "<<money<<std::endl;
        std::cout<<"The amount of money you bet is: "<<amountBet<<"\n"<<std::endl;
        //Gives the starting cards for the player and dealer
        player.push_back(drawCards());
        player.push_back(drawCards());
        dealer.push_back(drawCards());
        dealer.push_back(drawCards());

        //Checks if the player or dealer gets a natural blackjack or tie
        if(handValue(player)==21||handValue(dealer)==21){
            if(handValue(player)==handValue(dealer)){
                std::cout<<"Both you and the dealer have a natural blackjack! It's a tie!"<<std::endl;
                ties++;
                money+=amountBet;
            }
            else if(handValue(player)==21){
                std::cout<<"You have a natural blackjack! You win!"<<std::endl;
                money+=amountBet*2;
                wins++;
            }
            else if(handValue(dealer)==21){
                std::cout<<"The dealer has a natural blackjack! You Lose!"<<std::endl;
                losses++;
            }
            game = playAgain();
            continue;
        }

        //Shows the hands of the player and dealer
        std::cout<<"Your hand: "<<std::endl;
        showHand(player);  
        std::cout<<"Your hand has a value of: "<<handValue(player)<<"\n"<<std::endl;

        std::cout<<"The dealer's hand:"<<std::endl;
        showHand(dealer);
        std::cout<<"The dealer has a hand has value of: "<<handValue(dealer)<<"\n"<<std::endl;

        //Player turn
        while(handValue(player) < 21 && (choice != "Y" && choice != "y")) {
            std::cout<<"Press X to hit and Y to stand"<<std::endl;
            std::cin>>choice;

            if(choice=="X"||choice=="x"){
                hit(player);
                std::cout<<"\nYour hand: "<<std::endl;
                showHand(player);
                std::cout<<"\nYour hand has a value of: "<<handValue(player)<<"\n"<<std::endl;

                if(handValue(player)>21){
                    std::cout<<"\nYou busted with a hand value of "<<handValue(player)<<std::endl;
                    playerBust=true;
                    break;
                }
            }
            else if(choice=="Y"||choice=="y"){
                std::cout<<"You have decided to stand"<<std::endl;
            }
            else if(choice=="Z"||choice=="z"){
                std::cout<<"Thank you for playing"<<std::endl;
                finalStats(money,games,wins,ties,losses);
                return 0;
            }
        }
        choice="";
        while(handValue(dealer)<17 && !playerBust){
            hit(dealer);
            std::cout<<"The dealer's hand: "<<std::endl;
            showHand(dealer);
            std::cout<<"The dealer's hand has a value of: "<<handValue(dealer)<<"\n"<<std::endl;
        }

        // Determine the outcome
        if(playerBust){
            std::cout<<"Dealer wins!"<<std::endl;
            losses++;
        } else if(handValue(dealer) > 21){
            std::cout<<"Dealer busts! You win!"<<std::endl;
            wins++;
            money+=amountBet*2;
        } else if(handValue(player) > handValue(dealer)){
            std::cout<<"You win!"<<std::endl;
            wins++;
            money+=amountBet*2;
        } else if(handValue(player) < handValue(dealer)){
            std::cout<<"Dealer wins!"<<std::endl;
            losses++;
        } else{
            std::cout<<"It's a tie!"<<std::endl;
            money+=amountBet;
            ties++;
        }

        // Ask if the player wants to play again
        game = playAgain();
    }
    finalStats(money,games,wins,ties,losses);
}
