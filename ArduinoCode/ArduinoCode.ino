#define GET_VAR_NAME(var) (#var)
#define SERIAL_TX_BUFFER_SIZE 64
#define MOD_Check 2 << 32

long time = 0;

struct DataMessage {
  int temp, light, loudness, wind, wDirection, uv, o2, humidity, pressure, co2, co, no2, smog, ph1, ph2, ph3;
  //temp, light, loudness, wind, wDirection, uv, o2, humidity, pressure, co2, co, no2, smog
};
DataMessage dm;

class Postman{
  public:
    virtual void send (DataMessage d){
      long checksum = 0;
      int *values[] = { &d.temp, &d.light, &d.loudness, &d.wind, &d.wDirection, &d.uv, &d.o2, &d.humidity, &d.pressure, &d.co2, &d.co, &d.no2, &d.smog, &d.ph1, &d.ph2, &d.ph3};
      String names[] = {GET_VAR_NAME(d.temp), GET_VAR_NAME(d.light), GET_VAR_NAME(d.loudness),GET_VAR_NAME(d.wind), GET_VAR_NAME(d.wDirection), GET_VAR_NAME(d.uv), GET_VAR_NAME(d.o2), GET_VAR_NAME(d.humidity), GET_VAR_NAME(d.pressure), GET_VAR_NAME(d.co2), GET_VAR_NAME(d.co), GET_VAR_NAME(d.no2), GET_VAR_NAME(d.smog), GET_VAR_NAME(d.ph1),GET_VAR_NAME(d.ph2),GET_VAR_NAME(d.ph3)};
      Serial.print("{");
      for(int i = 0; i < sizeof(values) / sizeof(values[0]); i++){
        Serial.print(intToJson(names[i], *values[i]) + ", ");
        checksum = (checksum + *values[i]); //% MOD_Check;
      }
      Serial.print(longToJson("time", time));
      Serial.print(",");
      Serial.print(longToJson("checksum", checksum));
      Serial.println("}");
      getReturn(d);
    }
  private:
    void getReturn(DataMessage d){
        String incomingString = Serial.readStringUntil(0x0A);
        int msg = incomingString.substring(0, 2).toInt();
        switch(msg){
          case 0x06:
            //All good
            digitalWrite(53, HIGH);
            break;
          case 0x07:
            incomingString = Serial.readStringUntil(0x0A);
            time = atol(incomingString.c_str());
             digitalWrite(53, LOW);
            break;
          case 0x15:
            send(d);
            break;
          default:
            break;
        }
        
    }
  String intToJson(String name, int i){
    int index = name.lastIndexOf('.') + 1;
    int length = name.length();
    name = name.substring(index, length);
    return "\"" + name + "\":" + "\"" + i + "\"";
  }
  String longToJson(String name, long i){ 
    int index = name.lastIndexOf('.') + 1;
    int length = name.length();
    name = name.substring(index, length);
    return "\"" + name + "\":" + "\"" + i + "\"";
  }
};

Postman *p;


void setup(){
  Serial.begin(57600);
  p = new Postman();
  dm.temp = 5000;
  dm.light = random(500);
  dm.loudness = random(500);
  dm.wind = random(500);
  dm.wDirection = random(500);
  dm.uv = random(500);
  dm.o2 = random(500);
  dm.humidity = random(500);
  dm.pressure = random(500);
  dm.co2 = random(500);
  dm.co = random(500);
  dm.no2 = random(500);
  dm.smog = random(500);
  dm.ph1 = 0;
  dm.ph2 = 0;
  dm.ph3 = 0;
  pinMode(53, OUTPUT);
  digitalWrite(53, LOW);
}
void loop(){
  p->send(dm);
  delay(1000);
}