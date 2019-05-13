from math import floor
from perlin import PerlinNoiseFactory

def block(x, y, z):
    print('<group R="1" G="1" B="1">')
    print('  <translate x="{}" y="{}" z="{}" />'.format(x, y, z))
    print('  <models>')
    print('    <model file="models/box.3d" ambiR="0.5" ambiG="0.5" ambiB="0.5" texture="assets/grass_block.png" />')
    print('  </models>')
    print('</group>')

sky = [0.529, 0.808, 0.922]
print('<scene>')
print('<lights>')
print('  <light type="POINT" y="10" x="5" z="5"/>')
print('</lights>')
print('<group R="{}" G="{}" B="{}">'.format(sky[0], sky[1], sky[2]))
print('  <scale x="-200" y="-200" z="-200" />')
print('  <models>')
print('    <model file="models/sphere.3d" diffR="{}" diffG="{}" diffB="{}" />'.format(sky[0], sky[1], sky[2]))
print('  </models>')
print('</group>')

PNFactory = PerlinNoiseFactory(2)

x_size = 100
y_size = 100

for x in range(x_size):
    for z in range(y_size):
        y = floor( PNFactory(x / 16, z / 16) * 4 )
        block(x - (x_size / 2), y, z - (y_size / 2))


print('</scene>')
