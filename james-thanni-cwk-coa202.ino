#include <Wire.h>
#include <Adafruit_RGBLCDShield.h>
#include <utility/Adafruit_MCP23017.h>
#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7
#define RIGHTARROW 0
#define LEFTARROW 1

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

// MENU
bool inMenu = true;
int btnPress;
int screenNumber = 1;
boolean movingScreen = true;
bool practiceOption = false;
bool storyOption = false;
bool settingsOption = false;

// GAME
const char *symbols[] = {"+", "-", "*", "/"};
String S = "", ans = "";
int attempts = 0;
int score = 0;
int practiceScore = 0;
int level = 1;
bool tutorial = false;
bool practicing = false;
bool playing = false;
bool answering = false;
bool patternmatch = false;
bool changeD = false;
bool changeM = false;
bool changeMinN = false;
bool changeMaxN = false;

// SETTINGS
bool settings = false;
int startLength = 3;
int N = startLength;
int M = 2;
double D = 1000;
int maxLevel = 5;

// OTHER
byte a[8] = {0b00000, 0b10000, 0b11000, 0b11100, 0b11100, 0b11000, 0b10000, 0b00000};
byte b[8] = {0b00000, 0b00001, 0b00011, 0b00111, 0b00111, 0b00011, 0b00001, 0b00000};

void setup()
{
  Serial.begin(9600);
  lcd.begin(16, 2);
}

void loop()
{
  if (inMenu)
  {
    mainMenu();
    chooseOption();
  }

  if (tutorial)
  {
    instruction();
  }

  if (practicing)
  {
    runPractice();
  }

  if (playing)
  {
    runStory();
  }

  if (settings)
  {
    runSettings();
  }

  if (changeD)
  {
    changeDelay();
  }

  if (changeMaxN)
  {
    changeMaxLength();
  }

  if (changeMinN)
  {
    changeMinLength();
  }

  if (changeM)
  {
    changeSymbolNumber();
  }
}

void chooseOption()
{
  if (practiceOption)
  {
    int btn = lcd.readButtons();
    if (btn & BUTTON_SELECT)
    {
      inMenu = false;
      tutorial = true;
    }
  }

  if (storyOption)
  {
    int btn = lcd.readButtons();
    if (btn & BUTTON_SELECT)
    {
      inMenu = false;
      playing = true;
    }
  }

  if (settingsOption)
  {
    int btn = lcd.readButtons();
    if (btn & BUTTON_SELECT)
    {
      inMenu = false;
      settings = true;
    }
  }
}

void mainMenu()
{
  int btn = lcd.readButtons();
  if (movingScreen == true)
  {
    switch (screenNumber)
    {
    case 1:
    {
      startScreen();
    }
    break;
    case 2:
    {
      practicePage();
    }
    break;
    case 3:
    {
      storyPage();
    }
    break;
    case 4:
    {
      settingsPage();
    }
    break;
    }
  }

  if (btn != btnPress)
  {
    btnPress = btn;
    if (btnPress & BUTTON_RIGHT)
    {
      movingScreen = true;
      screenNumber++;
    }
    else if (btnPress & BUTTON_LEFT)
    {
      movingScreen = true;
      screenNumber--;
    }
  }
  else
  {
    movingScreen = false;
  }

  if (screenNumber == 5)
  {
    screenNumber = 2;
  }
  else if (screenNumber < 2)
  {
    screenNumber = 4;
  }
}

void startScreen()
{
  lcd.createChar(0, a);
  lcd.createChar(1, b);
  lcd.setCursor(0, 0); // Column, line
  lcd.write(RIGHTARROW);
  lcd.setCursor(1, 0);
  lcd.print("Symbol Match");
  lcd.setCursor(13, 0);
  lcd.write(LEFTARROW);
  lcd.setCursor(0, 1);
  lcd.print("Press <- or ->");
}

void practicePage()
{
  practiceOption = true;
  storyOption = false;
  settingsOption = false;
  int btnpress = lcd.readButtons();
  lcd.clear();
  lcd.setCursor(0, 0); // Column, line
  lcd.print("Practice Mode");
  lcd.setCursor(0, 1);
  lcd.print("=== Page 1 ===");
}

