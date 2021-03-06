#ifndef HI_MOXA_H_
#define HI_MOXA_H_

#include "hi_moxa_combuf.h"

#include <stdint.h>
#include <termios.h>
#include <sstream>

#include <string>
#include <vector>

#include "serialcomm/serialcomm.hpp"
#include "nf/nfv2.h"

#define STATUS_DISP_T 100
#define BUFF_SIZE 256


#define RESET   "\033[0m"
#define BLACK   "\033[30m"      /* Black */
#define RED     "\033[31m"      /* Red */
#define GREEN   "\033[32m"      /* Green */
#define YELLOW  "\033[33m"      /* Yellow */
#define BLUE    "\033[34m"      /* Blue */
#define MAGENTA "\033[35m"      /* Magenta */
#define CYAN    "\033[36m"      /* Cyan */
#define WHITE   "\033[37m"      /* White */
#define BOLDBLACK   "\033[1m\033[30m"      /* Bold Black */
#define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
#define BOLDGREEN   "\033[1m\033[32m"      /* Bold Green */
#define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
#define BOLDBLUE    "\033[1m\033[34m"      /* Bold Blue */
#define BOLDMAGENTA "\033[1m\033[35m"      /* Bold Magenta */
#define BOLDCYAN    "\033[1m\033[36m"      /* Bold Cyan */
#define BOLDWHITE   "\033[1m\033[37m"      /* Bold White */


namespace hi_moxa {

const std::size_t MOXA_SERVOS_NR = NF_BUFSZ_NumberOfDrives;
const int MAX_PARAM_SET_ATTEMPTS = 3;
const int MAX_COMM_TIMEOUTS = 10;
const std::size_t FIRST_HARDWARE_READS_WITH_ZERO_INCREMENT = 4;

const int VOLTAGE = 48.0;

class HI_moxa {
 public:
  HI_moxa(unsigned int numberOfDrivers,
          std::vector<unsigned int> card_addresses,
          std::vector<double> max_increments, int tx_prefix_len);
  ~HI_moxa();

  // initialize
  void init(std::vector<std::string> ports);

  long int longest_delay_, longest_read_delay_;

  // do communication cycle
  uint64_t write_read_hardware(long int nsec, int timeouts_to_print);

  uint64_t read_hardware(int timeouts_to_print);
  uint64_t write_hardware(void);
  void set_hardware_panic(void);

  // set parameter
  void set_parameter(int drive_number, const int parameter, ...);

  // send parameter to motor driver
  int set_parameter_now(int drive_number, const int parameter, ...);

  void set_pwm(int drive_number, double set_value);
  void set_current(int drive_number, double set_value);

  void set_pwm_mode(int drive_number);
  void set_current_mode(int drive_number);

  int get_current(int drive_number);
  float get_voltage(int drive_number);
  double get_increment(int drive_number);
  long int get_position(int drive_number);

  void reset_counters(void);
  void start_synchro(int drive_number);
  void finish_synchro(int drive_number);
  void unsynchro(int drive_number);
  bool in_synchro_area(int drive_number);
  bool robot_synchronized();
  bool drive_synchronized(int drive_number);

  bool is_impulse_zero(int drive_number);
  void reset_position(int i);

  void clear_buffer(int drive_number);

  uint16_t convert_to_115(float input);
  int cycle_nr;

 protected:
  bool hardware_panic;

 private:
/// communication baud rate (bps)
#if defined(B921600)
  static const speed_t BAUD = B921600;
#else
  static const speed_t BAUD = 921600;
#endif
  const int howMuchItSucks;

  /// (number of drives)-1
  std::size_t last_drive_number;
  /// vector of serial port names
  std::vector<std::string> port_names;
  /// tab od drives addresses
  std::vector<unsigned int> drives_addresses;
  /// tab of max allowed motor position increments
  std::vector<double> ridiculous_increment;

  /// tab of comunication class instances
  SerialComm* SerialPort[MOXA_SERVOS_NR];

  /// Receive Fail Counter
  uint8_t receiveFailCnt[MOXA_SERVOS_NR];
  bool receiveFail[MOXA_SERVOS_NR];

  // uint8_t maxReceiveFailCnt;
  // #define MAX_RECEIVE_FAIL_CNT  5 // *2ms extra time for packet receive

  bool all_hardware_read;

  /// tab of data buffers
  struct servo_St servo_data[MOXA_SERVOS_NR];
  struct termios oldtio[MOXA_SERVOS_NR];

  /// periodic timer used for generating read_write_hardware time base
  // lib::periodic_timer ptimer;

  /// comunication buffer
  NF_STRUCT_ComBuf NFComBuf;
  uint8_t txBuf[BUFF_SIZE];
  uint8_t txCnt;

  struct {
    uint8_t rxBuf[BUFF_SIZE];
    uint8_t rxCnt;
  } drive_buff[MOXA_SERVOS_NR];

  uint8_t rxCommandArray[BUFF_SIZE];
  uint8_t rxCommandCnt;
};

}  // namespace hi_moxa

#endif  // HI_MOXA_H_
