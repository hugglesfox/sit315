#define BTN1 2
#define BTN2 3
#define LED 13

volatile int state = LOW;

void setup() {
	Serial.begin(9600);

	pinMode(BTN1, INPUT_PULLUP);
	pinMode(BTN2, INPUT_PULLUP);
	pinMode(LED, OUTPUT);

	attachInterrupt(digitalPinToInterrupt(BTN1), blink, CHANGE);
	attachInterrupt(digitalPinToInterrupt(BTN2), blink, CHANGE);
}

void loop() {
	Serial.println(state);
	delay(500);
}

void blink() {
	state = !state;
	digitalWrite(LED, state);
}
