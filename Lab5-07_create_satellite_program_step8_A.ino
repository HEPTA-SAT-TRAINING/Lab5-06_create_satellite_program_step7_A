#include "src/HeptaSat.h"

HeptaCdh    cdh;
HeptaCom    com;
HeptaEps    eps;
HeptaSensor sensor;

// Two separate thresholds prevent chattering when voltage hovers near the boundary
const float VOLTAGE_TURN_OFF = 3.7;
const float VOLTAGE_TURN_ON  = 3.9;

bool sw3V3_is_on = true;

void downlink_hk_data(void) {
  float temp = sensor.get_temperature();
  float vbat = eps.get_battery_voltage();
  float v5 = eps.get_5v_voltage();
  float v3v3 = eps.get_3v3_voltage();
  float sap = eps.get_sap_voltage();
  float idis = eps.get_current_discharge();
  float ichg = eps.get_current_charge();

  com.printf(
    "TEMP=%.2f,VBAT=%.3f,V5=%.3f,V3V3=%.3f,SAP=%.3f,IDIS=%.3f,ICHG=%.3f\r\n",
    temp, vbat, v5, v3v3, sap, idis, ichg);

  cdh.printf(
    "HK: TEMP=%.2f C, VBAT=%.3f V, V5=%.3f V, V3V3=%.3f V, SAP=%.3f V, IDIS=%.3f A, ICHG=%.3f A\r\n",
    temp, vbat, v5, v3v3, sap, idis, ichg);
}

void setup() {
  cdh.begin();
  eps.init();
  sensor.begin();
  com.begin();
  cdh.wait_for_sd();
}

void loop() {
  downlink_hk_data();

  float battery_voltage = eps.get_battery_voltage();
  if (sw3V3_is_on && battery_voltage < VOLTAGE_TURN_OFF) {
    com.println("Battery voltage is low! Switching off 3.3V SW to save power.");
    eps.switch_3V3_off();
    sw3V3_is_on = false;
  } else if (!sw3V3_is_on && battery_voltage > VOLTAGE_TURN_ON) {
    com.println("Battery voltage recovered. Switching on 3.3V SW.");
    eps.switch_3V3_on();
    sw3V3_is_on = true;
  } else {
    com.printf("3.3V SW is %s.\n", sw3V3_is_on ? "on" : "off");
  }

  if (com.is_cmd_received()) {
    char cmd = com.get_command();

    if (cmd != '\0') {
      com.printf("Received command: %c\n", cmd);

      // Process the command and respond accordingly
      switch (cmd) {
        case 'a': {
          for(uint8_t i = 0; i < 10; i++) {
            com.println("Hello HEPTA-SAT");
            delay(1000);
          }
          break;
        }

        case 'b': {
          com.println("Saving voltages to the SD card...");
          File file = cdh.create_file("test.txt");
          if (file) {
            for(uint8_t i = 0; i < 10; i++) {
              float battery_voltage = eps.get_battery_voltage();
              cdh.printf_file(file, "Battery Voltage: %f V\r\n", battery_voltage);
              delay(1000);
            }
            file.close();
          } else {
            com.println("Failed to create file on SD card.");
            break;
          }

          com.println("Done saving voltages to the SD card.");
          com.println("Reading voltages from the SD card...");

          file = cdh.open_file("test.txt");
          while (file && file.available()) {
            com.write(cdh.read_file(file));
          }
          file.close();

          com.println("Done reading voltages from the SD card.");
          break;
        }

        case 'c': {
          com.println("Reading accelerometer data...");
          float ax, ay, az;
          for (uint8_t i = 0; i < 10; i++) {
            sensor.get_acceleration(&ax, &ay, &az);
            com.printf("Acceleration: ax=%.2f, ay=%.2f, az=%.2f m/s^2\n", ax, ay, az);
            delay(1000);
          }
          com.println("Done reading accelerometer data.");
          break;
        }

        case 'd': {
          com.println("Reading gyroscope data...");
          float gx, gy, gz;
          for (uint8_t i = 0; i < 10; i++) {
            sensor.get_gyro(&gx, &gy, &gz);
            com.printf("Gyroscope: gx=%.2f, gy=%.2f, gz=%.2f °/s\n", gx, gy, gz);
            delay(1000);
          }
          com.println("Done reading gyroscope data.");
          break;
        }

        case 'e': {
          // Please write your original code here

          
        }

        default:
          com.println("Unknown command");
          break;
      }
    }
  }
  delay(1000);
}
