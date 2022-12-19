// decide that forces us to use only 1 core
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t MyApp_cpu = 0;
#else
static const BaseType_t MyApp_cpu = 1;
#endif

//////////////////////////////////////////////// pin definitions
const int8_t myledpin = LED_BUILTIN;


//////////////////////////////////////////////// tasks
void Blink_Led(void *some_parameter)
{
  while (1)
  {
    digitalWrite(myledpin, HIGH);
    vTaskDelay(500 / portTICK_PERIOD_MS);
// delay is measured in ticks and we want 500ms , so we divide 500 by the number of ticks in a ms
// by default a tick in 1ms BUT it can ne both lareg or smaller
    digitalWrite(myledpin, LOW);
    vTaskDelay(500 / portTICK_PERIOD_MS);

  }
}

//////////////////////////////////////////////// main loops

void setup()
{
  // put your setup code here, to run once:
  pinMode(myledpin, OUTPUT);


//  task to run forever
 xTaskCreatePinnedToCore(    // ESP32 exclusive function use xTaskCreate() for vanilla FreeRTOS BUT that has no core selection parameter
   Blink_Led,   // the function that will be called
   "MMY blinker task",   // task name , not sure WHY it is needed 
   1024,   // stack size (bytes for ESP32, words in vanilla), there is a min that is set in setings but 1k is ok for this
   NULL, // parapeter to be passed to fucntion , i think it is some sort of input data/seting 
   1,  // task priority , larger number is higher priority , max number is set in setting ,25 by default so 0-24 are the available lvls
   NULL,   // task handle , no idea what this is 
   MyApp_cpu);  // run the task on a certain core (this thing is ESP32 ONLY!)
  
//  In vnilla FreeRTOS one needs to call VTaskStartSkedualer() in the main loop in order to start it all (AFTER all tasks were set up)
}

void loop()
{
  // put your main code here, to run repeatedly:
}












