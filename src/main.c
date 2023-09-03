#include <stdio.h>
#include <stdint.h>
#include <assert.h>

#include "harfbuzz/hb.h"
#include "freetype/freetype.h"

#define HARFBUZZ_PRECISION_FACTOR 64.0 

int main() {

	// char *text = "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.";

	char *text = "Hello World!";
	char *csv_filename = "glyph_positions.csv";
	FILE *csv_handler = fopen(csv_filename, "w");
	fprintf(csv_handler, "character,glyph_id,x_offset,y_offset,x_advance,y_advance,cursor_x,cursor_y\n");
	char *font_filepath = "/usr/share/fonts/google-noto/NotoSansMath-Regular.ttf";
	double font_size = 35.0;
	printf("Font size: %d\n", font_size);	


	/* Initialize FreeType and create FreeType font face. */
	FT_Library ft_library;
	FT_Face ft_face;
	FT_Error ft_error;

	if ((ft_error = FT_Init_FreeType (&ft_library)))
		abort();
	if ((ft_error = FT_New_Face (ft_library, font_filepath, 0, &ft_face)))
		abort();
	if ((ft_error = FT_Set_Char_Size (ft_face, font_size * 64.0, font_size * 64.0, 0, 0)))
		abort();

	/* Create hb-ft font. */
	hb_font_t *hb_font;
	hb_font = hb_ft_font_create(ft_face, NULL);



	hb_buffer_t *buf = hb_buffer_create();
        hb_buffer_add_utf8(buf, text, -1, 0, -1);

	hb_buffer_set_direction(buf, HB_DIRECTION_LTR);
        hb_buffer_set_script(buf, HB_SCRIPT_LATIN);
        hb_buffer_set_language(buf, hb_language_from_string("en", -1));

	// printf("Font data: %f\n", font->ptem);
	
	hb_shape(hb_font, buf, NULL, 0);
	unsigned int glyph_count;
        hb_glyph_info_t *glyph_info    = hb_buffer_get_glyph_infos(buf, &glyph_count);
        hb_glyph_position_t *glyph_pos = hb_buffer_get_glyph_positions(buf, &glyph_count);


	double cursor_x = 0.0;
	double cursor_y = 0.0;
	for (unsigned int i = 0; i < glyph_count; i++) {
		char current_char = text[i];
		hb_codepoint_t glyphid  = glyph_info[i].codepoint;
		double x_offset  = (double) glyph_pos[i].x_offset / HARFBUZZ_PRECISION_FACTOR;
		double y_offset  = (double) glyph_pos[i].y_offset / HARFBUZZ_PRECISION_FACTOR;
		double x_advance = (double) glyph_pos[i].x_advance / HARFBUZZ_PRECISION_FACTOR;
		double y_advance = (double) glyph_pos[i].y_advance / HARFBUZZ_PRECISION_FACTOR;	

		fprintf(
			csv_handler,
			"%c,%d,%f,%f,%f,%f,%f,%f\n",
			current_char,
			glyphid, 
			x_offset, y_offset,
			x_advance, y_advance, 
			cursor_x, cursor_y
		);

		cursor_x += x_advance;
		cursor_y += y_advance;

	}


	hb_buffer_destroy(buf);
	FT_Done_FreeType(ft_library);
	fclose(csv_handler);
	return 1;
}
