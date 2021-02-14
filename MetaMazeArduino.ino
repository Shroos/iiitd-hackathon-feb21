#include <LiquidCrystal.h>
#include "LedControl.h"

byte wall[8] = {B10101, B11111, B11111, B11111, B11111, B11111, B11111, B11111};

char str[16][2] = {"Pww   www  *   ", "w    w     ww www"};

byte ball[8] = {
  B00000,
  B01110,
  B11011,
  B10001,
  B11011,
  B01110,
  B00000,
  B00000
};

char maze[8][8] = 
{
  "P#.....#",
  ".#.#.#..",
  ".#G###.#",
  ".###.#.#",
  ".....#.#",
  "###.##.#",
  "..#..#.#",
  ".......#"
};

int VRx = A5;
int VRy = A4;
int SW = 2;
int xDrift = -23;
int yDrift = -10;

const int rs = 7, en = 8, d4 = 3, d5 = 4, d6 = 5, d7 = 6;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);

LedControl lc=LedControl(10,12,11,1);

void ShowTitle()
{
  lcd.setCursor(0,0);
  lcd.print("   ** META  **  ");
  lcd.setCursor(0,1);
  lcd.print("   ** MAZES **  ");
}

void setup() 
{
  lcd.begin(16, 2);
  Serial.begin(9600); 
  pinMode(VRx, INPUT);
  pinMode(VRy, INPUT);
  pinMode(SW, INPUT_PULLUP); 
  lc.shutdown(0,false);
  lc.setIntensity(0,1);
  lc.clearDisplay(0);
  ShowTitle();
}

void displayMaze(bool showPlayer, bool showGoal)
{
  for(int i = 0; i < 8; i++)
  {
    int col = 0;
    for(int j = 0; j < 8; j++)
    {
      col = col * 2;
      if(maze[i][j] == '#' || maze[i][j] == 'P' && showPlayer || maze[i][j] == 'G' && showGoal)
        col++;
    }
    lc.setRow(0,i,col);
  }
}

void MazeGame()
{
  int x=0, y=0;
  if(analogRead(VRx)-512 > 256 && x < 15)
  {
    if(str[x+1][y] == '*')
    {
      Serial.println(1);
      return;
    }
    else if(str[x+1][y] != '#')
    {
      str[x][y] = '.';
      str[x+1][y] = '#';
    }
  }
  if(analogRead(VRx)-512 < 256 && x > 0)
  {
    if(str[x][y-1] == '*')
    {
      Serial.println(1);
      return;
    }
    else if(str[x][y-1] != '#')
    {
      str[x][y] = '.';
      str[x][y-1] = '#';
    }
  }
  if(analogRead(VRy)-512 > 256 && y < 1)
  {
    if(str[x][y+1] == '*')
    {
      Serial.println(1);
      return;
    }
    else if(str[x][y+1] != '#')
    {
      str[x][y] = '.';
      str[x][y+1] = '#';
    }
  }
  if(analogRead(VRy)-512 < 256 && y > 0)
  {
    if(str[x][y-1] == '*')
    {
      Serial.println(1);
      return;
    }
    else if(str[x][y-1] != '#')
    {
      str[x][y] = '.';
      str[x][y-1] = '#';
    }
  }
}

void displayFrame(char str[])
{
  lcd.clear();
  lcd.createChar(4, wall);
  lcd.createChar(2, ball);
  for(int i = 0; i < 32; i++)
  {
    lcd.setCursor(i%16, i/16);
    if(str[i] == 'P')
      lcd.write(2);
    else if(str[i] == 'w')
      lcd.write(4);
    else if(str[i] == '*')
      lcd.print("*");
  }
  Serial.println(1);
}

void loop() 
{
  Serial.println(analogRead(VRx)-512);
  Serial.println(analogRead(VRy)-512);
  Serial.println(2048);
  delay(20);

  if(Serial.available())
  {
    char s[16];
    strcpy(s[16], str[0]);
    char a = Serial.read();
    if(a == 'M')
      displayFrame(s);
    else if(a == 'L')
      MazeGame();
  }
  
}
