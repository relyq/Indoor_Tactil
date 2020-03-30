void setup() {
  Serial.begin(9600);

  Serial.print("TFT size is ");
  Serial.print(tft.width());
  Serial.print("x");
  Serial.println(tft.height());

  pinMode(13, OUTPUT);
  pinMode(DHTPIN, INPUT_PULLUP);
  pinMode(FANPIN, OUTPUT);
  pinMode(VAPPIN, OUTPUT);
  pinMode(RIEGOPIN, OUTPUT);
  digitalWrite(RIEGOPIN, 0);
  pinMode(SENSORTIERRAPIN, INPUT_PULLUP);

  dht.begin();

  tft.reset();
  tft.begin(0x9341);
  tft.setRotation(0);
  tft.fillScreen(BLACK);

  HomeScreen();
  Serial.println("Starting Loop");
}