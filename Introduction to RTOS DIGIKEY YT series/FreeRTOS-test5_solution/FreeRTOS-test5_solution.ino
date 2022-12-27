
// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// Settings
static const uint8_t Queue_Max_Length = 5;

// Pins
static const int led_pin = LED_BUILTIN;

// Globals
static QueueHandle_t MY_Queue;
static QueueHandle_t MY_Queue2;

//*****************************************************************************
// Tasks

// Task: wait for item on queue and print it
void Print_Messages_And_Read_Serial(void *parameters)
{

  int item, p, The_delay;
  char c;
  char sample[16] = "delay ";
  char buf[buf_len];

  // Loop forever
  while (1)
  {

    // See if there's a message in the queue (do not block)
    if (xQueueReceive(MY_Queue2, (void *)&item, 0) == pdTRUE)
    {
      Serial.println(item);
    }
    //Serial.println(item);
    // Read characters from serial
    if (Serial.available() > 0)
    {
      c = Serial.read();
      Serial.println(c);
    }
    p = 0;
    for (int8_t i = 0; i < 6; i++)
    {
      if ( c[i] == sample[i])
      {
        p++;
      }
    }
    if (p = 16)
    {
      // send xxx to queue 1
      for (i = 7; i < ( c.sizeof() - 6 ) ; i++ )
      {
        buf[i - 7] = c[i];
      }


      The_delay = atoi(buf);
      if (xQueueSend(MY_Queue, (void *)&The_delay, 10) != pdTRUE)
      {
        Serial.println("Queue full, ops");
      }

    }






  }
}


void Task_no2(void *parameters)
{
  int new_data;
  uint32_t blinks = 0;
  char mesage[] = "Blinked ";
  char Mesage_to_Send[];

  // Loop forever
  while (1)
  {
    if (xQueueReceive(MY_Queue2, (void *)&new_data, 0) == pdTRUE)
    {
      Serial.print("New led delay: ");
      Serial.println(new_data);
    }
    digitalWrite(led_pin, HIGH);
    vTaskDelay(new_data / portTICK_PERIOD_MS);
    digitalWrite(led_pin, LOW);
    vTaskDelay(new_data / portTICK_PERIOD_MS);
    blinks++;
    if (  (blinks % 100) == 0)
    {
      sprintf(Mesage_to_Send, mesage, blinks);
      if (xQueueSend(MY_Queue2, (void *)&Mesage_to_Send, 10) != pdTRUE)
      {
        Serial.println("Queue2 full, ops");
      }
    }


  }
}


//*****************************************************************************
// Main (runs as its own task with priority 1 on core 1)

void setup()
{

  // Configure Serial
  Serial.begin(115200);

  // Wait a moment to start (so we don't miss Serial output)
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS Queue Demo---");

  // Create queue
  MY_Queue  = xQueueCreate(Queue_Max_Length, sizeof(int));
  MY_Queue2 = xQueueCreate(Queue_Max_Length, sizeof(int));

  // Start print task
  xTaskCreatePinnedToCore(Print_Messages_And_Read_Serial,
                          "Print Messages",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_cpu);

  xTaskCreatePinnedToCore(Task_no2,
                          "something here",
                          1024,
                          NULL,
                          1,
                          NULL,
                          app_cpu);

  vTaskDelete(NULL);
}

void loop()
{

  static int num  = 0;

  // Try to add item to queue for 10 ticks, fail if queue is full
  if (xQueueSend(MY_Queue, (void *)&num, 10) != pdTRUE)
  {
    Serial.println("Queue full, ops");
  }
  num++;

  // Wait before trying again
  vTaskDelay(1000 / portTICK_PERIOD_MS);
}
