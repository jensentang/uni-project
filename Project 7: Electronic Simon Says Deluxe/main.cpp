#include "mbed.h"
#include "LCD_I2C.h"
#include "oled.h"
#include <chrono>
#include <ratio>
#include <vector>
#include <cstdlib>  

// Buttons (PullUp)
DigitalIn startButton(PC_5, PullUp);

DigitalIn button1(PC_6, PullUp);
DigitalIn button2(PC_7, PullUp);
DigitalIn button3(PC_8, PullUp);
DigitalIn button4(PC_9, PullUp);

// LEDs
DigitalOut led1(PA_1);
DigitalOut led2(PA_4);
DigitalOut led3(PA_5);
DigitalOut led4(PA_6);
PwmOut buzzer(PB_6);

// I2C LCD
// SDA = PB_9
// SCL = PB_8
I2C i2c(PB_9, PB_8);

// Change LCD_ADDR_27 to LCD_ADDR_3F if using a 0x3F module
LCD_I2C lcd(&i2c, LCD_ADDR_27);

// SSD1306 OLED Module (Address 0x78 = 0x3C << 1)
Adafruit_SSD1306_I2c oled(i2c, NC, 0x78, 64, 128);

std::vector<int> sequence;
int currentStep = 0;  // Tracks player's current step in the sequence
int currentLevel = 1; // Initialise level
bool isGameActive = false;

Timer inputtime;
int allowedtime = 10;
int lastdisplaytime = -1;

Timer gameRoastTimer;
const int INGAME_ROAST_INTERVAL_MS = 3000;


// ROAST MESSAGES library

const char* inGameRoasts[] = {
    "TICK TOCK!\nFORGOT IT ALREADY?",
    "HESITATING?\nIT IS ONLY 4 BUTTONS!",
    "YOUR BRAIN IS\nLOADING...",
    "SPEED UP!\nTIME IS BLEEDING OUT!",
    "SWEATING YET?",
    "EVEN A GOLDFISH\nREMEMBERS THIS!",
    "MY GRANDMA PLAYS\nFASTER THAN YOU!",
    "ERROR 404:\nBRAIN CELLS MISSING",
    "LAGGING IN REAL\nLIFE, HUH?",
    "WATCHING YOU PLAY\nIS PAINFUL..."
};

const int TOTAL_INGAME_ROASTS = sizeof(inGameRoasts) / sizeof(inGameRoasts[0]);


void playTone(float frequency, float durations) {
    buzzer.period(1.0f / frequency); // Set Pitch/Frequency
    buzzer.write(0.5f);              // 50% volume (PWM duty cycle)
    ThisThread::sleep_for(chrono::milliseconds((int)(durations * 1000)));
    buzzer.write(0.0f);              // Turn off sound
}

void ClearLED(){
    led1=0;
    led2=0;
    led3=0;
    led4=0;
}

void flashLED(int ledNum, int durationMs)
{
    ClearLED();
    if (ledNum == 1){
        led1 = 1;
    }else if (ledNum == 2){
        led2 = 1;
    }else if (ledNum == 3){
        led3 = 1;
    }else if (ledNum == 4){
        led4 = 1;
    }

    ThisThread::sleep_for(chrono::milliseconds(durationMs));
    ClearLED();
}

// OLED DISPLAY FUNCTIONS
void displayInGameRoast() {
    int randomIndex = rand() % TOTAL_INGAME_ROASTS;
    oled.clearDisplay();
    oled.setTextCursor(0, 0);
    oled.setTextSize(1);
    oled.printf("--- ROASTER ---\n");
    oled.printf("LVL %d IN PROGRESS\n", currentLevel);
    oled.printf("-------------------\n");
    oled.printf("%s", inGameRoasts[randomIndex]);
    oled.display();
}

void updateInGameRoast() {
    // Check if 3 seconds have passed since the last roast during gameplay
    if (chrono::duration_cast<chrono::milliseconds>(gameRoastTimer.elapsed_time()).count() >= INGAME_ROAST_INTERVAL_MS) {
        gameRoastTimer.reset();
        displayInGameRoast();
    }
}

void triggerGameOverRoast(bool timeout) {
    oled.clearDisplay();
    oled.setTextCursor(0, 0);
    oled.setTextSize(1);
    oled.printf("=== DEFEAT ===\n\n");
    if (timeout) {
        oled.printf("TOO SLOW!\nU SLEEPING?");
    } else {
        oled.printf("WRONG BUTTON!\nEYES WHERE?");
    }
    oled.display();
}

void displayHome()
{
    lcd.cls();
    lcd.locate(0,0);
    lcd.printf("Simon Says");
    lcd.locate(0,1);
    lcd.printf("Press Any Button");
    lcd.locate(0,2);
    lcd.printf("Start Button Ready");
    lcd.locate(0,3);
    lcd.printf("Waiting...");

    // OLED Home Screen Setup
    oled.clearDisplay();
    oled.setTextCursor(0, 0);
    oled.setTextSize(1);
    oled.printf("=== SIMON SAYS ===\n\nPRESS START!\nIM HERE TO MAKE YOUR LIFE MISERABLE.");
    oled.display();
}

void showStart(){
    lcd.locate(0,3);
    lcd.printf("Start Pressed       ");
    led1 = 1;
    ThisThread::sleep_for(150ms);
    led1 = 0;
    ThisThread::sleep_for(150ms);
}

