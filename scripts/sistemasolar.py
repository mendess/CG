#!/bin/python3
import random
from random import uniform, gauss
from math import sin, cos, pi, sqrt

TRANSLATE_STEPS = 15

class Planet:
    def __init__(self, name, radius, r, g, b, distance=None, moons=[], rings=[], orbit_x=None, orbit_y=None, texture=None):
        self.name = name
        self.distance = distance
        self.radius = radius
        self.r = r
        self.g = g
        self.b = b
        self.moons = moons
        self.rings = rings
        if orbit_x is None:
            self.orbit_x = distance
        else:
            self.orbit_x = orbit_x
        if orbit_y is None:
            self.orbit_y = distance
        else:
            self.orbit_y = orbit_y
        self.texture = texture

    def print_planet(self, indent=8):
        time = uniform(20, 40);
        print(' ' * indent, '<!-- {} -->'.format(self.name))
        if self.texture is None:
            print(' ' * indent, '<group R="{}" G="{}" B="{}">'.format(self.r, self.g, self.b))
        else:
            print(' ' * indent, '<group R="1" G="1" B="1">')
        print(' ' * indent, '    <translate time="{}">'.format(time))
        for i in range(TRANSLATE_STEPS):
            a = ((2 * pi)/TRANSLATE_STEPS) * i
            x = self.orbit_x * cos(a)
            z = self.orbit_y * sin(a)
            print(' ' * indent, '        <point X="{}" Z="{}"/>'.format(x, z))
        print(' ' * indent, '    </translate>')
        print(' ' * indent, '    <scale X="{0}" Y="{0}" Z="{0}" />'.format(self.radius))
        print(' ' * indent, '    <models>')
        if self.texture is None:
            print(' ' * indent, '        <model file="models/sphere.3d"/>')
        else:
            print(' ' * indent, '        <model file="models/sphere.3d" texture="{}" AMBIR="0.001" AMBIB="0.001" AMBIG="0.001"/>'.format(self.texture))
        print(' ' * indent, '    </models>')
        for moon in self.moons:
            moon.print_moon()
        for ring in self.rings:
            ring.print_ring()
        print(' ' * indent, '</group>')

    def print_moon(self, indent=12):
        shift = uniform(0, 2 * pi)
        print(' ' * indent, '<!-- {} -->'.format(self.name))
        print(' ' * indent, '<group R="{}" G="{}" B="{}" >'.format(self.r, self.g, self.b))
        print(' ' * indent, '    <rotate x="{}" y="{}" z="{}" angle="{}" />'.format(uniform(0, 1), 0, uniform(0, 1), uniform(15, 50)))
        print(' ' * indent, '    <translate time="{}">'.format(10))
        for i in range(TRANSLATE_STEPS):
            a = ((2 * pi)/TRANSLATE_STEPS) * i
            x = self.orbit_x * sin(a)
            z = self.orbit_y * cos(a)
            print(' ' * indent, '        <point X="{}" Z="{}"/>'.format(x, z))
        print(' ' * indent, '    </translate>')
        print(' ' * indent, '    <rotate Y="1" time="10" />')
        print(' ' * indent, '    <scale X="{0}" Y="{0}" Z="{0}" />'.format(self.radius))
        print(' ' * indent, '    <models>')
        if self.texture is None:
            print(' ' * indent, '        <model file="models/sphere.3d" diffR="{}" diffG="{}" diffB="{}"/>'.format(self.r, self.g, self.b))
        else:
            print(' ' * indent, '        <model file="models/sphere.3d" texture="{}" AMBIR="0.001" AMBIB="0.001" AMBIG="0.001"/>'.format(self.texture))
        print(' ' * indent, '    </models>')
        print(' ' * indent, '</group>')


def cross(a, b):
    res = [0,0,0]
    res[0] = a[1] * b[2] - a[2] * b[1]
    res[1] = a[2] * b[0] - a[0] * b[2]
    res[2] = a[0] * b[1] - a[1] * b[0]
    return res

