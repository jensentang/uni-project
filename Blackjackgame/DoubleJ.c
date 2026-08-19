/***************************************************************************************
    Group Name            :   Double J
    1st Member Name       :   Jensen Tang Rui Xuan
    1st Matric            :   22011935
    2nd  Member Name      :   James Ling Lee Sheng
    2nd Matric            :   22011937

    Semester              :   Sep 2024

-Command to compile the project (modify and include additional files if exist):

gcc DoubleJ.c gfx.c -o DoubleJ.o -lX11 -lm

***********************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include "gfx.h"
#include <unistd.h>
#include <math.h>
#include <ctype.h>

// Constants //
#define WINDOW_WIDTH 1300
#define WINDOW_HEIGHT 800
#define CARD_WIDTH 80
#define CARD_HEIGHT 120

//* ---------------------- Declaring function ---------------------- *//
void start_screen();
void help_screen();
void game_screen();
void card_design();
void create_deck(int deck[52][3]);
void print_deck(int deck[52][3], const char* decktype);
void shuffle_card(int deck[52][3]);
void display_deck(int deck[52][3], const char* decktype);
void play_game(int deck[52][3], int *deckIndex);
int deal_card(int deck[52][3], int *deckIndex, int hand[10][3], int *cardCount);
int calculate_total_player(int hand[10][3], int cardCount);
int calculate_total_dealer(int hand[10][3], int cardCount);
void display_hand(char *owner, int hand[10][3], int cardCount);
void player_turn(int deck[52][3], int *deckIndex, int playerHand[10][3], int *playerCount,int dealerHand[10][3], int dealerCount);
void dealer_turn(int deck[52][3], int *deckIndex, int dealerHand[10][3], int *dealerCount);

//* ---------------------- Main function --------------------------- *//
int total;

int main()
{
    gfx_open(WINDOW_WIDTH, WINDOW_HEIGHT, "Blackjack Game");

    while (true)
    {
        start_screen();
    }
    return 0;
}

//* ---------------------- Function Definition --------------------- *//

void start_screen() // Start Screen
{
    int x,y;
    int ysize = 800;
    int xsize = 1300;
    char *text1="----------SINGLE-PLAYER BLACKJACK----------";
    char *text2="Start A New Game";
    char *text3="Get Help On How To Play The Game";
    char *text4="Exit";
    char c;
    
    srand(time(NULL));

    gfx_open(xsize,ysize,"Blackjack Game");// Open a new window for drawing
	
    //Set the background to green 
    gfx_clear_color(0,92,39);
    gfx_clear();
	
    gfx_color(0,0,0); //black line
    gfx_rectangle (400,225,500,50); //400-900(x),225-275(y)
    gfx_rectangle (400,375,500,50); 
    gfx_rectangle (400,525,500,50); 
    gfx_color(3,52,23); //dark green box
    gfx_fillrectangle(400,225,500,50);
    gfx_fillrectangle(400,375,500,50); 
    gfx_fillrectangle(400,525,500,50);
	
    gfx_color(255,255,255); //white word
    gfx_text(text1,393,100,2);
    gfx_text(text2,450, 240,1);
    gfx_text(text3,450, 390,1);
    gfx_text(text4,450, 540,1);
	
    while (true)
    {
	if (gfx_event_waiting())
	{
          c=gfx_wait();
	  x=gfx_xpos();
	  y=gfx_ypos();
			
	  if ((x>400 && x<900) && (y>225 && y<275))
	  {
	    game_screen();
	  }
				
	  if ((x>400 && x<900) && (y>375 && y<425))
	  {
	    help_screen();
	  }
			
	  if ((x>400 && x<900) && (y>525 && y<575))
	  {
	    exit(0);
	  }
				
	  gfx_flush();
	}	
    }
}

