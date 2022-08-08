#define BTN_A 2
#define BTN_B 3
#define LED 13

volatile int isPressed = 0;

void setup() {
	Serial.begin(9600);
	pinMode(BTN_A, INPUT_PULLUP);
	pinMode(BTN_B, INPUT_PULLUP);
	pinMode(LED, OUTPUT);

	// Because you can only have a single interrupt defined per pin, and I
	// wanted to use this unusual double interrupt design, I've wired the
	// button's output to both pin 2 and pin 3 then defined each interrupt
	// on each of them.
	attachInterrupt(digitalPinToInterrupt(BTN_A), on, FALLING);
	attachInterrupt(digitalPinToInterrupt(BTN_B), off, RISING);
}

void loop() {
	Serial.println(isPressed);
	delay(500);
}

void on() {
	isPressed = 1;
	digitalWrite(LED, HIGH);
}

void off() {
	isPressed = 0;
	digitalWrite(LED, LOW);
}
