#include <U8g2lib.h>

U8G2_SH1106_128X64_NONAME_2_HW_I2C u8g2(U8G2_R0, -1, A5, A4);

//Snake positions
int coloumn = 0;
int row =0;

//snake width: if eat an apple double the width
int snake_width = 4;

//Directions 
//0-> positive_x  1-> negative_x | 2-> posoitave_y | 3 -> negative_y
int current_direction=0;
int next_direction = 0;
bool is_direction_changed = false;

//Score
int score = 0;

void setup() {
  // put your setup code here, to run once:
  u8g2.begin();

  pinMode(10, INPUT);
  pinMode(9 , INPUT);
  pinMode(7, INPUT);
  pinMode(6, INPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_5x7_mf);

    int buttonState = digitalRead(10);//Right
    int row_button_state = digitalRead(9);//Down

    int Left_botton_state=digitalRead(7);//Left
    int up_button_state= digitalRead(6);//UP

  
    u8g2.drawStr(45, 6, "Score: " );
    u8g2.drawBox(coloumn, row,snake_width, 4);

    if (buttonState == HIGH && coloumn < 120 && current_direction!=1) {
      next_direction = 0;
      is_direction_changed=true;
    }

    if(Left_botton_state == HIGH && coloumn >=0 && current_direction !=0){
      next_direction = 1;
      is_direction_changed=true;
    }
    
    if (row_button_state == HIGH && row <60 && current_direction!=2) {
      next_direction = 3;
      is_direction_changed = true;
    }


    if(up_button_state ==HIGH && row>=0 &&current_direction !=3){
      next_direction = 2;
      is_direction_changed = true;
    }



    if(is_direction_changed==true){
      switch(next_direction){
        case 0:
          current_direction = 0;
          break;
        case 1:
          current_direction =1;
          break;
        case 2:
          current_direction =2;
          break;
        case 3:
          current_direction=3;
          break;
      }
    }


    switch(current_direction){
      case 0:
          if(coloumn < 120){
            coloumn++;
          }
        break;
      case 1:
          if(coloumn >= 0){
            coloumn--;
          }
        break;
      case 2:
          if(row >= 0){
            row--;
          }
        break;
      case 3:
          if(row <60){
            row++;
          } 
        break;
    }

    delay(100);
  } while ( u8g2.nextPage() );

}
