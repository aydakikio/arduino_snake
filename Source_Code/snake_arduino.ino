#include <U8g2lib.h>
#include <EEPROM.h>

U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, -1, A5, A4);

// EEPROM addresses
#define EEPROM_HIGH_SCORE_ADDR 0
#define EEPROM_MAGIC_ADDR 2
#define EEPROM_MAGIC_VALUE 42

#define GRID_SIZE 6
#define SCREEN_WIDTH 126  
#define SCREEN_HEIGHT 54  

// Max snake length 
#define SNAKE_MAX 150

// Store constant strings in PROGMEM (flash memory) to save RAM
const char str_score[] PROGMEM = "Score:";
const char str_best[] PROGMEM = "Best:";
const char str_you_win[] PROGMEM = "YOU WIN!";
const char str_max_length[] PROGMEM = "Max length!";
const char str_game_over[] PROGMEM = "GAME OVER";
const char str_new_high[] PROGMEM = "NEW HIGH SCORE!";
const char str_press_btn[] PROGMEM = "Press any button..";

struct Point {
  int8_t x, y;
};

Point snake[SNAKE_MAX];
int snake_length = 3;
Point food;

int8_t current_direction = 0;
int8_t next_direction = 0;
bool is_direction_changed = false;

int score = 0;
int high_score = 0;
bool game_over = false;
bool food_eaten = false;
bool new_high_score = false;

// Button pins
#define BTN_RIGHT 10
#define BTN_DOWN 9
#define BTN_LEFT 7
#define BTN_UP 6
#define BTN_RESET 4

unsigned long last_move = 0;
unsigned long last_input = 0;
unsigned long last_draw = 0;
int game_speed = 500;
int min_speed = 100;

#define INPUT_DEBOUNCE 50
#define DRAW_INTERVAL 42  

// Buffer for reading strings from PROGMEM
char buffer[20];

// Track if we need to redraw
bool needs_redraw = true;

void setup() {
  u8g2.begin();

  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_RESET, INPUT_PULLUP);

  loadHighScore();

  snake[0].x = GRID_SIZE * 10;
  snake[0].y = 8 + GRID_SIZE * 4;

  snake[1].x = GRID_SIZE * 9;
  snake[1].y = 8 + GRID_SIZE * 4;

  snake[2].x = GRID_SIZE * 8;
  snake[2].y = 8 + GRID_SIZE * 4;
  generateFood();
  randomSeed(analogRead(A0));
}

void loop() {
  unsigned long current_time = millis();

  if(digitalRead(BTN_RESET)==LOW){
    resetHighScore();
    needs_redraw = true;
    delay(500);
  }

  if (!game_over) {
    // Handle input with debouncing
    if (current_time - last_input > INPUT_DEBOUNCE) {
      if (handleInput()) {
        last_input = current_time;
      }
    }
    
    // Move snake at game speed
    if (current_time - last_move > game_speed) {
      moveSnake();
      checkCollisions();
      if (food_eaten) {
        growSnake();
        generateFood();
        food_eaten = false;
        score++;

        if (score > high_score) {
          high_score = score;
          new_high_score = true;
          saveHighScore();
        }

        // Speed progression
        if (score % 5 == 0 && game_speed > min_speed) {
          game_speed -= 15;
          if (game_speed < min_speed) {
            game_speed = min_speed;
          }
        }
        
        // Win - reached max length!
        if (snake_length >= SNAKE_MAX) {
          game_over = true;
        }
      }
      needs_redraw = true;
      last_move = current_time;
    }

    // Draw at fixed frame rate
    if (needs_redraw && (current_time - last_draw > DRAW_INTERVAL)) {
      drawGame();
      needs_redraw = false;
      last_draw = current_time;
    }
  } else {
    // Draw game over screen at fixed rate
    if (current_time - last_draw > DRAW_INTERVAL) {
      drawGameOver();
      last_draw = current_time;
    }

    if (digitalRead(BTN_RIGHT) == LOW || digitalRead(BTN_LEFT) == LOW || 
      digitalRead(BTN_UP) == LOW || digitalRead(BTN_DOWN) == LOW) {
      resetGame();
      needs_redraw = true;
      delay(500);
    }
  }
}

bool handleInput() {
  bool input_detected = false;
  
  if (digitalRead(BTN_RIGHT) == LOW && current_direction != 1) {
    next_direction = 0;
    is_direction_changed = true;
    input_detected = true;
  }
  else if (digitalRead(BTN_LEFT) == LOW && current_direction != 0) {
    next_direction = 1;
    is_direction_changed = true;
    input_detected = true;
  }
  else if (digitalRead(BTN_UP) == LOW && current_direction != 3) {
    next_direction = 2;
    is_direction_changed = true;
    input_detected = true;
  }
  else if (digitalRead(BTN_DOWN) == LOW && current_direction != 2) {
    next_direction = 3;
    is_direction_changed = true;
    input_detected = true;
  }
  
  return input_detected;
}

