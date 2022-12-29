// You'll likely need this on vanilla FreeRTOS
//#include <timers.h>

// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// Globals
static TimerHandle_t one_shot_timer = NULL;



//////////////////////////////////////////////// pin definitions
const int8_t myledpin = LED_BUILTIN;



// close led after 5s
void myTimerCallback(TimerHandle_t xTimer)
{
  // Print message if timer 1 expired
  if ((uint32_t)pvTimerGetTimerID(xTimer) == 1)
  {
    digitalWrite(myledpin, LOW);
    Serial.println("nothing in serial for 5s");
  }
}

void serialStuff(void *parameters)
{
  char v;

  while (1)
  {
    v = Serial.read();
    Serial.print(v);
    digitalWrite(myledpin, HIGH);
    xTimerStart(one_shot_timer, portMAX_DELAY);
  }
}



void setup()
{
  // put your setup code here, to run once:
  pinMode(myledpin, OUTPUT);

  // Create a one-shot timer
  one_shot_timer = xTimerCreate(
                     "One-shot timer",           // Name of timer
                     5000 / portTICK_PERIOD_MS,  // Period of timer (in ticks)
                     pdFALSE,                    // Auto-reload
                     (void *)0,                  // Timer ID
                     myTimerCallback);           // Callback function


  xTaskCreatePinnedToCore(serialStuff,
                          "serial Stuff task",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_cpu);

  // Delete "setup and loop" task
  vTaskDelete(NULL);




}


void loop()
{
  // put your main code here, to run repeatedly:
}








