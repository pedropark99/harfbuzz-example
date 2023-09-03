#include <stdio.h>

#include "raylib.h"


int main()
{
	char *text = "Hello World!";
	char *png_filepath = "test.png";
	char *font_filepath = "/usr/share/fonts/google-noto/NotoSansMath-Regular.ttf";
	// Color black_color = GetColor(0x00); 
	Image image = GenImageColor(800, 800, RAYWHITE);

	// ImageDrawRectangle(&image, 0, 0, 800, 800, RAYWHITE);
	Font font_for_image = LoadFontEx(font_filepath, 10, NULL, 0); 
	/*ImageDrawTextEx(
		&image,
		font_for_image, 
		text,
		vec_position, 
		10, 0,
		black_color
	);*/

	ExportImage(image, "test.png");


	return 1;
}
