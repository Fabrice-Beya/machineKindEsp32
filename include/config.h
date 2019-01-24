#ifndef CONFIG_H_INCLUDED
#define CONFIG_H_INCLUDED

/**************************************
* OBD-II Adapter options
**************************************/
#define OBD_ADAPTER_UART
#define OBD_PROTOCOL PROTO_AUTO

/**************************************
* Data logging options
**************************************/
// enable(1)/disable(0) data logging (if SD card is present)
#define ENABLE_DATA_LOG 1
#define SD_CS_PIN 5

/**************************************
* Data streaming options
**************************************/
// enable(1)/disable(0) data streaming
#define ENABLE_DATA_OUT 0

// uses software(1)/hardware(0) serial for data streaming
#define RF_SERIAL Serial
#define STREAM_BAUDRATE 115200 /* bps */

// this defines the format of data streaming
// FORMAT_BIN is required by Freematics OBD iOS App
// FORMAT_TEXT for text-based, text names for PID
#define STREAM_FORMAT FORMAT_TEXT

/**************************************
* Accelerometer & Gyro
**************************************/
#define ACC_DATA_RATIO 10
#define GYRO_DATA_RATIO 100

//GPRS
//SIM800 TX is connected to Arduino D8
#define SIM800_TX_PIN 8
//SIM800 RX is connected to Arduino D7
#define SIM800_RX_PIN 7

#endif
