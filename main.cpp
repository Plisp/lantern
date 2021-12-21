#include <stdio.h>

#include "display_sdl.h"
#include "libfov.h"
#include "map.h"

#define FOVRADIUS	100

/* Global variables ----------------------------------------------- */

MAP map(3);
unsigned radius = 20;
fov_direction_type direction = FOV_EAST;
float angle = 130.0f;
bool beam = false;
bool apply_to_opaque = true;
fov_settings_type fov_settings;

/* Callbacks ------------------------------------------------------ */

/**
 * Function called by libfov to apply light to a cell.
 *
 * \param map Pointer to map data structure passed to function such as
 *            fov_circle.
 * \param x   Absolute x-axis position of cell.
 * \param y   Absolute x-axis position of cell.
 * \param dx  Offset of cell from source cell on x-axis.
 * \param dy  Offset of cell from source cell on y-axis.
 * \param src Pointer to source data structure passed to function such
 *            as fov_circle.
 */
void apply(void *map, int x, int y, int dx, int dy, void *src) {
	if (((MAP *)map)->onMap(x, y))
		((MAP *)map)->setSeen(x, y);
}


/**
 * Function called by libfov to determine whether light can pass
 * through a cell. Return zero if light can pass though the cell at
 * (x,y), non-zero if it cannot.
 *
 * \param map Pointer to map data structure passed to function such as
 *            fov_circle.
 * \param x   Absolute x-axis position of cell.
 * \param y   Absolute x-axis position of cell.
 */
bool opaque(void *map, int x, int y) {
	return ((MAP *)map)->blockLOS(x, y);
}


/* Functions ------------------------------------------------------ */

/**
 * Offset player coordinates by (dx,dy). e.g. player_move(1,0) moves
 * the player right one cell.
 */
void player_move(int dx, int dy) {
	unsigned newx = pX + dx;
	unsigned newy = pY + dy;
	if (map.onMap(newx, newy)) {
		pX = newx;
		pY = newy;
	}
}


/**
 * Redraw the screen. Called in a loop to create the output.
 */
void redraw(void) {
	display_clear();

	/* Cause libfov to mark lit cells using the two callbacks given:
	 *  - opaque, which is used to determine information about your
	 *    map (a query you can define), and
	 *  - apply, which is used to modify your map once libfov
	 *    determines that a particular cell is lit (a command you can
	 *    define).
	 *
	 * In this call, the light source is at (pX,pY). We pass NULL for
	 * the source pointer parameter because we have no data structure
	 * defining our light source in this example. You may want to pass
	 * a pointer to a data structure holding your light source if, for
	 * example, you wanted different coloured lights. In that case the
	 * colour of the cell would depend upon the colour of the source
	 * and you would have to extract this colour from your data
	 * structure in each call to apply.
	 */
    if (beam) {
        fov_beam(&fov_settings, &map, NULL, pX, pY, radius, direction, angle);
    } else {
        fov_circle(&fov_settings, &map, NULL, pX, pY, radius);
    }

	map.display();
	display_put_char('@', pX, pY, 0x00, 0xFF, 0x00);
	display_refresh();
}

/**
 * Clean up and exit.
 */
void normal_exit(void) {
    display_exit();
    fov_settings_free(&fov_settings);
    exit(0);
}

/**
 * Handle a keypress. Callback used by display_event_loop.
 */
void keypressed(int key, int shift) {
	switch (key) {
	case SDLK_k:
		player_move( 0, -1);
        direction = FOV_NORTH;
        break;
	case SDLK_j:
		player_move( 0,  1); 
        direction = FOV_SOUTH;
        break;
	case SDLK_h:
		player_move(-1,  0);
        direction = FOV_WEST;
        break;
	case SDLK_l:
		player_move( 1,  0); 
        direction = FOV_EAST;
        break;
    case SDLK_y:
		player_move(-1, -1);
        direction = FOV_NORTHWEST;
        break;
    case SDLK_u:
		player_move( 1, -1); 
        direction = FOV_NORTHEAST;
        break;
    case SDLK_b:
		player_move(-1,  1);
        direction = FOV_SOUTHWEST;
        break;
    case SDLK_n:
		player_move( 1,  1); 
        direction = FOV_SOUTHEAST;
        break;
	case SDLK_EQUALS:
        radius++; 
        printf("Increased radius to %d\n", radius);
        break;
	case SDLK_MINUS:
        radius--; 
        if (radius <= 0)
            radius = 1; 
        printf("Decreased radius to %d\n", radius);
        break;
	case SDLK_RIGHTBRACKET:
        angle += 5.0;
        if (angle >= 360.0f)
            angle = 360.0f; 
        printf("Increased angle to %0.1f\n", angle);
        break;
	case SDLK_LEFTBRACKET:
        angle -= 5.0;
        if (angle <= 0.0f)
            angle = 0.0f; 
        printf("Decreased angle to %0.1f\n", angle);
        break;
	case SDLK_p: 
        fov_settings_set_shape(&fov_settings, FOV_SHAPE_CIRCLE_PRECALCULATE);
        printf("Precalculated circular limit\n");
        break;
	case SDLK_a:
		fov_settings_set_opaque_apply(&fov_settings, (fov_opaque_apply_type)(!fov_settings.opaque_apply));
		printf("Toggling applying to opaque tiles (%d)\n", fov_settings.opaque_apply);
		break;
	case SDLK_c:
		beam = !beam;
		printf("Toggling beam (%d)\n", beam);
		break;
	case SDLK_q:
        normal_exit();
		break;
	default:
		break;
	}
	redraw();
}


/**
 * Program entry point.
 */
int main (int argc, char *argv[]) {
    fov_settings_init(&fov_settings);
    fov_settings_set_opacity_test_function(&fov_settings, opaque);
    fov_settings_set_apply_lighting_function(&fov_settings, apply);

	display_init();
	redraw();
	display_event_loop(keypressed);
    normal_exit();
    return 0;
}