void storyPage()
{
  practiceOption = false;
  storyOption = true;
  settingsOption = false;
  lcd.clear();
  lcd.setCursor(0, 0); // Column, line
  lcd.print("Story Mode");
  lcd.setCursor(0, 1);
  lcd.print("=== Page 2 ===");
}

void settingsPage()
{
  practiceOption = false;
  storyOption = false;
  settingsOption = true;
  lcd.clear();
  lcd.setCursor(0, 0); // Column, line
  lcd.print("Game Settings");
  lcd.setCursor(0, 1);
  lcd.setCursor(0, 1);
  lcd.print("=== Page 3 ===");
}

void backToMenu()
{
  practiceOption = false;
  storyOption = false;
  settingsOption = false;
  practicing = false;
  playing = false;
  settings = false;
  answering = false;
  changeD = false;
  changeMaxN = false;
  changeMinN = false;
  changeM = false;

  inMenu = true;
  startScreen();
  screenNumber = 1;
  mainMenu();
  N = startLength;
  score = 0;
  attempts = 0;

  lcd.setBacklight(WHITE);
}

void generatePattern()
{
  int ctr, val;
  randomSeed(analogRead(0));
  for (ctr = 0; ctr < N; ctr++)
  {
    val = random(0, M);
    S += symbols[val];
  }
}

// PRACTICE MODE
void runPractice()
{
  if (practiceScore < 3)
  {
    if (!answering)
    {
      if (S == "")
      {
        generatePattern();
      }
      showPracticeQuestion();
    }
    else
    {
      while (ans.length() < N)
      {
        practiceTurn();
      }
      if (ans.length() == N)
      {
        checkPracticeAnswer();
      }
    }
  }
}

void showPracticeQuestion()
{
  lcd.clear();
  lcd.setBacklight(WHITE);
  char buffer[10];
  sprintf(buffer, "Sequence:", N - 2);
  lcd.print(buffer);
  for (int pos = 0; pos < N; pos++)
  {
    lcd.setCursor(10 + pos, 0);
    lcd.print(S[pos]);
    delay(500);
  }
  answering = true;
}

void instruction()
{
  lcd.clear();
  lcd.setBacklight(TEAL);
  lcd.setCursor(0, 0);
  lcd.print("+ LEFT, - UP");
  lcd.setCursor(0, 1);
  lcd.print("* RIGHT, / DOWN");
  delay(2000);

  lcd.clear();
  lcd.print("Match the order!");
  lcd.setCursor(0, 1);
  lcd.print("Now: win 3 lvls");
  delay(2000);

  lcd.clear();
  lcd.setBacklight(WHITE);
  tutorial = false;
  practiceScore = 0;
  attempts = 0;
  answering = false;
  practicing = true;
}

void practiceTurn()
{
  int b = lcd.readButtons();
  lcd.setBacklight(WHITE);

  if (b & BUTTON_LEFT)
  {
    ans += symbols[0];
  }
  if (b & BUTTON_UP)
  {
    ans += symbols[1];
  }
  if (b & BUTTON_RIGHT)
  {
    ans += symbols[2];
  }
  if (b & BUTTON_DOWN)
  {
    ans += symbols[3];
  }

  lcd.setCursor(0, 1);
  lcd.print(ans);
  lcd.print("                  ");
}

void checkPracticeAnswer()
{
  for (int i = 0; i < N; i++)
  {
    if (ans[i] == S[i])
    {
      patternmatch = true;
    }
    else
    {
      patternmatch = false;
      break;
    }
  }
  if (patternmatch)
  {
    ans = "";
    lcd.setBacklight(GREEN);
    delay(500);
    if (practiceScore < 2)
    {
      lcd.clear();
    }
    else
    {
      lcd.clear();
      delay(500);
      lcd.print("Levels Complete!");
      delay(1000);
      lcd.setCursor(0, 1);
      lcd.print("< or > for menu");
      lcd.setBacklight(WHITE);
      backToMenu();
    }
    answering = false;
    S = "";
    practiceScore++;
  }
  else
  {
    ans = "";
    lcd.clear();
    lcd.setBacklight(RED);
    delay(500);
    lcd.print("Wrong! Try Again");
    delay(1000);
    answering = false;
  }
}

