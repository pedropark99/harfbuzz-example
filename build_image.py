import sys
import os


font_filepath = "/usr/share/fonts/google-noto/NotoSansMath-Regular.ttf"
font_name = "Noto-Sans-Math-Regular"
output_filename = "test.png"


magick = "magick"
background = "-background white"
text_color = "-fill black"
font_cmd = f'-font "{font_name}"'
font_size = "-pointsize 32"
image_size = "-size 800x800"
label = f"label:H"



list_cmds = [
    magick,
    background, text_color,
    image_size, font_cmd,
    font_size
]







list_cmds.append(output_filename)
magick_cmd = " ".join(list_cmds)
print(magick_cmd)
os.system(magick_cmd)
