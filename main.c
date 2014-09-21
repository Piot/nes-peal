#include <nes.h>

typedef unsigned char u8;

#define ppu_background_scrolling_offset(a, b) *((u8*) 0x2005) = a; *((u8*) 0x2005) = b;

#define vram_address_register(a, b) *((u8*) 0x2006) = a; *((u8*) 0x2006) = b;

#define vram_write(a) *((u8*) 0x2007) = a;

#define ppu_control_register_2(a) *((u8*) 0x2001) = a;

void write_string(const char* s)
{
	vram_address_register(0x20, 0x41);
	while (*s)
	{
		vram_write(*s);
		s++;
	}
}

void enable_screen()
{
	ppu_control_register_2(0x08);
}

void wait_forever()
{
	while (1)
		;
}

void setup_colors()
{
	vram_address_register(0x3f, 0x00);
	vram_write(0x01);

	vram_address_register(0x3f, 0x03);
	vram_write(0x30);
}

void zero_screen_position()
{
	ppu_background_scrolling_offset(0x00, 0x00);
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
