/*
 * Test code for reading a 6-channel rc receiver 
 *
 * \note The Teensy 3.1 supports 5V input signals and can be plugged
 *       directly into a receiver. 
 */

/** 
 * pins correspond to the digital IO pins. Change these values if your
 * setup is different.
 */
#define NUM_CHANNELS 6
#define CH0 3
#define CH1 4
#define CH2 5
#define CH3 6
#define CH4 7
#define CH5 8

/**
 * global array of channels for iterating much more conveniently.
 * Do not change.
 */
const uint8_t myChannels[] = {CH0, CH1, CH2, CH3, CH4, CH5};


/**
 * \brief globals relevant to capturing the channel times
 * \note values are modified withing an ISR and must be volatile.
 */
struct rxVals{
    unsigned long channelTimes[5];

    unsigned long startTime;
    unsigned int currChannel;
    unsigned int channelIndex;
} volatile myRX_Vals;

/// Prototypes:
void startTime();
void stopTime();
void prettyPrintChannel(unsigned int channel);
void setup();
void loop();


void setup()
{
    pinMode(CH0, INPUT);
    pinMode(CH1, INPUT);
    pinMode(CH2, INPUT);
    pinMode(CH3, INPUT);
    pinMode(CH4, INPUT);
    pinMode(CH5, INPUT);

    Serial.begin(115200);   // baud rate is irrelevant for Teensy
    
    for (unsigned int i = 0; i < NUM_CHANNELS; ++i)
       attachInterrupt(myChannels[i], startTime, RISING);

    Serial.println(" PPM Receiver test");
}

/**
 * \brief prints the raw channel data microsecond count
 */
void loop()
{

    delay(50);

    Serial.println("PPM Values");
    for (int i = 0; i < NUM_CHANNELS; ++i)
    {
        Serial.print("Ch");
        Serial.print(i);
        Serial.print(": ");
        Serial.println(myRX_Vals.channelTimes[i]);
        prettyPrintChannel(i);
        Serial.println();
    }
    Serial.println();
}


/** 
 * \brief callback fn for evaluating receiver. Starts the timer and 
 *        attaches a falling-edge interrupt to the channel that
 *        triggered this interrupt.
 */
void startTime()
{
    myRX_Vals.startTime = micros();
    /// Find out which pin triggered the interrupt and set it to trigger
    /// on the next falling edge
    for (unsigned int i = 0; i < NUM_CHANNELS; ++i)
    {
        boolean val = digitalRead(myChannels[i]);
        if (val)
        {
            attachInterrupt(myChannels[i], stopTime, FALLING);
            myRX_Vals.currChannel = myChannels[i];
            myRX_Vals.channelIndex = i;
            break;
        }
        /// else.. this shouldn't happen.
    }
}


/**
 * \brief callback fn that logs the microsecond count of a channel and
 *        reattaches the rising-edge interrupt for that channel.
 */
void stopTime()
{
    myRX_Vals.channelTimes[myRX_Vals.channelIndex] = micros() - 
                                                     myRX_Vals.startTime;
    attachInterrupt(myRX_Vals.currChannel, startTime, RISING);
}



/*
 * \brief prints bars proportional to the raw microsecond count of
 *        a channel.
 */
void prettyPrintChannel(unsigned int myChannel)
{
    unsigned int numEquals;
    if (myRX_Vals.channelTimes[myChannel] < 3000)
        numEquals = myRX_Vals.channelTimes[myChannel] / 100;
    else 
    {
        Serial.print("x");
        return;
    }
    for (unsigned int i = 0; i < numEquals; ++i)
        Serial.print("=");
}

