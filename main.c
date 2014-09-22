#include <nes.h>

typedef unsigned char u8;

u8 player1_x = 2;
u8 player1_y = 2;

u8 joypad1_data = 0;
u8 joypad2_data = 0;

u8 temp;
u8 frame_counter = 0;

#define LEVEL_OCTET_SIZE (32*30)
u8 level[LEVEL_OCTET_SIZE];

int i;
int j;
int points = 0;

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
	u8 next_x = player1_x;
	u8 next_y = player1_y;

	read_joypads();
	frame_counter++;
	if (frame_counter % 4 != 0) {
		return;
	}
	if (joypad1_data & 0x01) {
		if (player1_x < 30) {
			next_x = player1_x + 1;
		} else {
			return;
		}
	} else if (joypad1_data & 0x02) {
		if (player1_x > 1) {
			next_x = player1_x - 1;
		} else {
			return;
		}
	} else if (joypad1_data & 0x04) {
		if (player1_y < 28) {
			next_y = player1_y + 1;
		} else {
			return;
		}
	} else if (joypad1_data & 0x08) {
		if (player1_y > 1) {
			next_y = player1_y - 1;
		} else {
			return;
		}
	} else {
		return;
	}

	i = ((int)next_y << 5) + next_x;
	temp = level[i];
	if (temp != '.' && temp != ' ') {
		return;
	}
	if (temp == '.') {
		level[i] = ' ';
		++points;
	}
	i = ((int)player1_y << 5) + player1_x;
	set_char(player1_x, player1_y, level[i]);
	player1_x = next_x;
	player1_y = next_y;
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

const char* level1 = "////////////////////////////////"
	"/ #############################/"
	"/............................../"
	"/.#.####.#............#.####.#./"
	"/.#.####.#.####..####.#.####.#./"
	"/..........####..####........../"
	"/.#.####.#.####..####.#.####.#./"
	"/.#.####.#.####..####.#.####.#./"
	"/..........####..####........../"
	"/#.#####.######..######.#####.#/"
	"/#.#####.######..######.#####.#/"
	"/#.......######..######.......#/"
	"/#.......##..........##.......#/"
	"/#.#####.##..........##.#####.#/"
	"/#.#####................#####.#/"
	"/#............................#/"
	"/#.#####.##..........##.#####.#/"
	"/#.#####.##..........##.#####.#/"
	"/#.......######..######.......#/"
	"/#.#####.######..######.#####.#/"
	"/#.#####.######..######.#####.#/"
	"/..........####..####........../"
	"/.#.####.#.####..####.#.####.#./"
	"/.#.####.#.####..####.#.####.#./"
	"/..........####..####........../"
	"/.#.####.#.####..####.#.####.#./"
	"/.#.####.#.####..####.#.####.#./"
	"/............................../"
	"/##############################/"
	"////////////////////////////////";
	
	
int count_underscores(char* level1) {
  int count = 0;

  for (int i = 0; i < strlen(level1); i++)
    if (level1[i] == '.') count++;
}

void draw_level()
{
	set_name_position_to_vram(0, 0);
	for (i=0; i < LEVEL_OCTET_SIZE; ++i) {
		vram_write(level[i]);
	}
}

void copy_to_active_level()
{
	for (i=0; i<LEVEL_OCTET_SIZE; ++i) {
		level[i] = level1[i];
	}
}


int main()
{
	waitvblank();
	setup_colors();
	copy_to_active_level();
	draw_level();
	set_char(player1_x, player1_y, 'x');

	while (1) {
		waitvblank();
		disable_screen();
		game_loop();
		zero_screen_position();
		enable_screen();
	}

	return 0;
}
