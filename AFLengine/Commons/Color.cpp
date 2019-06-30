#include <Color.h>

#define COLOR_GENERATOR_SEED 0x68A251

Color Color::RED (255, 0, 0);
Color Color::GREEN (0, 255, 0);
Color Color::BLUE (0, 0, 255);
Color Color::WHITE (255, 255, 255);
Color Color::GRAY (192, 192, 192);
Color Color::BLACK (0, 0, 0);

Color::Color () {
    this->r = 0;
    this->g = 0;
    this->b = 0;
    this->a = 255;
}

Color::Color (unsigned char r, unsigned char g, unsigned char b) {
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = 255;
}

Color::Color (unsigned char r, unsigned char g, unsigned char b, unsigned char a) {
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

Color::Clamped::Clamped (float r, float g, float b, float a) {
    this->r = r;
    this->g = g;
    this->b = b;
    this->a = a;
}

Color Color::create (unsigned int seed) {
    unsigned int generatedValue = COLOR_GENERATOR_SEED * seed;
    unsigned char r = generatedValue % 0x100;
    unsigned char g = (generatedValue >> 8) % 0x100;
    unsigned char b = (generatedValue >> 16) % 0x100;
    return Color (r, g, b);
}

Color::Clamped Color::clamp () {
    return Clamped (r / 255.0f, g / 255.0f, b / 255.0f, a / 255.0f);
}

Color Color::operator* (double value) {
    return Color (unsigned char (r * value), unsigned char (g * value), unsigned char (b * value), a);
}

Color& Color::operator*= (double value) {
    r = unsigned char (r * value);
    g = unsigned char (g * value);
    b = unsigned char (b * value);
    return *this;
}
