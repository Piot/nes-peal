#include <nes.h>

typedef unsigned char u8;

u8 player1_x = 0;
u8 player1_y = 2;

u8 temp;
u8 frame_counter = 0;

#define ppu_background_scrolling_offset(a, b) *((u8*) 0x2005) = a; *((u8*) 0x2005) = b;

#define vram_address_register(a, b) *((u8*) 0x2006) = a; *((u8*) 0x2006) = b;

#define vram_write(a) *((u8*) 0x2007) = a;

#define ppu_control_register_2(a) *((u8*) 0x2001) = a;

#define set_name_position_to_vram_with_offset(o, x, y) vram_address_register(o + (y >> 3), ((y << 5) + x) & 0xff); 

#define set_name_position_to_vram(x, y)  set_name_position_to_vram_with_offset(0x20, x, y);

void write_string(const char* s)
{
	while (*s)
	{
		vram_write(*s);
		s++;
	}
}


#define set_char(x, y, c) set_name_position_to_vram(x, y); vram_write(c); 

void enable_screen()
{
	ppu_control_register_2(0x08);
}

void disable_screen()
{
	ppu_control_register_2(0x00);
}


void game_loop()
{
	frame_counter++;
	if (frame_counter % 8 != 0) {
		return;
	}
	if (player1_x == 30)
	{
		return;
	}
	set_char(player1_x, player1_y, ' ');
	player1_x++;
	set_char(player1_x, player1_y, 'x');
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
	disable_screen();
	setup_colors();

	set_name_position_to_vram(2, 3);
	write_string("Andre is cool!");

	while (1) {
		waitvblank();
		disable_screen();
		game_loop();
		zero_screen_position();
		enable_screen();
	}

	return 0;
}
