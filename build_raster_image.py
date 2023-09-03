import os
import pandas as pd

font_filepath = "/usr/share/fonts/google-noto/NotoSansMath-Regular.ttf"
font_name = "Noto-Sans-Math-Regular"
output_filename = "test.png"
glyph_positions = pd.read_csv("glyph_positions.csv", sep = ";")
print(glyph_positions)

image_size = 2000
dpi = 300
font_size = 35
# glyph_positions["cursor_x"] = glyph_positions["cursor_x"] / font_size 


magick = "magick"
image_size = f"-size {image_size}x{image_size}"
background = "xc:white"
density = f"-density {dpi}"
text_color = "-fill black"
font_cmd = f'-font "{font_name}"'
font_size = f"-pointsize {font_size}"
label = f"label:H"

list_cmds = [
    magick
    , image_size
    , background
    #, density
    , text_color
    , font_cmd
    , font_size
]



def draw_cmd_glyph(char, x = 4.0, y = 100.0):
    t_specs = f"text {x},{y} '{char}'"
    draw_cmd = f'-draw "{t_specs}"'
    return draw_cmd


for index, row in glyph_positions.iterrows():
    list_cmds.append(
        draw_cmd_glyph(row["character"], row["cursor_x"], 100.0)
    )



list_cmds.append(output_filename)
magick_cmd = " ".join(list_cmds)
print(magick_cmd)
os.system(magick_cmd)
