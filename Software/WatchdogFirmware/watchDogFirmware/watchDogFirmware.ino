#define RST_PIN     8
#define ALIVE_PIN   2
#define LED_PIN     13

const uint16_t defaultDelay      = 1000; //ms
const uint16_t defaultResetDelay = 1000; //ms
const float defaultTimeout       = 20.0; //s
float timeout                    = defaultTimeout;

void setup() {
    pinMode(RST_PIN,OUTPUT);
    pinMode(LED_PIN,OUTPUT);
    pinMode(ALIVE_PIN,INPUT);

    digitalWrite(RST_PIN, HIGH);
    attachInterrupt(digitalPinToInterrupt(ALIVE_PIN), alive, CHANGE);

    Serial.begin(115200);
    Serial.println("Watchdog on duty!");
}

void loop() {
    Serial.print("Timeout:");
    Serial.println(timeout);
    if(timeout < 0.1)
    {
        Serial.println("Woof woof!!,Reset Host!");
        resetHost();
        timeout = defaultTimeout;
    }
    else
    {
        timeout -= defaultDelay/1000.0f;
        delay(defaultDelay);
    }
}

void alive()
{
    Serial.println("Host alive!");
    digitalWrite(LED_PIN,digitalRead(ALIVE_PIN));
    timeout = defaultTimeout;
}

void resetHost()
{
    digitalWrite(RST_PIN, LOW);
    delay(defaultResetDelay);
    digitalWrite(RST_PIN, HIGH);
}
