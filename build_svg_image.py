import svgwrite
import pandas as pd

font_filepath = "/usr/share/fonts/google-noto/NotoSansMath-Regular.ttf"
font_name = "Noto Sans Math"
# font_name = "Arial"
output_filename = "test.svg"
glyph_positions = pd.read_csv("glyph_positions.csv")
print(glyph_positions)

harfbuzz_factor = 35
font_size = "35pt"
y_pos = 80
glyph_positions["cursor_x"] = glyph_positions["cursor_x"] / harfbuzz_factor

dwg = svgwrite.Drawing('test.svg', profile='tiny')


for index, row in glyph_positions.iterrows():
    dwg.add(dwg.text(
        row["character"],
        x = [row["cursor_x"]],
        y = [y_pos],
        fill='black',
        font_size = font_size,
        font_family = font_name
    ))


dwg.save()

