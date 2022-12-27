const unsigned int MAX_MESSAGE_LENGTH = 12;
int off_time = 500;
int on_time = 500;
  // put your main code here, to run repeatedly:
  //Create a place to hold the incoming message
  char message[MAX_MESSAGE_LENGTH];
  int message_pos = 0;


void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(300);

}

void loop() 
{


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
  message_pos = 0;



}