void showQuestion()
{
  lcd.clear();
  lcd.setBacklight(WHITE);
  char buffer[16];
  sprintf(buffer, "Lvl: %d  Sc: %d", level, score * 10);
  lcd.print(buffer);
  for (int pos = 0; pos < N; pos++)
  {
    lcd.setCursor(pos, 1);
    lcd.print(S[pos]);
    delay(500);
  }
  delay(D * 1.1);
  answering = true;
}

// STORY MODE
void runStory()
{
  if (level != maxLevel)
  {
    storyMode();
  }
  else
  {
    M = 4;
    D = 1000;
    maxLevel = 5;
    winScreen();
  }
}

void storyMode()
{
  if (attempts < 4)
  {
    if (!answering)
    {
      if (S == "")
      {
        generatePattern();
      }
      showQuestion();
    }
    else
    {
      lcd.clear();
      while (ans.length() < N)
      {
        userTurn();
      }
      if (ans.length() == N)
      {
        checkAnswer();
      }
    }
  }
  else
  {
    displayLoseScreen();
    playing = false;
  }
}

void winScreen()
{
  lcd.clear();
  lcd.print(":) You win! :)");
  lcd.setCursor(0, 1);
  lcd.print("Resetting");
  lcd.setCursor(0, 0);
  celebrate();
  delay(250);
  celebrate();
  delay(500);
  level = 1;
  score = 0;
  D = 1000;
  startLength = 3;
  backToMenu();
}

void celebrate()
{
  lcd.setBacklight(RED);
  delay(250);
  lcd.setBacklight(YELLOW);
  delay(250);
  lcd.setBacklight(GREEN);
  delay(250);
  lcd.setBacklight(BLUE);
  delay(250);
  lcd.setBacklight(VIOLET);
  delay(250);
}

void displayLoseScreen()
{
  lcd.clear();
  delay(100);
  lcd.setBacklight(BLUE);
  lcd.print("You Lose :(");
  delay(1000);
  lcd.setCursor(0, 1);
  lcd.print("< or > to retry!");
  delay(100);
  backToMenu();
}

void userTurn()
{
  int b = lcd.readButtons();
  lcd.setBacklight(WHITE);

  if (b & BUTTON_LEFT)
  {
    ans += symbols[0];
  }
  if (b & BUTTON_UP)
  {
    ans += symbols[1];
  }
  if (b & BUTTON_RIGHT)
  {
    ans += symbols[2];
  }
  if (b & BUTTON_DOWN)
  {
    ans += symbols[3];
  }

  lcd.setCursor(0, 0);
  lcd.print(ans);
  lcd.print("                  ");
}

void checkAnswer()
{
  for (int i = 0; i < N; i++)
  {
    if (ans[i] == S[i])
    {
      patternmatch = true;
    }
    else
    {
      patternmatch = false;
      break;
    }
  }

  if (patternmatch)
  {
    ans = "";
    lcd.setBacklight(GREEN);
    delay(500);
    lcd.clear();
    answering = false;
    attempts = 0;
    S = "";
    score++;

    if (score % 3 == 0)
    {
      N++;
      level++;
      D = D / 1.025; // to add extra difficulty, shorten delay each level
    }
  }
  else
  {
    ans = "";
    if (attempts < 2)
    {
      lcd.clear();
      lcd.setBacklight(RED);
      delay(500);
      lcd.print("Wrong! Try Again");
      delay(1000);
      answering = false;
    }
    else if (attempts > 1 & attempts < 3)
    {
      lcd.clear();
      lcd.print("Last try :S");
      lcd.setBacklight(YELLOW);
      delay(1000);
      answering = false;
    }
    attempts++;
  }
}

// GAME SETTINGS
void runSettings()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("-> to navigate");
  lcd.setCursor(0, 1);
  lcd.print("Select to quit");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Up to increase");
  lcd.setCursor(0, 1);
  lcd.print("Down to decrease");
  delay(1000);
  lcd.clear();
  changeD = true;
  settings = false;
}