void help_screen() // Help Screen
{
    int x,y;
    int ysize = 800;
    int xsize = 1300;
    char *text1="----------Instruction on How To Play The Game----------";
    char *text2="Single-Player Blackjack is a card game where there is only one player play against the dealer of the cards.";
    char *text3="In Blackjack, regardless of the suit (Diamond, Heart, Spade, Club), cards 2-10 are worth the value of the";
    char *text4="number on the card's face. Jack, Queen and King are worth 10. Ace card can have a value of either 1 or 11.";
    char *text5="AT THE BEGINNING:";
    char *text6="Dealer will deal two cards each, face up, for himself and the player. Player can choose 'Hit' or 'Stand'.";
    char *text7="If he chooses to 'Hit', another card is dealt from the deck, face up. If he chooses to 'Stand', no more card";
    char *text8="is dealt and the total value is calculated from whatever cards Player has in his hand. Player can choose to";
    char *text9="'Hit' as many times to get a higher total value of cards, until he either chooses 'Stand' or he goes 'Bust'";
    char *text10="(i.e. total value of the card goes over 21), which means Player has lost. Once Player chooses to 'Stand' or";
    char *text11="goes 'Bust', Dealer will draw new cards into Dealer's hand.";
    char *text12="AT THE END:"; 
    char *text13="1. If either player or dealer gets a Blackjack in the first deal (i.e. the first two cards) then that person";
    char *text14="automatically wins without drawing any more cards. If both player and dealer get Blackjack in the first deal";
    char *text15="then it is a draw.";
    char *text16="2. If either one of the player or dealer has gone bust, the other person wins.";
    char *text17="3. If both player and dealer go bust, both of them have lost.";
    char *text18="4. If neither went bust, then the one with higher total card value wins. If both player and dealer have";
    char *text19="total card value (without going bust) then it is a draw.";
    char *text20="The goal is to get a set of cards in your hand that has a sum of values which is higher than the dealer's";
    char *text21="hand without going over 21.";
    char *text22="Back";

    char c;

    gfx_clear_color(0,92,39);
    gfx_clear();

    gfx_color(255,255,255); 
    gfx_rectangle (50,70,1100,520);
    gfx_text(text1,250,25,2);
    gfx_text(text2,60, 80,1);
    gfx_text(text3,60, 105,1);
    gfx_text(text4,60, 130,1);
    gfx_text(text5,60, 155,1);
    gfx_text(text6,60, 180,1);
    gfx_text(text7,60, 205,1);
    gfx_text(text8,60, 230,1);
    gfx_text(text9,60, 255,1);
    gfx_text(text10,60, 280,1);
    gfx_text(text11,60, 305,1);
    gfx_text(text12,60, 330,1);
    gfx_text(text13,60, 355,1);
    gfx_text(text14,60, 380,1);
    gfx_text(text15,60, 405,1);
    gfx_text(text16,60, 430,1);
    gfx_text(text17,60, 455,1);
    gfx_text(text18,60, 480,1);
    gfx_text(text19,60, 505,1);
    gfx_text(text20,60, 530,1);
    gfx_text(text21,60, 555,1);

    gfx_color(0,0,0); 
    gfx_rectangle (1000,600,100,50); //1000-1100(x),600-650(y)
    gfx_color(255,255,255);
    gfx_fillrectangle(1000,600,100,50); 

    while (true)
    {
        if (gfx_event_waiting())
        {
          c=gfx_wait();
          x=gfx_xpos();
          y=gfx_ypos();
	        
          while ((x>950 && x<1100) && (y>600 && y<650))
          {
            start_screen();
          }
          
          gfx_flush();
        }
        gfx_color(0,0,0);
        gfx_text(text22,1025, 615,1);
    }
}

void game_screen() 
{
    int deck[52][3];
    int deckIndex = 0;
    create_deck(deck);
    print_deck(deck,"New"); 
    shuffle_card(deck);
    display_deck(deck,"Shuffled");
    int again = 0;
 
    do {       
        play_game(deck, &deckIndex);
    
        // Buttons
        gfx_color(0, 0, 0);
        gfx_rectangle(250, 600, 100, 50); // "Continue" button
        gfx_rectangle(450, 600, 100, 50); // "Exit" button
        
        gfx_color(255, 255, 255);
        gfx_fillrectangle(250, 600, 100, 50);
        gfx_fillrectangle(450, 600, 100, 50);

        gfx_color(0, 0, 0);
        gfx_text("Continue", 260, 630, 1);
        gfx_text("Exit", 475, 630, 1);
        
        // Handle 'Continue' and 'Exit' Button Clicks
        int a, b;
        char choice1 = '\0';
        
        while (true) 
        {
            if (gfx_event_waiting()) 
            {
                gfx_wait();
                a = gfx_xpos(); // Get the coordinate of x//
                b = gfx_ypos(); // Get the coordinate of y//

                // "Continue" button clicked
                if ((a > 250 && a < 350) && (b > 600 && b < 650)) 
                {
                    again = 1;
                    choice1 = 'C';
                }

                // "Exit" button clicked
                if (a > 470 && a < 520 && b > 600 && b < 650)
                {
                    choice1 = 'E';
                    start_screen();
                }
                
                break;
            }
            
            gfx_flush();
        }
        
    } while (again == 1);
}

