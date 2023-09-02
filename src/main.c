#include <stdio.h>

#include "harfbuzz/hb.h"
#include "freetype/freetype.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

typedef struct {
    uint8_t r, g, b, a;
} Pixel32;

typedef struct {
    size_t width;
    size_t height;
    Pixel32 *pixels;
} Image32;

void save_image32_to_png(Image32 image, const char *filepath)
{
    int ok = stbi_write_png(filepath, image.width, image.height, 4,
                            image.pixels, image.width * sizeof(Pixel32));

    if (!ok) {
        fprintf(stderr, "Could not save file `%s`\n", filepath);
        exit(1);
    }
}




void slap_ftbitmap_onto_image32(Image32 dest,
                                FT_Bitmap *src,
                                Pixel32 color,
                                int x, int y)
{
    assert(src->pixel_mode == FT_PIXEL_MODE_GRAY);
    assert(src->num_grays == 256);

    for (size_t row = 0; (row < src->rows); ++row) {
        if (row + y < dest.height) {
            for (size_t col = 0; (col < src->width); ++col) {
                if (col + x < dest.width) {
                    color.a = src->buffer[row * src->pitch + col];
                    dest.pixels[(row + y) * dest.width + col + x] =
                        mix_pixels(
                            dest.pixels[(row + y) * dest.width + col + x],
                            color);
                }
            }
        }
    }
}



void draw_glyph(Image32 surface, FT_Face face, hb_codepoint_t glyphid,
                double x, double y)
{
    printf("Drawing glyph at (%lf, %lf)\n", x, y);

    FT_Error error = FT_Load_Glyph(face, glyphid, FT_LOAD_DEFAULT);
    if (error) {
        fprintf(stderr, "Could not load glyph (codepoint: %d)\n",
                glyphid);
        exit(1);
    }

    error = FT_Render_Glyph(face->glyph, FT_RENDER_MODE_NORMAL);
    if (error) {
        fprintf(stderr, "Could not render glyph (codepoint: %d)\n",
                glyphid);
        exit(1);
    }

    Pixel32 FONT_COLOR = {200, 200, 200, 255};

    slap_ftbitmap_onto_image32(surface,
                               &face->glyph->bitmap,
                               FONT_COLOR,
                               (int) floor(x) + face->glyph->bitmap_left,
                               (int) floor(y) - face->glyph->bitmap_top);

}




int main() {

	// char *text = "Lorem Ipsum is simply dummy text of the printing and typesetting industry. Lorem Ipsum has been the industry's standard dummy text ever since the 1500s, when an unknown printer took a galley of type and scrambled it to make a type specimen book. It has survived not only five centuries, but also the leap into electronic typesetting, remaining essentially unchanged. It was popularised in the 1960s with the release of Letraset sheets containing Lorem Ipsum passages, and more recently with desktop publishing software like Aldus PageMaker including versions of Lorem Ipsum.";

	char *text = "Hello World!";
	char *png_filepath = "test.png";
	char *filename = "/usr/share/fonts/google-noto/NotoSansMath-Regular.ttf";

	FILE *png_file = fopen(png_filepath, "wb");
	fclose(png_file);

	FT_Library ft_library;
	FT_Error error = FT_Init_FreeType( &ft_library );
	if ( error )
	{
		printf("Error while starting the freetype library!");
	}


	hb_buffer_t *buf = hb_buffer_create();
        hb_buffer_add_utf8(buf, text, -1, 0, -1);

	hb_buffer_set_direction(buf, HB_DIRECTION_LTR);
        hb_buffer_set_script(buf, HB_SCRIPT_LATIN);
        hb_buffer_set_language(buf, hb_language_from_string("en", -1));

	
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
		// draw_glyph(glyphid, cursor_x + x_offset, cursor_y + y_offset);
	}




	FT_Done_FreeType(ft_library);

	return 1;
}
