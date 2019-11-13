#ifndef RGBA_H_
#define RGBA_H_

#include <cstdint>

struct RGBA
{
    RGBA()=default;
    RGBA(const RGBA &)=default;
    RGBA(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a=0) noexcept;
    void set(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a) noexcept;
    unsigned char red() const noexcept;
    unsigned char green()const noexcept;
    unsigned char blue() const noexcept;
    unsigned char alpha()const noexcept;
    uint32_t pixel=0;

};

RGBA::RGBA(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a) noexcept
{
  set(_r,_g,_b,_a);
}

void RGBA::set(unsigned char _r, unsigned char _g, unsigned char _b, unsigned char _a) noexcept
{
  pixel=uint32_t(_a | (_b << 8) | (_g << 16) | (_r << 24));
}

unsigned char RGBA::red() const noexcept
{
  return (pixel & 0xFF000000) >> 24;
}

unsigned char RGBA::green()const noexcept
{
  return (pixel & 0x00FF0000) >> 16;
}

unsigned char RGBA::blue() const noexcept
{
  return (pixel & 0x0000FF00) >> 8 ;
}

unsigned char RGBA::alpha()const noexcept
{
  return (pixel & 0x000000FF);
}



#endif
