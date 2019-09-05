/**
 * @file WaterLevelWatcher.h
 * @brief 수위 체크
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */

#ifndef WATER_LEVEL_WATCHER_H
#define WATER_LEVEL_WATCHER_H

#include <EEPROM.h>
#include <ArduinoJson.h>
#ifdef ESP8266
#include <ESP8266HTTPClient.h>
#elif defined(ESP32)
#include <HTTPClient.h>
#endif

#ifndef EEPROM_SIZE
#define EEPROM_SIZE 4096
#endif

#define JSON_LEN 256

//DEBUG
#ifndef WATER_LEVEL_WATCHER_DEBUG_PORT
#define WATER_LEVEL_WATCHER_DEBUG_PORT Serial
#endif

#ifdef WATER_LEVEL_WATCHER_DEBUG
#define print(...) WATER_LEVEL_WATCHER_DEBUG_PORT.print(__VA_ARGS__)
#define printf(...) WATER_LEVEL_WATCHER_DEBUG_PORT.printf(__VA_ARGS__)
#define println(...) WATER_LEVEL_WATCHER_DEBUG_PORT.println(__VA_ARGS__)

#else
#define print(...)
#define printf(...)
#define println(...)
#endif
/**
 * @brief 수위 상태
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
enum WaterLevelState
{
  /** UltrasoundState의 NONE과 같음 */
  NONE_WATER_LEVEL,
  /** UltrasoundState의 GOOD_DISTANCE와 같음 */
  GOOD_WATER_LEVEL,
  /** UltrasoundState의 CLOSE_DISTANCE와 같음 */
  FLOOD_WATER_LEVEL,
  /** UltrasoundState의 FAR_DISTANCE와 같음 */
  LACK_WATER_LEVEL,
  /** 값을 가져오지 못한 경우 */
  ERROR_WATER_LEVEL
};

/**
 * @struct WATER_LEVEL_WATCHER_DATA
 * @brief 수위체크 EEPROM 데이터
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
struct WATER_LEVEL_WATCHER_DATA
{
  /** 데이터 저장유뮤 */
  bool is_saved;
  /** 수위 체크 사용 유무 */
  bool power;
  /** 초음파 센서 데이터 URL */
  char url[128];
  /** 제어할 콘센트 번호 */
  int outlet;
};

/**
 * @brief 수위 체크
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
class WaterLevelWatcher
{
private:
  WATER_LEVEL_WATCHER_DATA _water_level_watcher_data;
  WaterLevelState _state;
  bool _water_level_change_flag = false;
  int _eeprom_address = 0;

public:
  WaterLevelWatcher(void);
  void watch(void);
  bool isWaterLevelChanged(void);
  void setEEPORMAddress(int eeprom_address);
  void loadData(void);
  void saveData(void);

  bool getPower(void);
  void setPower(bool power);
  char *getUrl(void);
  void setUrl(char *url);

  int getOutlet(void);
  void setOutlet(int outlet);

  WaterLevelState getState(void);
};

#endif /** WATER_LEVEL_WATCHER_H **/