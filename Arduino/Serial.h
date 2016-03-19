#ifndef SERIAL_H
#define SERIAL_H

#include <unistd.h>
#include <stdlib.h>

class SerialPort : public Print {

    public:
    void begin(int speed)
    {
    }

    virtual size_t write(uint8_t ch)
    {
	::write(STDOUT_FILENO, &ch, 1);
	return 1;
    }

};

extern SerialPort Serial;

#endif /* SERIAL_H */