void moveSnake() {
  if (is_direction_changed) {
    current_direction = next_direction;
    is_direction_changed = false;
  }

  for (int i = snake_length - 1; i > 0; i--) {
    snake[i] = snake[i - 1];
  }

  switch (current_direction) {
    case 0: snake[0].x += GRID_SIZE; break;
    case 1: snake[0].x -= GRID_SIZE; break;
    case 2: snake[0].y -= GRID_SIZE; break;
    case 3: snake[0].y += GRID_SIZE; break;
  }
}

void checkCollisions() {
  if (snake[0].x < 0 || snake[0].x >= SCREEN_WIDTH || 
      snake[0].y < 8 || snake[0].y >= SCREEN_HEIGHT + 8) {
    game_over = true;
    return;
  }

  for (int i = 1; i < snake_length; i++) {
    if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
      game_over = true;
      return;
    }
  }

  if (snake[0].x == food.x && snake[0].y == food.y) {
    food_eaten = true;
  }
}

void growSnake() {
  if (snake_length < SNAKE_MAX) {
    snake[snake_length] = snake[snake_length - 1];
    snake_length++;
  }
}

void generateFood() {
  if (snake_length >= SNAKE_MAX) {
    return;
  }
  
  bool valid_position = false;

  while (!valid_position) {
    food.x = (random(0, SCREEN_WIDTH / GRID_SIZE)) * GRID_SIZE;
    food.y = 8 + (random(0, SCREEN_HEIGHT / GRID_SIZE)) * GRID_SIZE;

    valid_position = true;
    for (int i = 0; i < snake_length; i++) {
      if (food.x == snake[i].x && food.y == snake[i].y) {
        valid_position = false;
        break;
      }
    }
  }
}

void loadHighScore() {
  byte magic = EEPROM.read(EEPROM_MAGIC_ADDR);
  
  if (magic != EEPROM_MAGIC_VALUE) {
    high_score = 0;
    EEPROM.write(EEPROM_MAGIC_ADDR, EEPROM_MAGIC_VALUE);
    EEPROM.put(EEPROM_HIGH_SCORE_ADDR, high_score);
  } else {
    EEPROM.get(EEPROM_HIGH_SCORE_ADDR, high_score);
  }
}

void saveHighScore() {
  EEPROM.put(EEPROM_HIGH_SCORE_ADDR, high_score);
}

void resetHighScore() {
  high_score = 0;
  new_high_score = false;
  EEPROM.put(EEPROM_HIGH_SCORE_ADDR, high_score);
}

void drawGame() {
  u8g2.clearBuffer();
  
  u8g2.setFont(u8g2_font_5x7_mf);
  
  // Read strings from PROGMEM and draw
  strcpy_P(buffer, str_score);
  u8g2.drawStr(0, 6, buffer);
  u8g2.setCursor(30, 6);
  u8g2.print(score);
  
  strcpy_P(buffer, str_best);
  u8g2.drawStr(70, 6, buffer);
  u8g2.setCursor(95, 6);
  u8g2.print(high_score);

  for (int i = 0; i < snake_length; i++) {
    if (i == 0) {
      u8g2.drawBox(snake[i].x, snake[i].y, GRID_SIZE, GRID_SIZE);
    } else {
      u8g2.drawFrame(snake[i].x, snake[i].y, GRID_SIZE, GRID_SIZE);
    }
  }

  if (snake_length < SNAKE_MAX) {
    u8g2.drawBox(food.x + 1, food.y + 1, GRID_SIZE - 2, GRID_SIZE - 2);
  }
  
  u8g2.drawFrame(0, 8, SCREEN_WIDTH, SCREEN_HEIGHT);
  
  u8g2.sendBuffer();
}

void drawGameOver() {
  u8g2.clearBuffer();
  
  u8g2.setFont(u8g2_font_7x13_mf);
  
  if (snake_length >= SNAKE_MAX) {
    strcpy_P(buffer, str_you_win);
    u8g2.drawStr(30, 20, buffer);
    
    u8g2.setFont(u8g2_font_5x7_mf);
    strcpy_P(buffer, str_max_length);
    u8g2.drawStr(25, 30, buffer);
  } else {
    strcpy_P(buffer, str_game_over);
    u8g2.drawStr(25, 20, buffer);
  }

  u8g2.setFont(u8g2_font_5x7_mf);
  
  if (new_high_score) {
    strcpy_P(buffer, str_new_high);
    u8g2.drawStr(15, 42, buffer);
  }
  
  strcpy_P(buffer, str_score);
  u8g2.drawStr(36, 52, buffer);
  u8g2.setCursor(71, 52);
  u8g2.print(score);
  
  strcpy_P(buffer, str_press_btn);
  u8g2.drawStr(20 , 62 , buffer);
  
  u8g2.sendBuffer();
}

void resetGame() {
  snake_length = 3;

  snake[0].x = GRID_SIZE * 10;
  snake[0].y = 8 + GRID_SIZE * 4;

  snake[1].x = GRID_SIZE * 9;
  snake[1].y = 8 + GRID_SIZE * 4;

  snake[2].x = GRID_SIZE * 8;
  snake[2].y = 8 + GRID_SIZE * 4;

  current_direction = 0;
  next_direction = 0;
  is_direction_changed = false;
  score = 0;
  game_over = false;
  food_eaten = false;
  game_speed = 500;
  new_high_score = false;

  generateFood();
}