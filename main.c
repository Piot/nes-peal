#include <nes.h>

typedef unsigned char u8;

u8 player1_x = 0;
u8 player1_y = 0;

u8 joypad1_data = 0;
u8 joypad2_data = 0;

u8 temp;
u8 frame_counter = 0;

#define ppu_background_scrolling_offset(a, b) *((u8*) 0x2005) = a; *((u8*) 0x2005) = b;

#define vram_address_register(a, b) *((u8*) 0x2006) = a; *((u8*) 0x2006) = b;

#define vram_write(a) *((u8*) 0x2007) = a;

#define ppu_control_register_2(a) *((u8*) 0x2001) = a;

#define set_name_position_to_vram_with_offset(o, x, y) vram_address_register(o + (y >> 3), (((y << 5) & 0xff)+ x) & 0xff); 

#define set_name_position_to_vram(x, y)  set_name_position_to_vram_with_offset(0x20, x, y);

#define joypad_strobe *((u8 *) 0x4016) = 0x01; *((u8 *) 0x4016) = 0x00;

#define joypad1_read (*((u8*) 0x4016) & 0x01)
#define joypad2_read (*((u8*) 0x4017) & 0x01)

void write_string(const char* s)
{
	while (*s)
	{
		vram_write(*s);
		s++;
	}
}


#define set_char(x, y, c) set_name_position_to_vram(x, y); vram_write(c);

void read_joypads()
{
	u8 i;

	joypad_strobe;
	joypad1_data = 0;

	for (i=0; i<8; ++i) {
		joypad1_data <<= 1;
		joypad1_data |= joypad1_read;
	}
}

void enable_screen()
{
	ppu_control_register_2(0x1e);
}

void disable_screen()
{
	ppu_control_register_2(0x00);
}


void game_loop()
{
	read_joypads();
	frame_counter++;
	if (frame_counter % 4 != 0) {
		return;
	}
	set_char(player1_x, player1_y, ' ');
	if (joypad1_data & 0x01) {
		if (player1_x < 31) {
			player1_x++;
		}
	} else if (joypad1_data & 0x02) {
		if (player1_x > 0) {
			player1_x--;
		}
	} else if (joypad1_data & 0x04) {
		if (player1_y < 29) {
			player1_y++;
		}
	} else if (joypad1_data & 0x08) {
		if (player1_y > 0) {
			player1_y--;
		}
	}
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
