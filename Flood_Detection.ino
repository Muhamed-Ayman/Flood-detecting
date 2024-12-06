  #include <Adafruit_Sensor.h>
  #include <Adafruit_BMP280.h>
  #include <DHT.h>
  #include <WiFi.h>
  #include <PubSubClient.h>
  ////////////////////////
  // MQTT Topics
  const char* mqttServer              = "broker.hivemq.com";
  const char* dhtTempTopic            = "ESP32/DHTTEMP";
  const char* dhtHumditiyTopic        = "ESP32/DHTHUMIDITY";
  const char* bmpTempTopic            = "ESP32/bmpTemp";
  const char* bmpPressureTopic        = "ESP32/Pressure";
  const char* waterLevelTopic         = "ESP32/water";
  const char* flowRateTopic           = "ESP32/flowRate";
  const char* flowVolumeTopic         = "ESP32/flowVolume";
  const int   mqttPort                = 1883;
  /////////////////////////
  WiFiClient espClient;
  PubSubClient mqttClient(espClient);
  ////////////////////////////
  char bmpTempOut[20];
  char bmpPressureOut[20];
  char dhtTempOut[20];
  char dhtHumidityOut[20];
  char waterOut[20];
  char rateOut[20];
  char volumeOut[20];

  // Pin Definitions
  #define DHT11_PIN 4        // Pin for DHT sensor (GPIO 4)
  #define SOIL_PIN 36       // Analog pin for Soil Moisture sensor
  //#define WATER_LEVEL_PIN 35 // Analog pin for Water Level sensor
  #define FLOW_SENSOR_PIN 23 // Pin for Water Flow sensor
  #define BUZZERPIN   26

  // Instances for sensors
  Adafruit_BMP280 bmp;        // BMP280 instance
  DHT dht11(DHT11_PIN, DHT11); // DHT11 instance

  // Water Flow Sensor variables
  volatile int pulseCount = 0;
  float flowRate = 0.0;
  float flowVolume = 0.0;
  float flowRateInM3perS= 0.0;
  float area = 0.000127;
  float VelocityInM3perS=0.0;
  unsigned long lastTime = 0;

  // Flow Sensor Interrupt Service Routine
  void IRAM_ATTR countPulse() {
    pulseCount++;
  }
  ////////////////////////
  void setup() {
    Serial.begin(115200);
    connectToWiFi();
    connectToMQTT();
    pinMode(BUZZERPIN, OUTPUT);
    // Initialize BMP280
    if (!bmp.begin(0x76)) { // Check BMP280 (use 0x77 if needed)
      Serial.println("BMP280 not found!");
      while (1);
    }

    // Set up Flow Sensor
    pinMode(FLOW_SENSOR_PIN, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(FLOW_SENSOR_PIN), countPulse, FALLING);

    Serial.println("Setup Complete!");
  }

  void loop() {
    // Read Sensor Data
    readBMP280();
    readDHT11();
    readSoilMoisture();
    //readWaterLevel();
    calculateFlowSensor();

    delay(2000); // Delay 2 seconds
  }

  // Function to read BMP280 data
  void readBMP280() {
    float temperature = bmp.readTemperature();
    float pressure = bmp.readPressure() / 100.0F; // Convert to hPa
    //float altitude = bmp.readAltitude(1013.25);  // Replace 1013.25 with your local sea-level pressure

    Serial.print("BMP280 Temperature: "); Serial.print(temperature); Serial.println(" °C");
    Serial.print("BMP280 Pressure: "); Serial.print(pressure); Serial.println(" hPa");
    //Serial.print("BMP280 Altitude: "); Serial.print(altitude); Serial.println(" m");
    sprintf(bmpTempOut, "%.2f", temperature);
    if (!mqttClient.connected()) 
    {
      Serial.print("MQTT not connected,\n attempting to reconnect...");
      connectToMQTT();  // Call your reconnect function
    }
    mqttClient.publish(bmpTempTopic, bmpTempOut);
    sprintf(bmpPressureOut, "%0.2f", pressure);
    if (!mqttClient.connected()) 
    {
      Serial.print("MQTT not connected,\n attempting to reconnect...");
      connectToMQTT();  // Call your reconnect function
    }
    mqttClient.publish(bmpPressureTopic, bmpPressureOut);
  }

  // Function to read DHT11 data
  void readDHT11() {
    float d_temperature = 0;
    float humidity = 0;

    humidity = dht11.readHumidity();
    d_temperature = dht11.readTemperature();

    Serial.print("Humidity: ");
    Serial.print(humidity);
    Serial.print("%");
    Serial.println("Temperature: ");
    Serial.print(d_temperature);
    Serial.print(" C");

    sprintf(dhtTempOut, "%0.2f", d_temperature);
    if (!mqttClient.connected()) 
    {
      Serial.print("MQTT not connected,\n attempting to reconnect...");
      connectToMQTT();  // Call your reconnect function
    }
    mqttClient.publish(dhtTempTopic, dhtTempOut);

    sprintf(dhtHumidityOut, "%0.2f", humidity);
    if (!mqttClient.connected()) 
    {
      Serial.print("MQTT not connected,\n attempting to reconnect...");
      connectToMQTT();  // Call your reconnect function
    }
    mqttClient.publish(dhtHumditiyTopic , dhtHumidityOut);
  }

  // Function to read soil moisture
  void readSoilMoisture() {
    int soilMoistureValue = analogRead(SOIL_PIN);
    // Map the sensor value to a meaningful range
    // Assuming sensor output is 0-4095 and maps to 0-100% water level
    float waterLevelPercent = map(soilMoistureValue, 4095,1300, 0, 100);
    Serial.print("water level danger"); //Serial.println(soilMoistureValue);
    Serial.print(" | Water Level danger: ");
    Serial.print(waterLevelPercent);
    Serial.println("%");
    sprintf(waterOut, "%0.2f", waterLevelPercent);

    if (waterLevelPercent >= 100) { // 85% threshold
        digitalWrite(BUZZERPIN, HIGH); // Turn buzzer on
        delay(2000);
        digitalWrite(BUZZERPIN, LOW); // Turn buzzer off
    }
    if (!mqttClient.connected()) 
    {
      Serial.print("MQTT not connected,\n attempting to reconnect...");
      connectToMQTT();  // Call your reconnect function
    }
    mqttClient.publish(waterLevelTopic, waterOut);
    
  }

  // Function to read water level
  /*void readWaterLevel() {
    int waterLevelValue = analogRead(WATER_LEVEL_PIN);
    Serial.print("Water Level: "); Serial.println(waterLevelValue);
  }*/

  // Function to calculate flow rate and volume
  void calculateFlowSensor() {
    unsigned long currentTime = millis();
    if (currentTime - lastTime > 1000) { // Every 1 second
      flowRate = (pulseCount / 7.5); // Adjust 7.5 based on your sensor's calibration
      flowRateInM3perS = flowRate / 60000  ;
      VelocityInM3perS = (flowRateInM3perS*100) / area;
      //flowVolume += flowRate / 60.0; // Calculate total flow in liters
      pulseCount = 0; // Reset pulse count
      lastTime = currentTime;
    }
if (VelocityInM3perS > 150) { // 15 cm/s = 0.15 m/s
        digitalWrite(BUZZERPIN, HIGH); // Turn buzzer on
        delay(500);
        digitalWrite(BUZZERPIN, LOW); // Turn buzzer off
    }
    //Serial.print("Flow Rate in m^3/s: "); Serial.print(flowRateInM3perS); Serial.println(" M^3/sec");
    Serial.print("Flow Rate in L/min: "); Serial.print(flowRate); Serial.println(" L/min");
    Serial.print("water velocity in cm/s: "); Serial.print(VelocityInM3perS); Serial.println(" cm/sec");
    //Serial.print("Total Flow: "); Serial.print(flowVolume); Serial.println(" L");


    sprintf(rateOut, "%0.2f", flowRate);
    if (!mqttClient.connected()) 
    {
      Serial.print("MQTT not connected,\n attempting to reconnect...");
      connectToMQTT();  // Call your reconnect function
    }
    mqttClient.publish(flowRateTopic, rateOut);

    sprintf(volumeOut, "%0.2f", VelocityInM3perS);
    if (!mqttClient.connected()) 
    {
      Serial.print("MQTT not connected,\n attempting to reconnect...");
      connectToMQTT();  // Call your reconnect function
    }
    mqttClient.publish(flowVolumeTopic, volumeOut);
  }

  void connectToWiFi() 
  {
    WiFi.mode(WIFI_STA); 

    const char* ssid = "ESP32";       // Predefined Wi-Fi SSID
    const char* password = "12345678"; // Predefined Wi-Fi password

    
    Serial.print("Connecting to");
    Serial.println(ssid);

    WiFi.begin(ssid, password);  // Connect to the specified Wi-Fi network

    // Wait until the ESP32 is connected to the Wi-Fi
    int attempt = 0;
    while (WiFi.status() != WL_CONNECTED) 
    {
      delay(1000);
      Serial.print("Connecting ");
      Serial.print("Attempt ");
      Serial.print(++attempt);
    }

    Serial.print("\nWi-Fi connected");
    delay(1000);  // Brief delay to show the connection success message

    Serial.print("IP address:");
    Serial.println(WiFi.localIP());  // Display IP address on the LCD
  }

  void connectToMQTT() 
  {
    mqttClient.setServer(mqttServer, mqttPort);
    while (!mqttClient.connected()) 
    {
      Serial.print("Connecting to MQTT...");
      if (mqttClient.connect("ESP32Client")) 
      {
        Serial.println("Connected to MQTT");
      } 
      else 
      {
        Serial.print("Failed to connect to MQTT, rc=");
        Serial.println(mqttClient.state());
        delay(2000);
      }
    }
  }

  void checkConnection()
  {
    if (WiFi.status() != WL_CONNECTED) 
    {
      Serial.println("Wi-Fi connection lost. Reconnecting...");
      WiFi.reconnect();
      while (WiFi.status() != WL_CONNECTED) 
      {
        delay(1000);
        Serial.print(".");
      }
      Serial.println("");
      Serial.println("Wi-Fi reconnected");
      Serial.print("IP address: ");
      Serial.println(WiFi.localIP());
    }  
  }

  void reconnect() 
  {
    while (!mqttClient.connected()) 
    {
      Serial.print("Attempting MQTT connection...");
      if (mqttClient.connect("ESPClient")) 
      {
        Serial.println("connected");
        
      } 
      else
      {
        Serial.print("failed, rc=");
        Serial.print(mqttClient.state());
        Serial.println(" try again in 5 seconds"); 
        delay(5000);
      }
    }
    
  }
