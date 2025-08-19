#include <WiFi.h>

#include <ESP_Mail_Client.h>

#define WIFI_SSID "Meet"

#define WIFI_PASSWORD "Tobham@2304"

#define SMTP_server "smtp.gmail.com"

#define SMTP_Port 587

#define sender_email "cropmaster63@gmail.com"

#define sender_password "ytgz xaip avos ffja"

#define Recipient_email "meetbhatt2304@gmail.com"

#define Recipient_name "GOD"

SMTPSession smtp;

void setup(){

  Serial.begin(115200);

  Serial.println();

  Serial.print("Connecting...");

  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED)

  { Serial.print(".");

    delay(200);

   }

  Serial.println("");

  Serial.println("WiFi connected.");

  Serial.println("IP address: ");

  Serial.println(WiFi.localIP());

  Serial.println();

  smtp.debug(1);

  ESP_Mail_Session session;

  session.server.host_name = SMTP_server ;

  session.server.port = SMTP_Port;

  session.login.email = sender_email;

  session.login.password = sender_password;

  session.login.user_domain = "";

  /* Declare the message class */

  SMTP_Message message;

  message.sender.name = "ESP 32";

  message.sender.email = sender_email;

  message.subject = "ESP32 Testing Email";

  message.addRecipient(Recipient_name,Recipient_email);

   //Send HTML message

  String htmlMsg = "<div style=\"color:#000000;\"><h1> Hello Semicon!</h1><p> Mail Generated from ESP32</p></div>";

  message.html.content = htmlMsg.c_str();

  message.html.content = htmlMsg.c_str();

  message.text.charSet = "us-ascii";

  message.html.transfer_encoding = Content_Transfer_Encoding::enc_7bit;

 /* //Send simple text message

  String textMsg = "How are you doing";

  message.text.content = textMsg.c_str();

  message.text.charSet = "us-ascii";

  message.text.transfer_encoding = Content_Transfer_Encoding::enc_7bit;*/

  if (!smtp.connect(&session))

    return;

  if (!MailClient.sendMail(&smtp, &message))

    Serial.println("Error sending Email, " + smtp.errorReason());

}

void loop(){

 

}