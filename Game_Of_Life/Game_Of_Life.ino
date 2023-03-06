#define BLACK 0x0000
#define WHITE 0xFFFF
#define GREY  0x5AEB

#include <TFT_eSPI.h> // Graphics and font library for ST7735 driver chip
#include <SPI.h>

TFT_eSPI tft = TFT_eSPI();  // Invoke library, pins defined in User_Setup.h

bool board[40][32]; 

void setup(void){
  tft.init();

  tft.setRotation(1);

  tft.fillScreen(BLACK);
  initBoard();
}
void loop(){
  delay(2000);
  tft.fillScreen(BLACK);
  drawBoard();
  updateBoard();
}

void drawBoard(){
  for(int i=0; i<40; i++){
    for(int j=0; j<32;j++){
      if(board[i][j] == true){
        tft.fillRect(i*6,j*4,6,5,WHITE);
      }
    }
  }
}
void updateBoard(){
    bool futureBoard[40][32] ;

   for (int i = 0; i < 40; ++i)
  {
    for (int j = 0; j < 32; ++j)
    {
      futureBoard[i][j] = board[i][j];
    }
  }

  for (int i = 0; i < 40; ++i)
  {
    for (int j = 0; j < 32; ++j)
    {
      if(board[i][j] == true && (!stillAlive(i,j))){
        futureBoard[i][j] = false;
      }else if(board[i][j] == false &&(!stillDead(i,j))){
        futureBoard[i][j] = true;
      }
    }
  }

  for (int i = 0; i < 40; ++i)
  {
    for (int j = 0; j < 32; ++j)
    {
      board[i][j] = futureBoard[i][j];
    }
  }
  
}
bool stillAlive(int x, int y){
  int neighbors = surroundingSquaresAlive(x, y);
  return (neighbors == 2 || neighbors == 3);
}

bool stillDead(int x, int y){
    int neighbors = surroundingSquaresAlive(x, y);
    return (neighbors !=3);

}
int surroundingSquaresAlive(int x, int y){
  int alive =0;
  for(int i = x-1; i <x+2; i++){
      if(i <0 || i> 39) continue;
    for(int j = y-1; j<y+2; j++){
      if(j<0 || j>31) continue;
      if(i==x && j==y) continue;
      if(board[i][j] == true) alive++;

    }
  }
  return alive;
}
void initBoard(){
   for (int i = 0; i < 40; ++i)
  {
    for (int j = 0; j < 32; ++j)
    {
      board[i][j] = false;
    }
  }
  for(int j=5; j<25; j++){ //Line of L
    board[8][j] = true;
  }
  for(int i=8; i<14; i++){ //Bottom line of L
    board[i][24] = true;
  }
  for(int j=5; j<25; j++){ // I
      board[17][j] = true;

  }
  for(int j=5; j<25; j++){ // vertical line of F
      board[20][j] = true;

  }
  for(int i=20; i<24; i++){ //Top Hor. line of F
    board[i][5] = true;
  }
   for(int i=20; i<22; i++){ //Bottom Hor. line of F
    board[i][14] = true;
  }
  for(int j=5; j<25; j++){ // vertical line of E
      board[27][j] = true;

  }
   
  for(int i=27; i<31; i++){ //Top Hor. line of E
    board[i][5] = true;
  }
   
  for(int i=27; i<29; i++){ //Middle Hor. line of F
    board[i][14] = true;
  }
   
  for(int i=27; i<31; i++){ //Top Hor. line of E
    board[i][24] = true;
  }
  
}

