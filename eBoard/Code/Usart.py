import serial

def SerConfig():
	ser = serial.Serial( 
  		port='COM1',
  		baudrate=115200,
  		parity=serial.PARITY_NONE,
  		stopbits=serial.STOPBITS_ONE,
  		bytesize=serial.EIGHTBITS
	)
	return ser

port=SerConfig()
port.write('\xF0')
port.write('\x30')
port.write('\xF4')
port.write('\x34')
port.write('\xF4')
port.write('\xF1')

