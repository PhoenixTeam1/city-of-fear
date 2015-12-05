#include <png.h>
#include <GL/glut.h>
#include <GL/freeglut.h>
#include <signal.h>
#include <stdio.h>

#include "constants.h"
#include "visualization.h"
#include "entity.h"
#include "cell.h"

#define VISUALIZATION_SCALAR	2
#define RGB_SIZE		3

extern int lattice_height;
extern int lattice_width;
extern int running;
extern cell_t lattice[][LATTICE_WIDTH];

typedef struct rgb_t {
	unsigned char r;
	unsigned char g;
	unsigned char b;
} rgb_t;

unsigned char* visualLattice;

void saveLatticeSnapshot(char* basename, int timestep) {
	char buffer[64];
	sprintf(buffer, "%s-snapshot-timestep-%02d.png", basename, timestep);
	createLatticeImage(buffer);
	return;
}

void createLatticeImage(char* filename) {
	int i;
	int j;
	pixel_t* pixels;
	pixel_t* pixel;
	pixel_t latticePixel;
	float rowIndex;
	float colIndex;

	pixels = (pixel_t*)malloc(sizeof(pixel_t)*lattice_height*VISUALIZATION_SCALAR*lattice_width*VISUALIZATION_SCALAR);

	for (i = 0; i < lattice_height*VISUALIZATION_SCALAR; i++) {
		for (j = 0; j < lattice_width*VISUALIZATION_SCALAR; j++) {
			rowIndex = ((float)i) / (float)VISUALIZATION_SCALAR;
			colIndex = (float)j / (float)VISUALIZATION_SCALAR;
			latticePixel = getColorRepresentation(&lattice[(int)rowIndex][(int)colIndex]);

			pixel = &pixels[((lattice_height*VISUALIZATION_SCALAR-i-1)*lattice_width*VISUALIZATION_SCALAR)+j];
			pixel->red = latticePixel.red;
			pixel->green = latticePixel.green;
			pixel->blue = latticePixel.blue;
		}
	}
	savePNG(pixels, filename);
	free(pixels);
	return;
}

int savePNG(pixel_t *pixels, const char *path) {
	FILE* fp;
	int i;
	int j;
	int status = -1;
	int pixel_size = 3;
	int depth = 8;
	png_structp png_ptr = NULL;
	png_infop info_ptr = NULL;
	png_byte** row_pointers = NULL;

	fp = fopen(path, "wb");
	if (!fp) {
		goto fopen_failed;
	}

	png_ptr = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
	if (png_ptr == NULL) {
		goto png_create_write_struct_failed;
	}

	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		goto png_create_info_struct_failed;
	}

	if (setjmp(png_jmpbuf (png_ptr))) {
		goto png_failure;
	}

	png_set_IHDR (png_ptr,
			info_ptr,
			lattice_width*VISUALIZATION_SCALAR,
			lattice_height*VISUALIZATION_SCALAR,
			depth,
			PNG_COLOR_TYPE_RGB,
			PNG_INTERLACE_NONE,
			PNG_COMPRESSION_TYPE_DEFAULT,
			PNG_FILTER_TYPE_DEFAULT);

	row_pointers = png_malloc(png_ptr, lattice_height*VISUALIZATION_SCALAR * sizeof(png_byte *));
	for (i = 0; i < lattice_height*VISUALIZATION_SCALAR; i++) {
		png_byte* row = png_malloc(png_ptr, sizeof(uint8_t) * lattice_width*VISUALIZATION_SCALAR * pixel_size);
		row_pointers[i] = row;
		for (j = 0; j < lattice_width*VISUALIZATION_SCALAR; j++) {
			pixel_t* pixel = &pixels[(i*lattice_width*VISUALIZATION_SCALAR)+j];
			*row++ = pixel->red;
			*row++ = pixel->green;
			*row++ = pixel->blue;
		}
	}

	png_init_io (png_ptr, fp);
	png_set_rows (png_ptr, info_ptr, row_pointers);
	png_write_png (png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

	status = 0;

	for (i = 0; i < lattice_height*VISUALIZATION_SCALAR; i++) {
		png_free (png_ptr, row_pointers[i]);
	}
	png_free (png_ptr, row_pointers);

	png_failure:
	png_create_info_struct_failed:
	png_destroy_write_struct (&png_ptr, &info_ptr);
	png_create_write_struct_failed:
	fclose (fp);
	fopen_failed:
	return status;
}

void drawLattice(void) {
	int i;
	int j;
	pixel_t color;
	glRasterPos2f(-1, -1);
	for (i = 0; i < lattice_height; i++) {
		for (j = 0; j < lattice_width; j++) {
			color = getColorRepresentation(&lattice[i][j]);
			visualLattice[3*(i*lattice_width+j)+0] = color.red;
			visualLattice[3*(i*lattice_width+j)+1] = color.green;
			visualLattice[3*(i*lattice_width+j)+2] = color.blue;
		}
	}
	glPixelZoom(VISUALIZATION_SCALAR, VISUALIZATION_SCALAR);
	glDrawPixels(lattice_width, lattice_height, GL_RGB, GL_UNSIGNED_BYTE, visualLattice);
	glFlush();
	glutSwapBuffers();
	glutPostRedisplay();
	if (!running) glutLeaveMainLoop();
}

void *visualizer(void* args) {
	visualLattice = (unsigned char*)malloc(lattice_height*lattice_width*RGB_SIZE);
	int argc = 0;
	char** argv = 0;

	// GLUT initialization
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_ALPHA | GLUT_DEPTH);
	glutInitWindowSize(lattice_width*VISUALIZATION_SCALAR, lattice_height*VISUALIZATION_SCALAR);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("City of Fear");

	// GLUT setup stuff
	glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_CONTINUE_EXECUTION);
	glutDisplayFunc(drawLattice);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	signal(SIGINT, intHandler); // have to re-register these handlers since glut overwrote them
	signal(SIGKILL, intHandler);
	glutMainLoop();
	free(visualLattice);
	return 0;
}

pixel_t getColorRepresentation(cell_t* cell) {
	pixel_t color;
	if (cell->occupant == NULL) {
		switch (cell->type) {
			case type_barrier:
				// Black
				color.red = 0;
				color.green = 0;
				color.blue = 0;
				break;
			case type_open:
				// Grey
				color.red = 100;
				color.green = 100;
				color.blue = 100;
				break;
/*			case ????:
				// Yellow
				color.red = 255;
				color.green = 253;
				color.blue = 0;
*/	
			default:
				// White
				color.red = 255;
				color.green = 255;
				color.blue = 255;
				break;
		}
	}
	else {
		switch (cell->occupant->type) {
			case type_civilian:
				// Orange
				color.red = 255;
				color.green = 76;
				color.blue = 0;
				break;
			case type_police:
				// Purple
				color.red = 84;
				color.green = 14;
				color.blue = 173;
				break;
			case type_zombie:
				// Green
				color.red = 0;
				color.green = 174;
				color.blue = 104;
				break;
			default:
				// White
				color.red = 255;
				color.green = 255;
				color.blue = 255;
				break;
		}
	}
	return color;
}


void intHandler() {
	printf("Shutting down...\n");
	running = 0;
	return;
}