void changeDelay()
{

  if (D < 500)
  {
    D = 500;
  }
  lcd.setCursor(0, 1);
  lcd.print("Left Btn = Reset");
  lcd.setCursor(0, 0);
  lcd.print("Delay: ");
  double delay_info = D / 1000;
  lcd.print(delay_info);
  delay(750);
  lcd.clear();
  int btnpress = lcd.readButtons();

  
  if (btnpress & BUTTON_SELECT)
  {
    backToMenu();
  }

  if (btnpress & BUTTON_RIGHT)
  {
    changeD = false;
    changeMaxN = true;
  }

  if (btnpress & BUTTON_UP)
  {
    D = D + 100;
    lcd.clear();
  }

  if (btnpress & BUTTON_DOWN)
  {
    D = D - 100;
    lcd.clear();
  }

  if (btnpress & BUTTON_LEFT)
  {
    D = 1000;
  }
}

void changeMaxLength()
{

  if (maxLevel < 3)
  {
    maxLevel = 3;
  }
  else if (maxLevel > 13)
  {
    maxLevel = 13;
  }
  lcd.setCursor(0, 1);
  lcd.print("Left Btn = Reset");
  lcd.setCursor(0, 0);
  lcd.print("Max extra length:");
  lcd.print(maxLevel - 1);
  delay(750);
  lcd.clear();
  int btnpress = lcd.readButtons();

  if (btnpress & BUTTON_SELECT)
  {
    if (maxLevel < startLength)
    {
      startLength = maxLevel - 1;
    }
    backToMenu();
  }

  if (btnpress & BUTTON_RIGHT)
  {
    if (maxLevel < startLength)
    {
      startLength = maxLevel - 1;
    }
    changeMaxN = false;
    changeMinN = true;
  }

  if (btnpress & BUTTON_UP)
  {
    maxLevel++;
    lcd.clear();
  }

  if (btnpress & BUTTON_DOWN)
  {
    maxLevel--;
    lcd.clear();
  }

  if (btnpress & BUTTON_LEFT)
  {
    maxLevel = 5;
  }
}

void changeMinLength()
{

  if (startLength < 1)
  {
    startLength = 1;
  }
  else if (startLength > 12)
  {
    startLength = 12;
  }
  lcd.setCursor(0, 1);
  lcd.print("Left Btn = Reset");
  lcd.setCursor(0, 0);
  lcd.print("Min length: ");
  lcd.print(startLength);
  delay(750);
  lcd.clear();
  int btnpress = lcd.readButtons();

  if (btnpress & BUTTON_SELECT)
  {
    if (startLength > maxLevel)
    {
      maxLevel = startLength + 1;
    }
    backToMenu();
  }

  if (btnpress & BUTTON_RIGHT)
  {
    if (startLength > maxLevel)
    {
      maxLevel = startLength + 1;
    }
    changeMinN = false;
    changeM = true;
  }

  if (btnpress & BUTTON_UP)
  {
    startLength++;
    lcd.clear();
  }

  if (btnpress & BUTTON_DOWN)
  {
    startLength--;
    lcd.clear();
  }

  if (btnpress & BUTTON_LEFT)
  {
    startLength = 3;
  }
}

void changeSymbolNumber()
{

  if (M < 1)
  {
    M = 1;
  }

  if (M > 4)
  {
    M = 4;
  }
  lcd.setCursor(0, 1);
  lcd.print("Left Btn = Reset");
  lcd.setCursor(0, 0);
  lcd.print("Symbols: ");
  lcd.print(M);
  delay(750);
  lcd.clear();
  int btnpress = lcd.readButtons();

  if (btnpress & BUTTON_SELECT)
  {
    backToMenu();
  }

  if (btnpress & BUTTON_RIGHT)
  {
    changeM = false;
    changeD = true;
  }

  if (btnpress & BUTTON_UP)
  {
    M++;
    lcd.clear();
  }

  if (btnpress & BUTTON_DOWN)
  {
    M--;
    lcd.clear();
  }

  if (btnpress & BUTTON_LEFT)
  {
    M = 2;
  }
}
