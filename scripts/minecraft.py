from math import pi, cos, floor
import sys

def block(x, y, z):
    print('<group R="1" G="1" B="1">')
    print('<translate x="{}" y="{}" z="{}" />'.format(x, y, z))
    print('<models>')
    print('<model file="scenes/box.3d" ambiR="0.5" ambiG="0.5" ambiB="0.5" texture="assets/grass_block.png" />')
    print('</models>')
    print('</group>')

print('<scene>')
print('<lights>')
print('<light type="POINT" y="10" x="5" z="5"/>')
print('</lights>')
# d = 1.0
# c = 1.0
# a = c * (-1.0 + 2.0 * (floor(d * fx) % 2.0))
# b = -c * ((floor(d * fx) % 2.0))
# y = (d - floor(d * fx)) * a + b + (c / 2.0)

# t = 1.0
# p = 1
# cenas = floor(((2 * t) / p) + 0.5)
# y = (4 / p) * (t - (p / 2) * cenas) * -1 ** cenas

ys = [-2, -1, 0, 1, 2]
y_index = 0
up = True

for x in range(-8, 8):
    for z in range(-8, 8):
        print(y_index, file=sys.stderr)
        y = ys[y_index]
        block(x, y, z)
        if up:
            y_index += 1
            if y_index >= len(ys):
                up = False
                y_index = len(ys) - 1
        else:
            y_index -= 1
            if y_index < 0:
                y_index = 0
                up = True



print('</scene>')
