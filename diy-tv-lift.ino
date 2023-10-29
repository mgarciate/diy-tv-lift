#define SIMULATOR
#define DEBUG

const long intervalMotorB = 10000;

const int DEFAULT_SPEED_MOTOR_B = 255;
const int switchPin = 10;
const int fcPin = 11;
const int motorBPin = 7;
const int directionMotorBPin = 6;

int switchState = LOW;
int previousSwitchState = LOW;
int fcState = LOW;
int directionMotorBState = HIGH;
int motorBSpeed = 0;

bool isBooting = true;
bool working = false;
bool isReadyToWork = false;

unsigned long currentMillis = 0;
unsigned long previousMillisForMotorB = 0;

void initValues()
{
    directionMotorBState = HIGH;
    motorBSpeed = 0;
    currentMillis = 0;
    previousMillisForMotorB = 0;

    digitalWrite(directionMotorBPin, directionMotorBState);
}

void setup()
{
    pinMode(fcPin, INPUT);
    pinMode(switchPin, INPUT);

    initValues();
#ifdef DEBUG
    // initialize serial communication:
    Serial.begin(9600);
    Serial.println("Start");
#endif
}

void loop()
{
    fcState = digitalRead(fcPin);
    previousSwitchState = switchState;
    switchState = digitalRead(switchPin);

    currentMillis = millis();


    if (isBooting) // Tv lift must go to the initial position
    {
        if (fcState == LOW) // Tv lift isn't closed, we need to close it
        {
            directionMotorBState = HIGH;
            rotateMotorB();
        }
        else // Tv lift is closed, stop the motor
        {
            stopMotorB();
            isBooting = false;
            isReadyToWork = true;
            working = true;
#ifdef DEBUG
            Serial.print("working: ");
            Serial.print(working);
            Serial.print("\n");
#endif
        }
    }
    else if (working) // Tv lift is ready to work
    {
        if ((switchState != previousSwitchState) && isMotorBWorking()) // When tv lift is openning and we turn off the switch, we need to stop the motor and return the motor to the initial position
        {
            stopMotorB();
            isBooting = true;
            isReadyToWork = false;
            working = false;
#ifdef DEBUG
            Serial.print("working: ");
            Serial.print(working);
            Serial.print("\n");
#endif
        }
        else if (switchState == LOW) 
        {
            if (fcState == HIGH) // Tv lift is closed, stop the motor
            {
                stopMotorB();
                isReadyToWork = true;
            }
            else // Tv lift isn't closed, we need to close it
            {
                directionMotorBState = HIGH;
                rotateMotorB();
            }
        }
        else
        {
            if (isReadyToWork) // We want the tv lift to be openned
            {
                if (!isMotorBWorking()) // Open the tv lift, start the motor
                {
                    previousMillisForMotorB = currentMillis;
                    directionMotorBState = LOW;
                    rotateMotorB();
                }
                else if ((currentMillis - previousMillisForMotorB) >= intervalMotorB) // Tv lift is openned, stop the motor
                {
                    stopMotorB();
                    isReadyToWork = false;
                }
            }
        }
    }

#ifdef SIMULATOR
    delay(30);
#endif
}

void rotateMotorB()
{
    if (!isMotorBWorking())
    {
#ifdef DEBUG
        Serial.print("rotateMotorB:");
        Serial.print(directionMotorBState);
        Serial.print("\n");
#endif
        motorBSpeed = DEFAULT_SPEED_MOTOR_B;
        digitalWrite(directionMotorBPin, directionMotorBState);
        analogWrite(motorBPin, motorBSpeed); // PWM Speed Control
    }
}

void stopMotorB()
{
    if (isMotorBWorking())
    {
        motorBSpeed = 0;
        analogWrite(motorBPin, 0); // PWM Speed Control

#ifdef DEBUG
        Serial.println("Motor B stop");
#endif
    }
}

bool isMotorBWorking()
{
    return (motorBSpeed > 0);
}

void customDelay(unsigned long ms)
{
    delay(ms);
    currentMillis = millis();
}