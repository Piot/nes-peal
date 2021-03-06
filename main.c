#include <nes.h>
#include "graphics.h"

typedef unsigned char u8;
typedef signed char s8;
typedef unsigned int u16;

u8 player1_x = 16;
u8 player1_y = 16;

u8 player1_old_x = 0;
u8 player1_old_y = 0;

s8 player1_direction_x = 0;
s8 player1_direction_y = 0;

u16 player1_points = 0;

u8 joypad1_data = 0;
u8 joypad2_data = 0;

u8 temp;
u8 frame_counter = 0;

#define LEVEL_OCTET_SIZE (32*29)
u8 level[LEVEL_OCTET_SIZE];

int i;
int j;

#define ppu_control(a) *((u8 *) 0x2000) = a;

#define ppu_background_scrolling_offset(a, b) *((u8*) 0x2005) = a; *((u8*) 0x2005) = b;

#define vram_address_register(a, b) *((u8*) 0x2006) = a; *((u8*) 0x2006) = b;

#define vram_write(a) *((u8*) 0x2007) = a;

#define ppu_control_register_2(a) *((u8*) 0x2001) = a;

#define set_name_position_to_vram_with_offset(o, x, y) vram_address_register(o + (y >> 3), (((y << 5) & 0xff)+ x) & 0xff); 

#define set_name_position_to_vram(x, y)  set_name_position_to_vram_with_offset(0x20, x, y);

#define joypad_strobe *((u8 *) 0x4016) = 0x01; *((u8 *) 0x4016) = 0x00;

#define joypad1_read (*((u8*) 0x4016) & 0x01)
#define joypad2_read (*((u8*) 0x4017) & 0x01)


#define set_char(x, y, c) ; set_name_position_to_vram(x, y); vram_write(c);

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

void joypad_direction(u8 joypad_data, s8* direction_x, s8* direction_y)
{
	if (joypad_data & 0x01) {
		*direction_y = 0;
		*direction_x = 1;
	} else if (joypad1_data & 0x02) {
		*direction_y = 0;
		*direction_x = -1;
	} else if (joypad1_data & 0x04) {
		*direction_x = 0;
		*direction_y = 1;
	} else if (joypad1_data & 0x08) {
		*direction_x = 0;
		*direction_y = -1;
	}
}

int can_avatar_walk_there(u8 x, u8 y)
{
	i = ((int)y << 5) + x;
	temp = level[i];
	return temp == '.' || temp == ' ';
}

void check_new_direction(u8 joypad_data, u8 player_x, u8 player_y, s8 *player_direction_x, s8 *player_direction_y)
{
	u8 next_x;
	u8 next_y;
	s8 new_player_direction_x = 0;
	s8 new_player_direction_y = 0;

	joypad_direction(joypad_data, &new_player_direction_x, &new_player_direction_y);
	if (new_player_direction_y == 0 && new_player_direction_x == 0) {
		return;
	}
	next_x = player_x + new_player_direction_x;
	next_y = player_y + new_player_direction_y;
	if (can_avatar_walk_there(next_x, next_y)) {
		*player_direction_x = new_player_direction_x;
		*player_direction_y = new_player_direction_y;
	}
}

void move_avatar(u8* player_x, u8* player_y, s8 player_direction_x, s8 player_direction_y)
{
	*player_x += player_direction_x;
	*player_y += player_direction_y;
}

void fastcall clear_char(u8 player_x, u8 player_y)
{
	i = ((int)player_y << 5) + player_x;
	set_char(player_x, player_y, level[i]);
}

void check_avatar_needs_to_stop(u8 player_x, u8 player_y, s8* player_direction_x, s8* player_direction_y)
{
	u8 next_x = player_x + *player_direction_x;
	u8 next_y = player_y + *player_direction_y;
	if (!can_avatar_walk_there(next_x, next_y)) {
		*player_direction_x = 0;
		*player_direction_y = 0;
	}
}

void check_if_pill(u8 player_x, u8 player_y, u16* points)
{
	i = ((int)player_y << 5) + player_x;
	temp = level[i];
	if (temp == '.') {
		level[i] = ' ';
		 ++*points;
	}
}

void fastcall display_score(u16 score, u8 x)
{
	set_name_position_to_vram(x, 3);
	vram_write(48 + (score / 100) % 10);
	vram_write(48 + (score / 10) % 10);
	vram_write(48 + score % 10);
}


void game_loop()
{
	frame_counter++;

	read_joypads();
	check_new_direction(joypad1_data, player1_x, player1_y, &player1_direction_x, &player1_direction_y);
	if (frame_counter % 4 != 0) {
		return;
	}
	check_avatar_needs_to_stop(player1_x, player1_y, &player1_direction_x, &player1_direction_y);
	move_avatar(&player1_x, &player1_y, player1_direction_x, player1_direction_y);
	check_if_pill(player1_x, player1_y, &player1_points);
}

void setup_colors()
{
	vram_address_register(0x3f, 0x00);
	vram_write(0x01);

	vram_address_register(0x3f, 0x03);
	vram_write(0x30);
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
	;
	
	
int count_underscores() {
	int count = 0;

	for (i = 0; i < LEVEL_OCTET_SIZE; i++) {
		if (level[i] == '.') count++;
	}

	return count;
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

void fastcall do_all_rendering(void)
{
	clear_char(player1_old_x, player1_old_y);
	asm("ldx %v", player1_x);
	asm("ldy %v", player1_y);
	draw_avatar();
	display_score(player1_points, 3);
}


int main()
{
	waitvblank();
	ppu_control(0);
	setup_colors();
	copy_to_active_level();
	draw_level();

	enable_screen();
	while (1) {
		wait_for_that_vsync();
		waitvblank();
		do_all_rendering();
		zero_screen_position();
		player1_old_x = player1_x;
		player1_old_y = player1_y;

		game_loop();
	}

	return 0;
}