// Game Logic Functions
void create_deck(int deck[52][3]) // Create deck before shuffle //
{
    int suit, face_value;
    int card_value = 0;
    
    for (suit = 0; suit < 4; suit++) 
    {  
        for (face_value = 1; face_value <= 13; face_value++) 
        {  
            deck[card_value][0] = suit; // Diamond, Heart, Spade, Club //
            deck[card_value][1] = face_value;   // A, 2, 3, 4, 5, 6, 7, 8, 9, 10, J, Q, K //
            
            if (face_value == 1) 
            {
                deck[card_value][2] = 1;   
            } 
            else if (face_value >= 11) 
            {
                deck[card_value][2] = 10;  
            } 
            else 
            {
                deck[card_value][2] = face_value;  
            }
            
            card_value++;
        }
    }
}

void print_deck(int deck[52][3], const char* decktype) // Print out deck with 52 cards //
{
    char *suits[] = {"Diamond", "Heart", "Spade", "Club"};
    char *faces[] = {"Ace", "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King"};
    FILE *fp;
    char filename[]="NewDeck.txt";
    
    printf("\nDeck of Cards:\n");
    fp = fopen(filename, "w"); // Write deck (before shuffle) into NewDeck.txt file //
    
    // From Workbook //
    if (fp==NULL)
    {
      printf("Error: Cannot Open file %s.\n",filename);
    }
    fprintf(fp, "%s Deck of Cards:\n\n", decktype);
    
    for (int i = 0; i < 52; i++) 
    {
      printf("%s - %s (Value: %d)\n" , suits[deck[i][0]],faces[deck[i][1] -1 ], deck[i][2]);
      fprintf(fp,"%s - %s (Value: %d)\n" , suits[deck[i][0]],faces[deck[i][1] -1 ], deck[i][2]);
    }
    printf("-----------------------------");
    fclose(fp);
}

void shuffle_card(int deck[52][3]) // Shuffle randomly deck that was created //
{
    srand(time(0));
    
    for (int i = 0; i < 52; i++)
    {
        int rand_card = rand() % 52;

        // Swap the cards //
        int suit = deck[i][0];
        int face_value = deck[i][1];
        int card_value = deck[i][2];

        deck[i][0] = deck[rand_card][0];
        deck[i][1] = deck[rand_card][1];
        deck[i][2] = deck[rand_card][2];

        deck[rand_card][0] = suit;
        deck[rand_card][1] = face_value;
        deck[rand_card][2] = card_value;
    }
}

void display_deck(int deck[52][3], const char* decktype) // Print out shuffled deck with 52 cards //
{
    char *suits[] = {"Diamond", "Heart", "Spade", "Club"};
    char *faces[] = {"Ace", "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King"};
    FILE *fp;
    char filename[]="ShuffledDeck.txt";
    
    printf("\nShuffled Deck:\n");
    fp = fopen(filename, "w"); // Write deck (after shuffle) into ShuffledDeck.txt file //
    
    // From Workbook //
    if (fp==NULL)
    {
      printf("Error: Cannot Open file %s.\n",filename);
    }
    fprintf(fp, "%s Shuffled Deck:\n\n", decktype);
    
    for (int i = 0; i < 52; i++) 
    {
        printf("%s - %s (Value: %d)\n" , suits[deck[i][0]], faces[deck[i][1] - 1], deck[i][2]);
        fprintf(fp, "%s - %s (Value: %d)\n" , suits[deck[i][0]], faces[deck[i][1] - 1], deck[i][2]);
    }
    printf("-----------------------------");
    fclose(fp);
}

