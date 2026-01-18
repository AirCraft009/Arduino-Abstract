## 18.6 Data Transmission – The USART Transmitter
The USART Transmitter is enabled by setting the Transmit Enable (TXEN) bit in the UCSRnB Register.\
When the Transmitter is enabled, the normal port operation of the TxDn pin is overrid- den by the USART\
and given the function as the Transmitter’s serial output. The baud rate,\
mode of operation and frame format must be set up once before doing any transmissions.\
If syn-chronous operation is used, the clock on the XCKn pin will be overridden and used as transmission clock.

## 18.6.1 Sending Frames with 5 to 8 Data Bit
A data transmission is initiated by loading the transmit buffer with the data to be transmitted.\
The CPU can load the transmit buffer by writing to the UDRn I/O location.\
The buffered data in the transmit buffer will be moved to the Shift Register\
when the Shift Register is ready to send a new frame.\
The Shift Register is loaded with new data if it is in idle state (no ongoing transmission)\
or immediately after the last stop bit of the previous frame is transmitted.\
When the Shift Register is loaded with new data, it will transfer one complete frame at the rate given by the Baud Register,\
U2Xn bit or by XCKn depending on mode of operation.
The following code examples show a simple USART transmit function based on polling of the Data Register Empty (UDREn) Flag.\
When using frames with less than eight bits, the most significant bits written to the UDRn are ignored.\
The USART has to be initialized before the function can be used.\
For the assembly code, the data to be sent is assumed to be stored in Register R16\

## 18.11 Register Description


## Official Atmega328p datasheet

19.10 Register Description
19.5 USART Initialization