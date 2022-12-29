// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// Settings
static const uint16_t timer_divider = 8;     // clock is 80MHz so timer is 10MHz now
static const uint64_t timer_max_count = 100000;  // 10MHz /100000 ==> 10Hz


// Pins
static const int adc_pin = A0;


// Globals
static hw_timer_t *timer = NULL;
float My_average;

//circular buffer stuff
int8_t buffer_length = 20;
int MY_Buffer[buffer_length];
int idx=0;
int idx2=0; // index for the averageing task


// task control
static SemaphoreHandle_t Average_protect;  // protects the agerage while it is written to or read from
static SemaphoreHandle_t avg_triger;  // protects the agerage while it is written to or read from

//////////////////////////////////////////////////////////////////////////
//tasks

// Interrupt Service Routines (ISRs)

// This function executes when timer reaches max (and resets)
void IRAM_ATTR ADC_reader_task()
{

  BaseType_t task_woken = pdFALSE;

  // Perform action (read from ADC)
  MY_Buffer[idx] = analogRead(adc_pin);
  idx++;
  if(idx >= buffer_length)
  {
    idx=0;
  }

  //notify other task that there are 10 values to average out
  if(idx==0 || idx==10)
  {
      xSemaphoreGive(avg_triger);
  }

  // Give semaphore to tell task that new value is ready
  xSemaphoreGiveFromISR(bin_sem, &task_woken);

  // Exit from ISR (Vanilla FreeRTOS)
  //portYIELD_FROM_ISR(task_woken);

  // Exit from ISR (ESP-IDF)
  if (task_woken)
  {
    portYIELD_FROM_ISR();
  }
}


// task A average out the values from the buffer when there are 10 in there

void Averager(void *parameters)
{
   int sum =0;
  for( int i=0; i<10; i++)
  {
    sum=sum+MY_Buffer[i+idx2];
  }


  My_average=sum/10;
  
}


//task B the serial task

void printValues(void *parameters)
{
  char My_txt;
  // Loop forever, wait for semaphore, and print value
  while (1)
  {
    My_txt = serial.read();
    if (My_txt == "avg" || My_txt == "AVG")
    {
      Serial.print("The average is: ");
      //protect global variable here
      if (xSemaphoreTake(Average_protect, 0) == pdTRUE)
      {
        Serial.print(My_average);
        //end protection
        xSemaphoreGive(mutex);
      }

    }
    else
    {
      Serial.println(My_txt);
    }

  }
}

/////////////////////////////////////////////////////////////////////////////////////////////
// main loops


void setup()
{

  // Configure Serial
  Serial.begin(115200);

  // Wait a moment to start (so we don't miss Serial output)
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println();
  Serial.println("---FreeRTOS ISR Buffer chalendhe my solution---");

    // Create mutex before starting tasks
  My_average = xSemaphoreCreateMutex();
  avg_triger = xSemaphoreCreateMutex();


  // Start task to print out results (higher priority!)
  xTaskCreatePinnedToCore(printValues,
                          "Prints values",
                          1024,
                          NULL,
                          2,
                          NULL,
                          app_cpu);

  // Delete "setup and loop" task
  vTaskDelete(NULL);

}

void loop()
{
  // Do nothing, forever
}