void play_game(int deck[52][3], int *deckIndex) // Main game //
{
    int playerHand[10][3] = {0};
    int dealerHand[10][3] = {0};
    int playerCount = 0;
    int dealerCount = 0;
    int playerTotal = 0;
    int dealerTotal = 0;

    
    // Check if there are at least 10 cards left
    if (52 - *deckIndex < 10) 
    {
        printf("Not enough cards left in the deck. Reshuffling...\n");
        *deckIndex = 0;
        shuffle_card(deck);
        display_deck(deck,"Shuffled");
        
    }
    
    while (true) 
    {
        // Clear and redraw screen
        gfx_clear_color(128, 128, 128); // Grey background
        gfx_clear();
        
        gfx_color(0, 0, 0);
        gfx_rectangle(17, 19, 1261, 761); 
        gfx_color(133,27,29);     //red colour
        gfx_fillrectangle(18, 20, 1260 , 760);
        
        // Title
        gfx_color(255, 255, 255);
        gfx_text("Blackjack Game", 550, 50, 2);
        
        // Deal initial cards
	deal_card(deck, deckIndex, playerHand, &playerCount);
	deal_card(deck, deckIndex, playerHand, &playerCount);

	deal_card(deck, deckIndex, dealerHand, &dealerCount);
	deal_card(deck, deckIndex, dealerHand, &dealerCount);

        // Dealer's Cards
        gfx_color(255,255,255);
        gfx_text("Dealer's Cards:", 100, 50, 1);
        for (int i = 0; i < dealerCount; i++) 
        {
            card_design(dealerHand[i][1], dealerHand[i][0], 100 + (i * 100), 100);
        }

        // Player's Cards
        gfx_color(255,255,255);
        gfx_text("Player's Cards:", 100, 300, 1);
        for (int i = 0; i < playerCount; i++) 
        {
            card_design(playerHand[i][1], playerHand[i][0], 100 + (i * 100), 350);
        }

        // Player's Total
        gfx_text("Player's Total:", 100, 500, 1);
        playerTotal = calculate_total_player(playerHand, playerCount);
        
        // Dealer's Total
        gfx_text("Dealer's Total:", 100, 250, 1);
        dealerTotal = calculate_total_dealer(dealerHand, dealerCount);
       
        // Check blackjack   
        int player_blackjack = calculate_total_player(playerHand, playerCount);
        int dealer_blackjack = calculate_total_dealer(dealerHand, dealerCount);
        
        gfx_color(255, 215, 0); // Gold colour for result annoucement

        if (player_blackjack == 21 || dealer_blackjack == 21) 
        {
            
            if (player_blackjack == 21 && dealer_blackjack == 21) 
            {
                gfx_text("Both player and dealer have Blackjack! It's a draw.", 500, 300, 2);
                
                return; // End game if both have Blackjack
            } 
            else if (player_blackjack == 21) 
            {
                gfx_text("Blackjack! Player wins!", 500, 300, 2);
                
                return; // End game if player has Blackjack
            } 
            else if (dealer_blackjack == 21) 
            {
                gfx_text("Blackjack! Dealer wins!", 500, 300, 2);
                
                return; // End game if dealer has Blackjack
            }
        }

        // Buttons
        gfx_color(0, 0, 0);
        gfx_rectangle(50, 600, 100, 50); // "Hit" button
        gfx_rectangle(50, 700, 100, 50); // "Stand" button

        gfx_color(255, 255, 255);
        gfx_fillrectangle(50, 600, 100, 50);
        gfx_fillrectangle(50, 700, 100, 50);

        gfx_color(0, 0, 0);
        gfx_text("Hit", 75, 630, 1);
        gfx_text("Stand", 65, 730, 1);

        // Handle 'Hit' and 'Stand' Button Clicks
        int x, y;
        char choice = '\0';

        while (true) 
        {
            if (gfx_event_waiting()) 
            {
                gfx_wait();
                x = gfx_xpos();
                y = gfx_ypos();

                // "Hit" button clicked
                if ((x > 50 && x < 150) && (y > 600 && y < 650)) 
                {
                    choice = 'H';
                }

                // "Stand" button clicked
                if ((x > 50 && x < 150) && (y > 700 && y < 750)) 
                {
                    choice = 'S';
                    break;
                }
            }

            // Execute based on player's choice
            if (choice == 'H') 
            {
                deal_card(deck, deckIndex, playerHand, &playerCount);
                playerTotal = calculate_total_player(playerHand, playerCount);
                
                for (int i = 0; i < playerCount; i++) 
                {
                    card_design(playerHand[i][1], playerHand[i][0], 100 + (i * 100), 350);
                }
                choice = '\0';
                usleep(500000);
            }
            
            if (playerTotal == 21 || playerTotal > 21) 
            {
                break;
            }
        }
        
        if (dealerTotal < 17)
        {
            dealer_turn(deck, deckIndex, dealerHand, &dealerCount);
            dealerTotal = calculate_total_dealer(dealerHand, dealerCount);
            for (int i = 0; i < dealerCount; i++) 
            {
                card_design(dealerHand[i][1], dealerHand[i][0], 100 + (i * 100), 100);
            }
            
            usleep(500000);
        }
        
        gfx_color(255, 215, 0);
        // Determine the winner under six conditions //
        if (playerTotal > 21 && dealerTotal <= 21) 
        {
            
            gfx_text("Player busts! Dealer wins.", 500, 300, 2);
        } 
        else if (playerTotal <= 21 && dealerTotal > 21)
        {
            
            gfx_text("Dealer busts! Player wins.", 500, 300, 2);
        }
        else if (playerTotal > 21 && dealerTotal > 21)
        {
         
            gfx_text("Player and Dealer bust! Both of you lost!", 500, 300, 2);
        }
        else if (playerTotal > dealerTotal) 
        {
          
            gfx_text("Player wins!", 500, 300, 2);
        } 
        else if (playerTotal < dealerTotal) 
        {
          
            gfx_text("Dealer wins!", 500, 300, 2);
        } 
        else if (playerTotal <= 21 && dealerTotal <= 21)
        {
           
            gfx_text("It's a draw!", 500, 300, 2);
        }
        
        sleep(2);
        break ;
    }
}

