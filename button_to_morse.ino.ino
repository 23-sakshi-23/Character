const int buttonPin = 2;          // Pin for the button
unsigned long pressStartTime;     // Time when button press starts
float pressDuration;              // How long the button is held (in seconds)
bool isPressed = false;           // State of button press
unsigned long debounceDelay = 50; // Debounce delay in milliseconds
unsigned long lastDebounceTime = 0;
unsigned long lastReleaseTime = 0; // Time when button was last released
unsigned long currentTime = 0;
bool spacePrinted = false;         // To track if space has been printed

void setup() {
  pinMode(7, INPUT);
  pinMode(11, OUTPUT);
  Serial.begin(9600);  // Initialize Serial communication at 9600 baud rate
}

void loop() {
  currentTime = millis();         // Get the current time
  int buttonState = digitalRead(7);  // Read the state of the button

  // Debouncing logic
  if (buttonState == HIGH && (currentTime - lastDebounceTime) > debounceDelay) {
    if (!isPressed) {
      // Button is pressed down, mark the time
      pressStartTime = currentTime;
      isPressed = true;
      spacePrinted = false;       // Reset space printed flag since input is being received
    }
    lastDebounceTime = currentTime; // Update the debounce timer
  }

  if (buttonState == LOW && isPressed && (currentTime - lastDebounceTime) > debounceDelay) {
    // Button is released, calculate duration
    pressDuration = (currentTime - pressStartTime) / 1000.0;  // Convert ms to seconds

    // Determine if it's a dot or dash
    if (pressDuration > 0.10 && pressDuration < 0.3) {  // Short press (< 0.3 seconds)
      digitalWrite(11, HIGH);
      delay(100);
      digitalWrite(11, LOW);
      Serial.print(".");
    } else if (pressDuration > 0.3 && pressDuration < 0.590) {  // Long press (>= 0.3 seconds)
      digitalWrite(11, HIGH);
      delay(300);
      digitalWrite(11, LOW);
      Serial.print("-");
    }

    isPressed = false;  // Reset press state
    lastDebounceTime = currentTime;  // Update debounce time
    lastReleaseTime = currentTime;   // Mark when button was released
  }

  // Check if no input was given for more than 0.3 seconds and space has not been printed yet
  if (!isPressed && (currentTime - lastReleaseTime > 600) && !spacePrinted) {
    Serial.print(" ");  // Print space after 0.3 seconds of no input
    spacePrinted = true;  // Ensure space is only printed once per idle period
  }
}
