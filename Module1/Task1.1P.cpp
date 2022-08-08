#define BTN 2
#define LED 13

void setup() {
	Serial.begin(9600);
	pinMode(BTN, INPUT_PULLUP);
	pinMode(LED, OUTPUT);
}

void loop() {
	int isPressed = !digitalRead(BTN);  // button pulls the pin low

	Serial.println(isPressed);

	if (isPressed) {
		digitalWrite(LED, HIGH);
	} else {
		digitalWrite(LED, LOW);
	}

	delay(500);
}