void card_design(int rank, int suit, int xPos, int yPos) // Card Design Function to display cards
{
    char suitz[10];
    
    // Draw the card border
    gfx_color(255, 215, 0);  //gold colour
    
    gfx_rectangle(xPos-2, yPos-2, CARD_WIDTH+2, CARD_HEIGHT+2);  // Card border
    gfx_color(0,0,0);  // black background
    gfx_fillrectangle(xPos, yPos, CARD_WIDTH, CARD_HEIGHT);  // Card fill

    // Card Rank and Suit
    char *suits[] = {"Diamond", "Heart", "Spade", "Club"};
    char *ranks[] = {"A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K"};

    gfx_color(255, 215, 0); // Text color
    
    // Display rank at top-left of card
    gfx_text(ranks[rank - 1], xPos + 10, yPos + 10, 2);  
    
    // Display suit at bottom-left of card
    strcpy(suitz, suits[suit]);
    char suitzz = suitz[0]; // Take the first character to determine the suit
    
    switch (suitzz)
    {
        case 'S':
        {
	  // Spades
	  gfx_color(197,152,71); // gold colour
	  int x3 = xPos+42; // fixed
	  int y3 = yPos+41; // fixed
	  for(int i = 0; i < 24;i+= 2) // upper triangle
	  {	
	      gfx_fillrectangle(x3-i, y3+i,i+1,3);
	      gfx_fillrectangle(x3, y3+i,i+1,3);
	      y3 +=1;
	  }
	  gfx_fillcircle(xPos+30,yPos+80,25);
	  gfx_fillcircle(xPos+53,yPos+80,25);
	  gfx_fillrectangle(xPos+40.5, yPos+70, 4, 35); // long vertical stem
	  
	  // lower triangle under the stem
	  int x2 = xPos+41; //fixed
	  int y2 = yPos+93; //fixed
	  for (int i = 0; i < 7;i+=1)
	  {	
	      gfx_fillrectangle(x2-i, y2+i,i+3,2);
	      gfx_fillrectangle(x2-1, y2+i,i+3,2);
	      y2 +=1;
	  }
	  
	  break;
        }
        
        case 'D':
        {
          // Diamond
	  gfx_color(255, 215, 0); // gold colour
	  int x5 = xPos+42; // fixed
	  int y5 = yPos+48.5; // fixed
	  for(int i = 0; i < 20;i+= 2) // upper triangle 
	  {	
	      gfx_fillrectangle(x5-i, y5+i,i+1,3);
	      gfx_fillrectangle(x5, y5+i,i+1,3);
	      y5 +=1;
	  }
	  
	  int x6 = xPos+43; // fixed
	  int y6 = yPos+98.5; // fixed
          for (int i = 20; i > 0;i-=2) // inverted lower triangle 
          {	
	      gfx_fillrectangle(x6-i, y6-i,i,3);
	      gfx_fillrectangle(x6-1, y6-i,i,3);
	      y6+=1;
	  }
	  
	  break;
        }
        
        case 'C':
        {
          // Clubs
	  gfx_color(197,152,71); // gold colour
	  gfx_fillcircle(xPos+41.5,yPos+61,25);
	  gfx_fillcircle(xPos+30,yPos+80,25);
	  gfx_fillcircle(xPos+53,yPos+80,25);
	  gfx_fillrectangle(xPos+40.5, yPos+70, 4, 35); // long vertical stem
	  
	  //triangle under the stem
	  int x2 = xPos+41; // fixed
	  int y2 = yPos+93; // fixed
	  for (int i = 0; i < 7;i+=1)
	  {	
	      gfx_fillrectangle(x2-i, y2+i,i+3,2);
	      gfx_fillrectangle(x2-1, y2+i,i+3,2);
	      y2+=1;
	  }
	  
	  break;
        }
        
        case 'H':
        {
          // Heart
	  gfx_color(255, 215, 0); // gold colour
	  gfx_fillcircle(xPos+30,yPos+67,25);
	  gfx_fillcircle(xPos+53,yPos+67,25);
	  int x4 = xPos+43; // fixed
	  int y4 = yPos+94; // fixed
          for (int i = 24; i > 0;i-=2) // triangle
          {	
	      gfx_fillrectangle(x4-i, y4-i,i,3);
	      gfx_fillrectangle(x4-1, y4-i,i,3);
	      y4+=1;
	  }
	  
	  break;
        }
        
    }

}

