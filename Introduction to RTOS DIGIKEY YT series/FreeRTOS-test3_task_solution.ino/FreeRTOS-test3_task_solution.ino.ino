


// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// Some string to print
const unsigned int MAX_MESSAGE_LENGTH = 12;
int off_time = 500;
int on_time = 500;

// Task handles
static TaskHandle_t task_1 = NULL;
static TaskHandle_t task_2 = NULL;

//////////////////////////////////////////////// pin definitions
const int8_t myledpin = LED_BUILTIN;
//*****************************************************************************
// Tasks

// Task: print to Serial Terminal with lower priority
void startTask1(void *parameter)
{
  while (1)
  {
    digitalWrite(myledpin, HIGH);
    vTaskDelay(on_time / portTICK_PERIOD_MS);
    digitalWrite(myledpin, HIGH);
    vTaskDelay(off_time / portTICK_PERIOD_MS);
  }
}

void startTask2(void *parameter)
{
  Serial.begin(300);
  while (1)
  {

    //Create a place to hold the incoming message
    char message[MAX_MESSAGE_LENGTH];
    int message_pos = 0;

    //Read the next available byte in the serial receive buffer
    char inByte = Serial.read();

    //Message coming in (check not terminating character) and guard for over message size
    if ( inByte != '\n' && (message_pos < MAX_MESSAGE_LENGTH - 1) )
    {
      //Add the incoming byte to our message
      message[message_pos] = inByte;
      message_pos++;
    }
    //Full message received...
    else
    {
      //Add null character to string
      message[message_pos] = '\0';
    }
    //Print the message (or do other things)
    Serial.println(message);
    on_time = atoi(message);
    Serial.print("led on time is now: ");
    Serial.println(on_time);



  }
}

void setup()
{
  // put your setup code here, to run once:
  // Configure Serial (go slow so we can watch the preemption)
  //Serial.begin(300);

  // Wait a moment to start (so we don't miss Serial output)
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  //Serial.println();


  // Task to run forever
  xTaskCreatePinnedToCore(startTask1,
                          "Task 1",
                          1024,
                          NULL,
                          1,
                          &task_1,
                          app_cpu);

  // Task to run once with higher priority
  xTaskCreatePinnedToCore(startTask2,
                          "Task 2",
                          1024,
                          NULL,
                          2,
                          &task_2,
                          app_cpu);

}

void loop()
{
  // put your main code here, to run repeatedly:

}
