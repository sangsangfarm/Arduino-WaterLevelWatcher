# Arduino-Water-Level-Watcher

Water level watcher in Arduino.

## Support platform

- esp8226
- esp32

## Usage

```cpp
WaterLevelWatcher watcher = WaterLevelWatcher();
void setup() {
  char *watching_url = "http://watching.url";
  int oultet_pin = 10;
  watcher.setPower(true);
  watcher.setOutlet(oultet_pin);
  watcher.setUrl(watching_url);

  //If you load or save water level watcher info, use this.
  int eeprom_address = 1000;
  watcher.setEEPORMAddress(eeprom_address);
  watcher.loadData();
  watcher.saveData();
}

void loop() {
  watcher.watch();
}
```