int deal_card(int deck[52][3], int *deckIndex, int hand[10][3], int *cardCount) // Give the cards when round starts or "Hit" //
{
    hand[*cardCount][0] = deck[*deckIndex][0];
    hand[*cardCount][1] = deck[*deckIndex][1];
    hand[*cardCount][2] = deck[*deckIndex][2];
    (*deckIndex)++;
    (*cardCount)++;
    
    // Display number of cards left in the deck //
    gfx_color(255,255,255);
    gfx_text("Cards left in the deck: ", 900, 200, 2);
    char deckIndexText[20];
    
    gfx_color(133,27,29);
    gfx_fillrectangle(900, 250, 200, 50);
    gfx_color(255,255,255);
    sprintf(deckIndexText, "%d", 52 - *deckIndex); // Converts the remaining card count to a string for display.
    gfx_text(deckIndexText, 900, 250, 2);
    usleep(500000);

    return hand[*cardCount - 1][2];
}

int calculate_total_player(int hand[10][3], int cardCount)  // Calculate total of cards of Player //
{
    int total = 0;
    int ace_one = 0;
    
    // Calculate the total and count Ace //
    for (int i = 0; i < cardCount; i++) 
    {
        total += hand[i][2];
        
        if (hand[i][1] == 1) // If the card is an Ace //
        {
            ace_one++;
        }
    }

    // Calculate the total if Ace is 11 //
    int ace_eleven = total + (ace_one * 10); // count each Ace as 11 instead of 1 //
    
    if (ace_eleven > 21) 
    {
        ace_eleven = total; // if the total exceeds 21, revert back to count Ace as 1 //
       
    }
    
    if (total < 100)
    {
        char playerTotal[20];
        gfx_color(133,27,29);
        gfx_fillrectangle(100, 475, 250, 50);
        gfx_color(255,255,255);
        gfx_text("Player's Total: ", 100, 500, 1);
        sprintf(playerTotal, "%d", ace_eleven);
        gfx_text(playerTotal, 250, 500, 1);
    }
      
    // Print the possible values only if Aces are present
    
    if (ace_one > 0) 
    {
        // Print the total //
        char aceplayer1total[20];
        char aceplayer2total[20];
        
        gfx_color(133,27,29);
        gfx_fillrectangle(100, 475, 250, 50);
        gfx_color(255,255,255);
        gfx_text("Player's Total: ", 100, 500, 1);
        sprintf(aceplayer1total, "%d", total);
        gfx_text(aceplayer1total, 250, 500, 1);
        gfx_text("or", 275, 500, 1);
        sprintf(aceplayer2total, "%d", ace_eleven);
        gfx_text(aceplayer2total, 300, 500, 1);
        
    } 
    
    return ace_eleven; // return the best total below 21 //
}

