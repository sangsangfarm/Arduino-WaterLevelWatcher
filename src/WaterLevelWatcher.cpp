/**
 * @file WaterLevelWatcher.cpp
 * @brief 수위 체크
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */

#include <WaterLevelWatcher.h>

/**
 * @fn WaterLevelWatcher::WaterLevelWatcher(void)
 * @brief 수위 체크 생성자
 * @return 수위 체크 클래스
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
WaterLevelWatcher::WaterLevelWatcher(void)
{
}

/**
 * @fn void WaterLevelWatcher::watch(void)
 * @brief 수위 상태 값 가져오기
 * @return void
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
void WaterLevelWatcher::watch(void)
{
  printf("[WaterLevelWatcher] 전원 상태 : %s\n", _water_level_watcher_data.power ? "켜짐" : "꺼짐");
  if (!_water_level_watcher_data.power || strncmp(_water_level_watcher_data.url, "", sizeof("")) == 0 || _water_level_watcher_data.outlet == -1)
  {
    _water_level_change_flag = false;
    return;
  }
  WaterLevelState state = _state;
  HTTPClient http;
  String url = String(_water_level_watcher_data.url);
  http.begin(url);
  http.setConnectTimeout(1000);
  println(url);
  http.addHeader("Content-Type", "application/json");
  int http_code = http.GET();
  String result = http.getString();
  println(result);
  printf("[WaterLevelWatcher] 수위 체크 접속 Http Code : %d\n", http_code);
  http.end();
  DynamicJsonDocument root(JSON_LEN);
  DeserializationError error = deserializeJson(root, result);
  if (200 <= http_code && http_code < 300)
  {
    if (error)
    {
      printf("[WaterLevelWatcher] Parse %s failed.\r\n", result.c_str());
      println(error.c_str());
      state = ERROR_WATER_LEVEL;
    }
    if (!root["SensorState"].isNull())
    {
      state = (WaterLevelState)(int)root["SensorState"];
    }
  }
  else
  {
    state = ERROR_WATER_LEVEL;
  }

  root.clear();
  if (_state != state)
  {
    _water_level_change_flag = true;
    _state = state;
  }
  else
  {
    _water_level_change_flag = false;
  }
}

/**
 * @fn bool WaterLevelWatcher::getPower(void)
 * @brief 수위 체크 전원 상태 가져오기
 * @return 수위 체크 전원 상태 
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
bool WaterLevelWatcher::getPower(void)
{
  return _water_level_watcher_data.power;
}

/**
 * @fn void WaterLevelWatcher::setPower(bool power)
 * @brief 수위 체크 전원 상태 설정
 * @param power 설정할 전원 상태
 * @return void
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
void WaterLevelWatcher::setPower(bool power)
{
  _water_level_watcher_data.power = power;
}
/**
 * @fn bool WaterLevelWatcher::isWaterLevelChanged(void)
 * @brief 수위 변화 감지
 * @return 수위 변화 감지 여부
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
bool WaterLevelWatcher::isWaterLevelChanged(void)
{
  return _water_level_change_flag;
}

/**
 * @fn void WaterLevelWatcher::setEEPORMAddress(int eeprom_address)
 * @brief EEPROM 주소 설정
 * @param eeprom_address 설정할 EEPROM 주소
 * @return void
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
void WaterLevelWatcher::setEEPORMAddress(int eeprom_address)
{
  _eeprom_address = eeprom_address;
}

/**
 * @fn void WaterLevelWatcher::loadData(void)
 * @brief EEPROM 데이터 불러오기
 * @return void
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
void WaterLevelWatcher::loadData(void)
{
  EEPROM.begin(EEPROM_SIZE);
  EEPROM.get(_eeprom_address, _water_level_watcher_data);
  if (!_water_level_watcher_data.is_saved)
  {
    memset(_water_level_watcher_data.url, 0, sizeof(_water_level_watcher_data.url));
    _water_level_watcher_data.outlet = -1;
  }
  EEPROM.end();
  printf("[WaterLevelWatcher] url : %s outlet: %d\n", _water_level_watcher_data.url, _water_level_watcher_data.outlet);
}

/**
 * @fn void WaterLevelWatcher::saveData(void)
 * @brief EEPROM 데이터 저장
 * @return void
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
void WaterLevelWatcher::saveData(void)
{
  EEPROM.begin(EEPROM_SIZE);
  printf("[WaterLevelWatcher] EEPROM 주소: %d\n", _eeprom_address);
  _water_level_watcher_data.is_saved = true;
  EEPROM.put(_eeprom_address, _water_level_watcher_data);
  EEPROM.commit();
  EEPROM.end();
}

/**
 * @fn char *WaterLevelWatcher::getUrl(void)
 * @brief 수위 체크 데이터 url 가져오기
 * @return 수위 체크 데이터 url
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
char *WaterLevelWatcher::getUrl(void)
{
  return _water_level_watcher_data.url;
}

/**
 * @fn void WaterLevelWatcher::setUrl(char *url)
 * @brief 수위 체크 데이터 url 설정
 * @param url 수위 체크 데이터 url 
 * @return void
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
void WaterLevelWatcher::setUrl(char *url)
{
  memset(_water_level_watcher_data.url, 0, sizeof(_water_level_watcher_data.url));
  memcpy(_water_level_watcher_data.url, url, strlen(url) + 1);
}

/**
 * @fn char *WaterLevelWatcher::getDeviceName(void)
 * @brief 수위 체크가 제어하는 디바이스 이름 가져오기
 * @return 수위 체크가 제어하는 디바이스 이름 
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
char *WaterLevelWatcher::getDeviceName(void)
{
  return _water_level_watcher_data.device_name;
}

/**
 * @fn void WaterLevelWatcher::setOutlet(int outlet)
 * @brief 수위 체크가 제어하는 디바이스 이름 설정
 * @param outlet 수위 체크가 제어하는 디바이스 이름
 * @return void
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
void WaterLevelWatcher::setDeviceName(char *device_name)
{
  memset(_water_level_watcher_data.device_name, 0, sizeof(_water_level_watcher_data.device_name));
  memcpy(_water_level_watcher_data.device_name, device_name, strlen(device_name) + 1);
}

/**
 * @fn int WaterLevelWatcher::getOutlet(void)
 * @brief 수위 체크가 제어하는 콘센트 가져오기
 * @return 수위 체크가 제어하는 콘센트 번호
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
int WaterLevelWatcher::getOutlet(void)
{
  return _water_level_watcher_data.outlet;
}

/**
 * @fn void WaterLevelWatcher::setOutlet(int outlet)
 * @brief 수위 체크가 제어하는 콘센트 설정
 * @param outlet 수위 체크가 제어하는 콘센트 번호
 * @return void
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
void WaterLevelWatcher::setOutlet(int outlet)
{
  _water_level_watcher_data.outlet = outlet;
}

/**
 * @fn WaterLevelState WaterLevelWatcher::getState(void)
 * @brief 수위 체크 상태 가져오기
 * @return 수위 체크 상태
 * @date 2019-08-27
 * @author Janghun Lee (jhlee@sangsang.farm)
 */
WaterLevelState WaterLevelWatcher::getState(void)
{
  printf("[WaterLevelWatcher] state: %d\n", _state);
  return _state;
}