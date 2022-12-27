// Needed for atoi()
#include <stdlib.h>

// Use only core 1 for demo purposes
#if CONFIG_FREERTOS_UNICORE
static const BaseType_t app_cpu = 0;
#else
static const BaseType_t app_cpu = 1;
#endif

// Settings
static const uint8_t buf_len = 20;
int length2 = 0;




// Task: Read from serial terminal
// Feel free to use Serial.readString() or Serial.parseInt(). I'm going to show
// it with atoi() in case you're doing this in a non-Arduino environment. You'd
// also need to replace Serial with your own UART code for non-Arduino.
void readSerial2(void *parameters)
{

  char c;
  char buf[buf_len];
  uint8_t idx = 0;

  // Clear whole buffer
  memset(buf, 0, buf_len);

  // Loop forever
  while (1)
  {

    // Read characters from serial
    if (Serial.available() > 0)
    {
      c = Serial.read();

      // Update delay variable and reset buffer if we get a newline character
      if (c == '\n')
      {
        buf[idx] = c;
        idx++;
      }
      else

      {

        int *ptr = (buf*)pvPortMalloc( idx * sizeof(char) );

        // One way to prevent heap overflow is to check the malloc output
        if (ptr == NULL)
        {
          Serial.println("Not enough heap.");
          vPortFree(NULL);
        }
        else
        {
          for (int i = 0; i < idx; i++)
          {
            ptr[i] = buf[i];
          }
        }


        length2 = idx;
        idx = 0;
      }
    }
  }
}




void PrintSerial2(void *parameters)
{
  while (1)
  {
    if (length2 != 0)
    {
      Serial.print("What was typed: ");
      for ( int i = 0; i < length2 ; i++)
      {
        Serial.print(ptr[i]);
      }

      Serial.println("  ");
      vPortFree(ptr);
    }
  }
}












void setup()
{
  // put your setup code here, to run once:

  // Configure pin


  // Configure serial and wait a second
  Serial.begin(115200);
  vTaskDelay(1000 / portTICK_PERIOD_MS);
  Serial.println("Multi-task memory test");
  Serial.println("Enter a number in to be parced between tasks then printed");

  // Start blink task
  xTaskCreatePinnedToCore(  // Use xTaskCreate() in vanilla FreeRTOS
    PrintSerial2,      // Function to be called
    "Print_Serial",   // Name of task
    1024,           // Stack size (bytes in ESP32, words in FreeRTOS)
    NULL,           // Parameter to pass
    1,              // Task priority
    NULL,           // Task handle
    app_cpu);       // Run on one core for demo purposes (ESP32 only)

  // Start serial read task
  xTaskCreatePinnedToCore(  // Use xTaskCreate() in vanilla FreeRTOS
    ReadSerial2,     // Function to be called
    "Read_Serial",  // Name of task
    1024,           // Stack size (bytes in ESP32, words in FreeRTOS)
    NULL,           // Parameter to pass
    1,              // Task priority (must be same to prevent lockup)
    NULL,           // Task handle
    app_cpu);       // Run on one core for demo purposes (ESP32 only)

  // Delete "setup and loop" task
  vTaskDelete(NULL);
}

void loop()
{
  // put your main code here, to run repeatedly:

}