int calculate_total_dealer(int hand[10][3], int cardCount)  // Calculate total of cards of Dealer //
{
    int total = 0;
    int ace_one = 0;
    
    // Calculate the total and count Ace //
    for (int i = 0; i < cardCount; i++) 
    {
        total += hand[i][2];
        
        if (hand[i][1] == 1) // If the card is an Ace //
        {
            ace_one++;
        }
    }
    
    int ace_eleven = total + (ace_one * 10); // count each Ace as 11 instead of 1 //
    if (total < 100)
    {
        char dealerTotal[20];
        gfx_color(133,27,29);
        gfx_fillrectangle(100, 250, 250, 50);
        gfx_color(255,255,255);
        gfx_text("Dealer's Total: ", 100, 250, 1);
        sprintf(dealerTotal, "%d", ace_eleven);
        gfx_text(dealerTotal, 250, 250, 1);
    }
    
    if (ace_one > 0) 
    {
        // Print the total //
        char acedealer1total[20];
        char acedealer2total[20];
        
        gfx_color(133,27,29);
        gfx_fillrectangle(100, 250, 250, 50);
        gfx_color(255,255,255);
        gfx_text("Dealer's Total: ", 100, 250, 1);
        sprintf(acedealer1total, "%d", total);
        gfx_text(acedealer1total, 250, 250, 1);
        gfx_text("or", 275, 250,1);
        sprintf(acedealer2total, "%d", ace_eleven);
        gfx_text(acedealer2total, 300, 250, 1);
    }    
          
    return ace_eleven; // return the best total below 21 //
}

void display_hand(char *owner, int hand[10][3], int cardCount) 
{
    char *suits[] = {"Diamond", "Heart", "Spade", "Club"};
    char *faces[] = {"Ace", "2", "3", "4", "5", "6", "7", "8", "9", "10", "Jack", "Queen", "King"};
    
    printf("%s\n",owner);
    
    for (int i = 0; i < cardCount; i++) 
    {
        printf("%d: %s of %s\n", i + 1, faces[hand[i][1] - 1], suits[hand[i][0]]);
    }
    printf("\n");
    
}

void player_turn(int deck[52][3], int *deckIndex, int playerHand[10][3], int *playerCount,int dealerHand[10][3], int dealerCount) 
{
    char choice;
    int dealer_total = calculate_total_dealer(dealerHand, dealerCount);
    
    do {
        total = calculate_total_player(playerHand, *playerCount);
        
        if (total >= 21) 
        {
          break;
        }
        printf("Total: %d\t\t\t\t", total);
        printf("Total: %d\n\n", dealer_total);
        printf("Do you want to hit (h) or enter any key to stand ? "); // Player's choice //
        scanf(" %c", &choice);
        
        if (choice == 'h' || choice == 'H') 
        {
            deal_card(deck, deckIndex, playerHand, playerCount);    
        }
        
    } while ((choice == 'h' || choice == 'H') && total < 21);
    
}

void dealer_turn(int deck[52][3], int *deckIndex, int dealerHand[10][3], int *dealerCount) 
{

    do {
        total = calculate_total_dealer(dealerHand, *dealerCount);
        
        if (total >= 17) 
        {
          break;
        }
        
        printf("Dealer Hits! \n");
        deal_card(deck, deckIndex, dealerHand, dealerCount);
        
    } while (total < 17);
    
}

