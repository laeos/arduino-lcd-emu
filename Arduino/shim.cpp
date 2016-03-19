#include <unistd.h>
#include <stdio.h>
#include <thread>
#include <sys/time.h>

#include "Arduino.h"
#include "SFML_TFT.h"

class Sketch {
    private:
	sf::Thread thread;

    public:
	Sketch() : thread(&Sketch::work, this)
	{
	}

	void launch(void)
	{
	    thread.launch();
	}

	void terminate(void)
	{
	    thread.terminate();
	}

	void work(void)
	{
	    printf("starting sketch\n");
	    setup();
	    while (1) {
		loop();
	    }
	}
};

Sketch sketch;
SerialPort Serial;

sf::RenderWindow *SFML_BASE::rw;
SFML_BASE *SFML_BASE::tft_object;

void delay(unsigned long ms)
{
    sf::sleep(sf::milliseconds(ms));
}

void yield(void)
{
}

unsigned long micros(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_usec + (tv.tv_sec * 1000 * 1000);
}

int main(int argc, char *argv[])
{
    printf("starting simulator\n");
    sketch.launch();

    while (SFML_BASE::rw->isOpen()) {
	if (SFML_BASE::tft_object) {
	    SFML_BASE::tft_object->poll();
	    SFML_BASE::tft_object->maybe_display();
	}
	usleep(33 * 1000);
    }
    sketch.terminate();
    return 0;
}
