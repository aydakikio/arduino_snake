#include <U8g2lib.h>

U8G2_SH1106_128X64_NONAME_2_HW_I2C u8g2(U8G2_R0, -1, A5, A4);

// Snake properties
#define MAX_SNAKE_LENGTH 100
#define GRID_SIZE 4
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64

struct Point {
  int x, y;
};

Point snake[MAX_SNAKE_LENGTH];
int snake_length = 3;
Point food;

// Directions: 0=right, 1=left, 2=up, 3=down
int current_direction = 0;
int next_direction = 0;
bool is_direction_changed = false;

// Game state
int score = 0;
bool game_over = false;
bool food_eaten = false;

// Button pins
#define BTN_RIGHT 10
#define BTN_DOWN 9
#define BTN_LEFT 7
#define BTN_UP 6

// Timing
unsigned long last_move = 0;
int game_speed = 600; // milliseconds - much slower starting speed

void setup() {
  Serial.begin(9600);
  u8g2.begin();
  
  // Initialize button pins
  pinMode(BTN_RIGHT, INPUT);
  pinMode(BTN_DOWN, INPUT);
  pinMode(BTN_LEFT, INPUT);
  pinMode(BTN_UP, INPUT);
  
  // Initialize snake at center of screen
  snake[0].x = 32;  // Head
  snake[0].y = 32;
  snake[1].x = 28;  // Body
  snake[1].y = 32;
  snake[2].x = 24;  // Tail
  snake[2].y = 32;
  
  // Generate first food
  generateFood();
  
  randomSeed(analogRead(A0));
}

void loop() {
  if (!game_over) {
    handleInput();
    
    // Move snake based on timing
    if (millis() - last_move > game_speed) {
      moveSnake();
      checkCollisions();
      if (food_eaten) {
        growSnake();
        generateFood();
        food_eaten = false;
        score++;
        
        // Speed up every 3 points, more noticeable progression
        if (score % 3 == 0 && game_speed > 80) {
          game_speed -= 20; // Bigger speed jumps
        }
      }
      last_move = millis();
    }
    
    drawGame();
  } else {
    drawGameOver();
    
    // Reset game if any button is pressed
    if (digitalRead(BTN_RIGHT) == HIGH || digitalRead(BTN_LEFT) == HIGH || 
        digitalRead(BTN_UP) == HIGH || digitalRead(BTN_DOWN) == HIGH) {
      resetGame();
      delay(500); // Debounce
    }
  }
}

void handleInput() {
  int right_state = digitalRead(BTN_RIGHT);
  int down_state = digitalRead(BTN_DOWN);
  int left_state = digitalRead(BTN_LEFT);
  int up_state = digitalRead(BTN_UP);
  
  // Prevent reverse direction
  if (right_state == HIGH && current_direction != 1) {
    next_direction = 0;
    is_direction_changed = true;
  }
  if (left_state == HIGH && current_direction != 0) {
    next_direction = 1;
    is_direction_changed = true;
  }
  if (up_state == HIGH && current_direction != 3) {
    next_direction = 2;
    is_direction_changed = true;
  }
  if (down_state == HIGH && current_direction != 2) {
    next_direction = 3;
    is_direction_changed = true;
  }
}

void moveSnake() {
  // Update direction if changed
  if (is_direction_changed) {
    current_direction = next_direction;
    is_direction_changed = false;
  }
  
  // Move body segments (from tail to head)
  for (int i = snake_length - 1; i > 0; i--) {
    snake[i] = snake[i - 1];
  }
  
  // Move head based on direction
  switch (current_direction) {
    case 0: // Right
      snake[0].x += GRID_SIZE;
      break;
    case 1: // Left
      snake[0].x -= GRID_SIZE;
      break;
    case 2: // Up
      snake[0].y -= GRID_SIZE;
      break;
    case 3: // Down
      snake[0].y += GRID_SIZE;
      break;
  }
}

void checkCollisions() {
  // Check wall collision
  if (snake[0].x < 0 || snake[0].x >= SCREEN_WIDTH || 
      snake[0].y < 8 || snake[0].y >= SCREEN_HEIGHT) {
    game_over = true;
    return;
  }
  
  // Check self collision
  for (int i = 1; i < snake_length; i++) {
    if (snake[0].x == snake[i].x && snake[0].y == snake[i].y) {
      game_over = true;
      return;
    }
  }
  
  // Check food collision
  if (snake[0].x == food.x && snake[0].y == food.y) {
    food_eaten = true;
  }
}

void growSnake() {
  if (snake_length < MAX_SNAKE_LENGTH) {
    // Add new segment at the tail
    snake[snake_length] = snake[snake_length - 1];
    snake_length++;
  }
}

void generateFood() {
  bool valid_position = false;
  
  while (!valid_position) {
    // Generate random position aligned to grid
    food.x = (random(0, SCREEN_WIDTH / GRID_SIZE)) * GRID_SIZE;
    food.y = (random(2, SCREEN_HEIGHT / GRID_SIZE)) * GRID_SIZE; // Avoid score area
    
    // Make sure food doesn't spawn on snake
    valid_position = true;
    for (int i = 0; i < snake_length; i++) {
      if (food.x == snake[i].x && food.y == snake[i].y) {
        valid_position = false;
        break;
      }
    }
  }
}

void drawGame() {
  u8g2.firstPage();
  do {
    // Draw score
    u8g2.setFont(u8g2_font_5x7_mf);
    u8g2.drawStr(0, 6, "Score:");
    u8g2.setCursor(30, 6);
    u8g2.print(score);
    
    // Draw snake
    for (int i = 0; i < snake_length; i++) {
      if (i == 0) {
        // Draw head slightly different (filled circle)
        u8g2.drawBox(snake[i].x, snake[i].y, GRID_SIZE, GRID_SIZE);
      } else {
        // Draw body segments
        u8g2.drawFrame(snake[i].x, snake[i].y, GRID_SIZE, GRID_SIZE);
      }
    }
    
    // Draw food (small filled circle)
    u8g2.drawBox(food.x + 1, food.y + 1, GRID_SIZE - 2, GRID_SIZE - 2);
    
    // Draw border
    u8g2.drawFrame(0, 8, SCREEN_WIDTH, SCREEN_HEIGHT - 8);
    
  } while (u8g2.nextPage());
}

void drawGameOver() {
  u8g2.firstPage();
  do {
    u8g2.setFont(u8g2_font_7x13_mf);
    
    // Center text
    u8g2.drawStr(25, 25, "GAME OVER");
    
    u8g2.setFont(u8g2_font_5x7_mf);
    u8g2.drawStr(35, 40, "Score:");
    u8g2.setCursor(65, 40);
    u8g2.print(score);
    
    u8g2.drawStr(25, 55, "Press any key");
    
  } while (u8g2.nextPage());
}

void resetGame() {
  // Reset snake
  snake_length = 3;
  snake[0].x = 32;
  snake[0].y = 32;
  snake[1].x = 28;
  snake[1].y = 32;
  snake[2].x = 24;
  snake[2].y = 32;
  
  // Reset game state
  current_direction = 0;
  next_direction = 0;
  is_direction_changed = false;
  score = 0;
  game_over = false;
  food_eaten = false;
  game_speed = 600; // Reset to slower starting speed
  
  // Generate new food
  generateFood();
}                                                                  