void showSequence(){
    lcd.cls();
    lcd.locate(0, 0);
    lcd.printf("Level %d", currentLevel);
    lcd.locate(0, 1);
    lcd.printf("Watch Carefully!");
    ThisThread::sleep_for(1000ms);

    int flashtime = 500 - ((currentLevel - 1)*50);
    if(flashtime < 120){
        flashtime = 120;
    }
    int pausetime = flashtime/2;

    int sequenceLen = (int)sequence.size();
    for (int i = 0; i < sequenceLen; i++) {
        flashLED(sequence[i], flashtime); 
        ThisThread::sleep_for(chrono::milliseconds(pausetime));      
    }

    allowedtime = 10 - (currentLevel - 1);
    if(allowedtime < 2){
        allowedtime = 2;
    }
    lcd.locate(0, 2);
    lcd.printf("Your Turn! (%ds) available", allowedtime); // display amount of time before time runs out per button press
    currentStep = 0; // Reset player step counter for the new sequence
    lastdisplaytime = allowedtime; // let last display time to be the last allowed time
    inputtime.reset(); // reset the timer
    inputtime.start(); // start the counting
    gameRoastTimer.reset(); // reset the roasting
    gameRoastTimer.start(); // start it
    displayInGameRoast();   // display
}

// initialise randomisation
void startGame(){
    // 1. Clear previous game sequence
    sequence.clear();
    currentLevel = 1;
    isGameActive = true;
    // 2. Seed the random generator ONCE at start
    srand(Kernel::Clock::now().time_since_epoch().count());
    // 3. Randomize initial 4 LEDs
    for (int i = 0; i < 4; i++) 
    {
        int randomLed = (rand() % 4) + 1; // Pick LED 1, 2, 3, or 4
        sequence.push_back(randomLed);   // Store inside array
    }
    // 4. Play back sequence to player
    showSequence();
}

void gameOver(bool timeout = false) {
    isGameActive = false;
    inputtime.stop();
    gameRoastTimer.stop();
    triggerGameOverRoast(timeout);
    
    // Flash all LEDs to indicate Game Over
    for (int i = 0; i < 3; i++) {
        led1 = 1; led2 = 1; led3 = 1; led4 = 1;
        ThisThread::sleep_for(200ms);
        ClearLED();
        ThisThread::sleep_for(200ms);
    }
    
    lcd.cls();
    lcd.locate(0, 0); lcd.printf("You are so bad"); // display losing quote
    lcd.locate(0, 1); lcd.printf("I can beat you");
    lcd.locate(0, 2); lcd.printf("with my eyes closed");
    ThisThread::sleep_for(3s);
    
    lcd.cls();
    if (timeout){
        lcd.locate(0, 0); lcd.printf("TIME OUT!");
    }else{
        lcd.locate(0, 0); lcd.printf("Wrong Sequence!");
    }
    lcd.locate(0, 1); lcd.printf("GAME OVER!"); 
    lcd.locate(0, 2); lcd.printf("Level Reached: %d", currentLevel);
    lcd.locate(0, 3); lcd.printf("Press Start to retry"); 
}

void nextLevel() {
    lcd.cls();
    lcd.locate(0, 1);
    lcd.printf("Correct! Next Level");
    ThisThread::sleep_for(1000ms);
    currentLevel++;
    // Add 1 extra random LED to sequence
    sequence.push_back((rand() % 4) + 1);
    showSequence();
}

void PlayerInput(int buttonPressed) {
    // 1. Light up pressed LED briefly
    flashLED(buttonPressed, 200);
    // 2. Check if pressed button matches current step in sequence
    if (buttonPressed == sequence[currentStep]) {
        // Correct press!
        currentStep++;
        // 3. Check if player finished the entire sequence
        if (currentStep == (int)sequence.size()) {
            nextLevel();
        } else{
            inputtime.reset(); // reset timer input after each press
        }
    } else {
        // Wrong press!
        gameOver();
    }
}

int main()
{
    // Initialize LCD
    lcd.init();
    
    // Initialize OLED
    oled.begin();
    oled.clearDisplay();
    oled.display();

    ClearLED();
    displayHome();

    while(true)
    {
        // Start Button
        if(!startButton)
        {
            showStart();
            while(!startButton);
            ThisThread::sleep_for(150ms);
            startGame();
        }    

        if (isGameActive){
            // 1. Calculate live countdown time
            int elapsedsec = chrono::duration_cast<chrono::seconds>(inputtime.elapsed_time()).count();
            int remainings = allowedtime - elapsedsec;

            // 2. Check timeout condition
            if (remainings <= 0) {
                gameOver(true); // Timed out
                continue;
            }
            if (remainings != lastdisplaytime) {
                lastdisplaytime = remainings;
                lcd.locate(0, 2);
                lcd.printf("Your Turn! %ds left  ", remainings);

                switch(currentLevel){
                    case 1:
                        playTone(2000.0f, 0.05f);
                        break;
                    case 2:
                        playTone(2100.0f, 0.10f);
                        break;
                    case 3:
                        playTone(2200.0f, 0.15f);
                        break;
                    case 4:
                        playTone(2300.0f, 0.20f);
                        break;
                    case 5:
                        playTone(2400.0f, 0.25f);
                        break;
                    default:
                        playTone(2500.0f, 0.30f);
                        break;
                }
            }
            updateInGameRoast();
            int pressedb = 0;
            if(!button1){
                pressedb = 1;
            }else if(!button2){
                pressedb = 2;
            }else if(!button3){
                pressedb = 3;
            }else if(!button4){
                pressedb = 4;
            }
            if(pressedb != 0) {
                // Wait for button release
                while(!button1 || !button2 || !button3 || !button4);
                ThisThread::sleep_for(50ms);
                PlayerInput(pressedb);
                // Immediately refresh LCD time after press
                lastdisplaytime = allowedtime;
            }
        }
        ThisThread::sleep_for(30ms);
    }
}