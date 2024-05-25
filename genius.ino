#define RED_LED 5
#define GREEN_LED 4
#define YELL_LED 3
#define BLUE_LED 2

#define BUTTON_RED 11
#define BUTTON_GREEN 10
#define BUTTON_YELLOW 9
#define BUTTON_BLUE 8

#define SEQUENCY_LENGTH 40  // Define a high limit for the sequence length

enum states {
  READY_FOR_THE_NEXT_ROUND,
  USER_RESPONSE,
  GAME_OVER_FAIL
};

int lights[SEQUENCY_LENGTH];
int currentRound = 0;
int ledAnswer = 0;
int gameState = READY_FOR_THE_NEXT_ROUND;

void setup() {
  portStarter();
  Serial.begin(9600);
  initGame();
}

void portStarter() {
  pinMode(RED_LED, OUTPUT);
  pinMode(BLUE_LED, OUTPUT);
  pinMode(YELL_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  pinMode(BUTTON_RED, INPUT_PULLUP);
  pinMode(BUTTON_GREEN, INPUT_PULLUP);
  pinMode(BUTTON_YELLOW, INPUT_PULLUP);
  pinMode(BUTTON_BLUE, INPUT_PULLUP);
}

void initGame() {
  int game = analogRead(0);
  randomSeed(game);
  for(int i = 0; i < SEQUENCY_LENGTH; i++) {
    lights[i] = randomColor();
    delay(200);
  }
}

int randomColor() {
  int colors[] = {BLUE_LED, YELL_LED, GREEN_LED, RED_LED};
  return colors[random(0, 4)];
}

void loop() {
  switch (gameState) {
    case READY_FOR_THE_NEXT_ROUND:
      allBlink();
      Serial.println("Pronto para a proxima rodada");
      createNewRound();
      gameState = USER_RESPONSE;
      break;
    case USER_RESPONSE:
      Serial.println("Jogador respondendo");
      checkPlayerAnswer();
      break;
    case GAME_OVER_FAIL:
      sequencialBlink();
      Serial.println("Jogo finalizado com falha");
      delay(1000);
      resetGame();
      break;
  }
  delay(500);
}

void createNewRound() {
  currentRound++;
  ledAnswer = 0;
  roundLedBlink();
}

void processPlayerAnswer(int buttonPressed) {
  if (lights[ledAnswer] == buttonPressed) {
    ledBlink(buttonPressed);
    ledAnswer++;
    if (ledAnswer == currentRound) {
      gameState = actualState();
    }
  } else {
    gameState = GAME_OVER_FAIL;
  }
}

int actualState() {
  return READY_FOR_THE_NEXT_ROUND;
}

void roundLedBlink() {
  for (int i = 0; i < currentRound; i++) {
    ledBlink(lights[i]);
    delay(1000);
  }
}

void checkPlayerAnswer() {
  if(digitalRead(BUTTON_RED) == LOW) {
    processPlayerAnswer(RED_LED);
  } else if(digitalRead(BUTTON_GREEN) == LOW) {
    processPlayerAnswer(GREEN_LED);
  } else if(digitalRead(BUTTON_YELLOW) == LOW) {
    processPlayerAnswer(YELL_LED);
  } else if(digitalRead(BUTTON_BLUE) == LOW) {
    processPlayerAnswer(BLUE_LED);
  }
}

void sequencialBlink() {
  ledBlink(RED_LED);
  ledBlink(GREEN_LED);
  ledBlink(YELL_LED);
  ledBlink(BLUE_LED);
}

void allBlink() {
  digitalWrite(RED_LED, HIGH);
  digitalWrite(BLUE_LED, HIGH);
  digitalWrite(YELL_LED, HIGH);
  digitalWrite(GREEN_LED, HIGH);
  delay(1000);
  digitalWrite(RED_LED, LOW);
  digitalWrite(BLUE_LED, LOW);
  digitalWrite(YELL_LED, LOW);
  digitalWrite(GREEN_LED, LOW);
  delay(1000);
}

void ledBlink(int ledport) {
  digitalWrite(ledport, HIGH);
  delay(300); // Increased delay for slower blinking
  digitalWrite(ledport, LOW);
  delay(300); // Increased delay for slower blinking
}

void resetGame() {
  currentRound = 0;
  ledAnswer = 0;
  gameState = READY_FOR_THE_NEXT_ROUND;
  initGame();
}
