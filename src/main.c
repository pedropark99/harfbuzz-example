#include <stdio.h>

#include "harfbuzz/hb.h"



int main() {
	char *text = "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.";

	
	hb_buffer_t *buf = hb_buffer_create();
        hb_buffer_add_utf8(buf, text, -1, 0, -1);

	hb_buffer_set_direction(buf, HB_DIRECTION_LTR);
        hb_buffer_set_script(buf, HB_SCRIPT_LATIN);
        hb_buffer_set_language(buf, hb_language_from_string("en", -1));

	
	char *filename = "/usr/share/fonts/google-noto/NotoSansMath-Regular.ttf";
	hb_blob_t *blob = hb_blob_create_from_file(filename); /* or hb_blob_create_from_file_or_fail() */
        hb_face_t *face = hb_face_create(blob, 0);
        hb_font_t *font = hb_font_create(face);
	
	hb_shape(font, buf, NULL, 0);
	unsigned int glyph_count;
        hb_glyph_info_t *glyph_info    = hb_buffer_get_glyph_infos(buf, &glyph_count);
        hb_glyph_position_t *glyph_pos = hb_buffer_get_glyph_positions(buf, &glyph_count);

	double cursor_x = 0.0;
	double cursor_y = 0.0;
	for (unsigned int i = 0; i < glyph_count; i++) {
		hb_codepoint_t glyphid  = glyph_info[i].codepoint;
		double x_offset  = glyph_pos[i].x_offset;
		double y_offset  = glyph_pos[i].y_offset;
		double x_advance = glyph_pos[i].x_advance;
		double y_advance = glyph_pos[i].y_advance;

		cursor_x += x_advance;
		cursor_y += y_advance;


		printf("Glyph ID: %d\n", glyphid);
		printf("x offset: %f\n", x_offset);
		printf("y offset: %f\n", y_offset);
		printf("x advance: %f\n", x_advance);
		printf("y advance: %f\n", y_advance);
		printf("cursor x: %f\n", cursor_x);
		printf("cursor y: %f\n", cursor_y);
       /* draw_glyph(glyphid, cursor_x + x_offset, cursor_y + y_offset); */
	}






	return 1;
}