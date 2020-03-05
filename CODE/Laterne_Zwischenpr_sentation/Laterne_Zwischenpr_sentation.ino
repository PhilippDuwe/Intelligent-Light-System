//Konstanten für den Ultraschallsensor
#define Echo_EingangsPin_1 7 // Echo Eingangs-Pin
#define Trigger_AusgangsPin_1 8 // Trigger Ausgangs-Pin
#define Echo_EingangsPin_2 9 // Echo Eingangs-Pin
#define Trigger_AusgangsPin_2 11 // Trigger Ausgangs-Pin

#define Ultraschall_Delay 80 // Wartedauer nach Ultraschallmessung


//Konstanten für die Led-Steuerung
#define PvmPin 6 // Steuert die Helligkeit

//Konstanten für den Helligkeitssensor
#define sensorPin A5 // Input Pin für das Potentiometer

//Lichtstärke Stufen
#define aus 0
#define gedimmt 20
#define hell 250

//Zeit die die Lampe an bleibt
#define Leucht_Dauer 10000

//Geschwindigkeit der Led Dimmung (hoch -> langsam)
#define Led_Dimming 20


//Variablen für den Ultraschallsensor
int maximumRange = 200;
int minimumRange = 10;
long Abstand;
long Dauer;
boolean wasTrueS1[3] = {false, false, false};
boolean wasTrueS2[3] = {false, false, false};


//Variablen für den Helligkeitssensor
int sensorValue = 0; // Speichert die Sensordaten
int sensorDark = 450; //Wert der unterschritten werden muss, damit der Raum als dunkel gilt

//Variable für die Led-Steuerung
int currPower = 0;

void setup() {
  //Ultraschall Sensor
  pinMode(Trigger_AusgangsPin_1, OUTPUT);
  pinMode(Echo_EingangsPin_1, INPUT);
  pinMode(Trigger_AusgangsPin_2, OUTPUT);
  pinMode(Echo_EingangsPin_2, INPUT);

  //Led-Steuer-Pin
  pinMode(PvmPin, OUTPUT);

  Serial.begin (9600);
}



void ultraschall_1() {
  // Abstandsmessung wird mittels des 10us langen Triggersignals gestartet
  digitalWrite(Trigger_AusgangsPin_1, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trigger_AusgangsPin_1, LOW);
  Dauer = pulseIn(Echo_EingangsPin_1, HIGH);
  Abstand = Dauer / 58.2;
  if (Abstand >= maximumRange || Abstand <= minimumRange) {
    wasTrueS1[0] = false;
    wasTrueS1[1] = false;
    wasTrueS1[2] = false;
    // Pause zwischen den einzelnen Messungen
    delay(Ultraschall_Delay);
  }
  else {
    Serial.println("---MessbereichS1---");
    if (wasTrueS1[0] && wasTrueS1[1]) {
      wasTrueS1[2] = true;
    } else if (wasTrueS1[0]) {
      wasTrueS1[1] = true;
    } else {
      wasTrueS1[0] = true;
    }
    // Pause zwischen den einzelnen Messungen
    delay(Ultraschall_Delay);
  }
}

void ultraschall_2() {
  // Abstandsmessung wird mittels des 10us langen Triggersignals gestartet
  digitalWrite(Trigger_AusgangsPin_2, HIGH);
  delayMicroseconds(10);
  digitalWrite(Trigger_AusgangsPin_2, LOW);
  Dauer = pulseIn(Echo_EingangsPin_2, HIGH);
  Abstand = Dauer / 58.2;
  if (Abstand >= maximumRange || Abstand <= minimumRange) {
    wasTrueS2[0] = false;
    wasTrueS2[1] = false;
    wasTrueS2[2] = false;
    // Pause zwischen den einzelnen Messungen
    delay(Ultraschall_Delay);
  }
  else {
    Serial.println("---MessbereichS2---");
    if (wasTrueS2[0] && wasTrueS2[1]) {
      wasTrueS2[2] = true;
    } else if (wasTrueS2[0]) {
      wasTrueS2[1] = true;
    } else {
      wasTrueS2[0] = true;
    }
    // Pause zwischen den einzelnen Messungen
    delay(Ultraschall_Delay);
  }
}

boolean ultraschall() {
  // immer beide Sensoren prüfen um wasTrue aktuell zu halten
  ultraschall_1();
  ultraschall_2();

  boolean s1 = wasTrueS1[0] && wasTrueS1[1] && wasTrueS1[2];
  boolean s2 = wasTrueS2[0] && wasTrueS2[1] && wasTrueS2[2];
  return s1 || s2;
}
boolean testIfDark() {
  //Helligkeitssensor
  sensorValue = analogRead(sensorPin);
  Serial.print("Die Helligkeit beträgt: ");
  Serial.println(sensorValue, DEC);

  if (sensorValue >= sensorDark) {
    return true;
  } else {
    return false;
  }
}

void setLed(int power) {
  if (power > currPower) {
    for (int i = currPower; i <= power; i++) {
      analogWrite(PvmPin, i);
      delay(Led_Dimming);
    }
    currPower = power;
  } else if (power < currPower) {
    for (int i = currPower; i >= power; i--) {
      analogWrite(PvmPin, i);
      delay(Led_Dimming);
    }
    currPower = power;
  }
}

void loop() {
  if (testIfDark()) {
    if (ultraschall()) {
      setLed(hell);
      Serial.println("---Person erkannt---");
      delay(Leucht_Dauer);
    } else {
      setLed(gedimmt);
    }
  } else {
    setLed(aus);
  }
}
