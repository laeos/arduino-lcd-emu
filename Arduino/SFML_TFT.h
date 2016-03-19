#ifndef SFML_TFT_H
#define SFML_TFT_H

#include <iostream>
#include <memory>
#include <cmath>
#include <list>

#include <SFML/Graphics.hpp>

#include "Adafruit_GFX.h"

class SFML_BASE {
    public:
    static sf::RenderWindow *rw;
    static SFML_BASE *tft_object;

    virtual void display() = 0;
    virtual void maybe_display() = 0;

    static bool poll()
    {
	// Process events
	sf::Event event;
	if (rw->pollEvent(event)) {
	    switch (event.type) {
		case sf::Event::KeyPressed:
		    rw->close();
		    return false;

		case sf::Event::Closed:
		    rw->close();
		    return false;

		case sf::Event::MouseButtonPressed:
		    // this->x = unscale(event.mouseButton.x);
		    // this->y = unscale(event.mouseButton.y);
		    return true;

		default:
		    return false;
	    }
	}
	return false;
    }
};

class SFML_TFT : public Adafruit_GFX, public SFML_BASE {
private:

    sf::Mutex *mutex;
    sf::Clock clock;
    std::list<sf::RectangleShape *> list;
    std::vector<sf::RectangleShape *> cache;
    sf::Color clearColor;
    float scale_factor;
    int display_w, display_h;

    sf::RectangleShape *__get_circle_at(int x, int y)
    {
	int pos = (x * display_h) + y;

	if ((pos >= cache.size()) || (cache[pos] == nullptr)) {
	    cache[pos] = new sf::RectangleShape(sf::Vector2f(scale(1), scale(1)));
	    cache[pos]->setPosition(scale(x), scale(y));
	    list.push_back(cache[pos]);
	}
	return cache[pos];
    }

    /* must be called from main thread, with lock */
    void do_display()
    {
	rw->clear(clearColor);
	for (const sf::RectangleShape *x : list) {
	    rw->draw(*x);
	}
	rw->display();
	clock.restart();
    }

    int scale(int v)
    {
	return std::floor(v * scale_factor);
    }

    int unscale(int v)
    {
	return std::ceil((float)v / scale_factor);
    }

protected:

    void convertColor(uint16_t color, sf::Color& c)
    {
	c.r = ((color >> (5 + 6)) & 0x1f) << 3;
	c.g = ((color >> 5) & 0x3f) << 2;
	c.b = (color & 0x1f) << 3;
    }

public:

    SFML_TFT(int16_t w, int16_t h, float scale_factor = 1.0f) : Adafruit_GFX(w, h), mutex(new sf::Mutex())
    {
	this->scale_factor = scale_factor;
	rw = new sf::RenderWindow(sf::VideoMode(scale(w), scale(h)), "TFT", sf::Style::Titlebar | sf::Style::Close);
	display_w = w;
	display_h = h;
	cache.resize(w * h);
	tft_object = this;
    }

    virtual void drawPixel(int16_t x, int16_t y, uint16_t color)
    {
	sf::Lock lock(*mutex);
	sf::Color c;

	if (x < 0) x = 0;
	if (y < 0) y = 0;
	if (x >= display_w) x = display_w - 1;
	if (y >= display_h) y = display_h - 1;

	convertColor(color, c);
	sf::RectangleShape *circle = __get_circle_at(x, y);
	circle->setFillColor(c);
    }


    virtual void fillScreen(uint16_t color)
    {
	sf::Lock lock(*mutex);

	convertColor(color, clearColor);
	for (const sf::RectangleShape *x : list)
	    delete x;
	list.clear();
	cache.clear();
    }

    /* must be called from main thread */
    virtual void display()
    {
	sf::Lock lock(*mutex);
	do_display();
    }

    /* must be called from main thread */
    virtual void maybe_display()
    {
	sf::Lock lock(*mutex);
	if (clock.getElapsedTime().asMilliseconds() > 100)
	    do_display();
    }

    uint16_t color565(uint8_t r, uint8_t g, uint8_t b)
    {
	return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
    }
};
#endif 
