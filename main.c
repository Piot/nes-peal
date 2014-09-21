#include <nes.h>

typedef unsigned char u8;

#define ppu_register(a, b) *((u8*)0x2005) = a; *((u8*)0x2005) = b;

#define ppu_select(a, b) *((u8*)0x2006) = a; *((u8*)0x2006) = b;

#define ppu_write(a) *((u8*)0x2007) = a;

void write_string(const char* s)
{
	ppu_select(0x20, 0x41);
	while (*s)
	{
		ppu_write(*s);
		s++;
	}
}

void enable_screen()
{
	*((u8*) 0x2001) = 0x08;
}

void wait_forever()
{
	while (1)
		;
}

void setup_colors()
{
	ppu_select(0x3f, 0x00);
	ppu_write(0x01);

	ppu_select(0x3f, 0x03);
	ppu_write(0x30);
}

void zero_screen_position()
{
	ppu_register(0x00, 0x00);
}

int main()
{
	waitvblank();

	setup_colors();

	write_string("André is cool!");

	zero_screen_position();	
	
	enable_screen();

	wait_forever();

	return 0;
}