def make_rand_vector():
    vec = [gauss(0, 1) for i in range(3)]
    mag = sum(x**2 for x in vec) ** .5
    return [x/mag for x in vec]

def make_rand_vector_par(u):
    vec = [(-u[0] - u[2]) / u[0], 1, 1]
    mag = sum(x**2 for x in vec) ** .5
    return [x/mag for x in vec]

class Ring:
    def __init__(self, r, g, b, rx, ry, rz, angle, sx, sy, sz, texture=None):
        self.r = r
        self.g = g
        self.b = b
        self.rx = rx
        self.ry = ry
        self.rz = rz
        self.angle = angle
        self.sx = sx
        self.sy = sy
        self.sz = sz
        self.texture = texture

    def print_ring(self, indent=12):
        print(' ' * indent, '<group R="{}" G="{}" B="{}">'.format(self.r, self.g, self.b))
        print(' ' * indent, '    <rotate X="{}" Y="{}" Z="{}" angle="{}" />'.format(self.rx, self.ry, self.rz, self.angle))
        print(' ' * indent, '    <scale X="{}" Y="{}" Z="{}" />'.format(self.sx, self.sy, self.sz))
        print(' ' * indent, '    <models>')
        if self.texture is not None:
            print(' ' * indent, '        <model file="models/torus.3d" texture="{}" diffR="{}" diffG="{}" diffB="{}"/>'.format(self.texture, self.r, self.g, self.b))
        else:
            print(' ' * indent, '        <model file="models/torus.3d" diffR="{}" diffG="{}" diffB="{}"/>'.format(self.r, self.g, self.b))
        print(' ' * indent, '    </models>')
        print(' ' * indent, '</group>')


def draw_asteroids(number, min_distance, max_distance, indent=8):
    for i in range(number):
        time     = uniform(5, 40)
        distance = uniform(min_distance, max_distance)
        size     = uniform(0.005, 0.05)
        STEPS    = 100
        shift    = uniform(0, 2 * pi)
        print(' ' * indent, '<!-- Asteroid {} -->'.format(i))
        print(' ' * indent, '<group R="0.6" G="0.6" B="0.6" >')
        print(' ' * indent, '    <translate time="{}">'.format(time))
        for i in range(TRANSLATE_STEPS):
            a = ((2 * pi) / TRANSLATE_STEPS) * i + shift
            x = distance * sin(a)
            z = distance * cos(a)
            print(' ' * indent, '        <point X="{}" Z="{}"/>'.format(x, z))
        print(' ' * indent, '    </translate>')
        print(' ' * indent, '    <scale X="{0}" Y="{0}" Z="{0}" />'.format(0.01))
        print(' ' * indent, '    <models>')
        print(' ' * indent, '        <model file="models/sphere.3d" diffR="0.6" diffG="0.6" diffB="0.6"/>')
        print(' ' * indent, '    </models>')
        print(' ' * indent, '</group>')


def draw_commet(indent=8):
    time     = 20
    distance_x = 15
    distance_z = 8
    STEPS    = 100
    print(' ' * indent, '<!-- Commet -->')
    print(' ' * indent, '<group R="1" G="0.6" B="0" >')
    print(' ' * indent, '    <translate time="{}">'.format(time))
    yShift = 10
    for i in range(TRANSLATE_STEPS):
        a = ((2 * pi) / TRANSLATE_STEPS) * i
        x = distance_x * sin(a) + distance_x * .75
        z = distance_z * cos(a)
        print(' ' * indent, '        <point X="{}" Z="{}"/>'.format(x, z))
    print(' ' * indent, '    </translate>')
    print(' ' * indent, '    <scale X="{0}" Y="{0}" Z="{0}" />'.format(0.01))
    print(' ' * indent, '    <models>')
    print(' ' * indent, '        <model file="models/teapot.3d"/>')
    print(' ' * indent, '    </models>')
    print(' ' * indent, '</group>')

earth_moon = [
        Planet('Moon', 0.25, 0.8, 0.8, 0.8, 2.0514, texture='assets/2k_moon.jpg')
        ]
jupiter_moons = [
        Planet('Europa',   0.0222,  0.8, 0.8, 0.8, 2.0896),
        Planet('Io',       0.03181, 0.8, 0.8, 0.8, 2.0468),
        Planet('Ganymede', 0.03704, 0.8, 0.8, 0.8, 2.1432),
        Planet('Callisto', 0.03448, 0.8, 0.8, 0.8, 2.2514)
        ]
saturn_rings = [Ring(0.803, 0.702, 0.503, 1, 0, 0.5, 90, 1.5, 1.5, 0.1, texture='assets/8k_saturn_ring_alpha_full.png')]
saturn_moons = [Planet('Titan', 0.04425, 0.8, 0.8, 0.8, 2.1604)]
uranus_moons = [
        Planet('Miranda' , 0.0037728, 0.8, 0.8, 0.8, 2.035120),
        Planet('Ariel'   , 0.0092624, 0.8, 0.8, 0.8, 2.528160),
        Planet('Umbriel' , 0.0093552, 0.8, 0.8, 0.8, 3.130400),
        Planet('Titania' , 0.0126144, 0.8, 0.8, 0.8, 4.487280),
        Planet('Oberon'  , 0.0121824, 0.8, 0.8, 0.8, 5.668160)
        ]
neptune_moons = [
        Planet('Triton', 0.0000205, 0.8, 0.8, 0.8, 16.27)
        ]

mercury = Planet('Mercury', orbit_x=1.8,  orbit_y=1.8,  radius=0.0216, r=1.0,    g=0,      b=0, texture="assets/8k_mercury.jpg")
venus   = Planet('Venus',   orbit_x=2.2,  orbit_y=2.2,  radius=0.0531, r=0.0,    g=1,      b=0, texture="assets/8k_venus_surface.jpg")
earth   = Planet('Earth',   orbit_x=2.5,  orbit_y=2.5,  radius=0.0555, r=0.0,    g=0.52,      b=0.74, moons=earth_moon, texture="assets/8k_earth_daymap.jpg")
mars    = Planet('Mars',    orbit_x=3.4,  orbit_y=3.4,  radius=0.0333, r=0.5,    g=0,      b=0, texture="assets/8k_mars.jpg")
jupiter = Planet('Jupiter', orbit_x=7.2,  orbit_y=7.2,  radius=0.6111, r=0.6030, g=0.490,  b=0.373, moons=jupiter_moons, texture="assets/8k_jupiter.jpg")
saturn  = Planet('Saturn',  orbit_x=13.6, orbit_y=13.6, radius=0.4888, r=0.9290, g=0.800,  b=0.6588, moons=saturn_moons, rings=saturn_rings, texture="assets/8k_saturn.jpg")
uranus  = Planet('Uranus',  orbit_x=27.2, orbit_y=32.6, radius=0.2052, r=0.5882, g=0.7215, b=0.7647, moons=uranus_moons, texture="assets/2k_uranus.jpg")
neptune = Planet('Neptune', orbit_x=40,   orbit_y=40,   radius=0.1887, r=0.2352, g=0.4627, b=0.6588, moons=neptune_moons, texture="assets/2k_neptune.jpg")
pluto   = Planet('Pluto',   orbit_x=53.6, orbit_y=103.6, radius=0.01,   r=0.5607, g=0.5294, b=0.5058)

planets = [ mercury, venus, earth, mars, jupiter, saturn, uranus, neptune, pluto ]

print('<scene>')
print('    <lights>')
print('        <light type="Point" />')
print('    </lights>')
print('    <!--Sun-->')
print('    <group R="1" G="1" B="1" A="1">')
print('        <scale X="20" Y="20" Z="20" />')
print('        <models>')
print('            <model file="models/sphere.3d" texture="assets/8k_sun.jpg" EMISR="1" EMISG="1" EMISB="1"/>')
print('        </models>')
for planet in planets:
    planet.print_planet()
draw_commet()
draw_asteroids(100, 4.7, 5.3)
print('    </group>')
print('</scene>')
