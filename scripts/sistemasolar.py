#!/bin/python3
import random
from math import sin, cos, pi

class Planet:
    def __init__(self, name, radius, r, g, b, distance=None, moons=[], rings=[], orbit_x=None, orbit_y=None):
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

    def print_planet(self, indent=8):
        time = random.uniform(10, 20);
        print(" " * indent, "<!-- {} -->".format(self.name))
        print(" " * indent, "<group R=\"{}\" G=\"{}\" B=\"{}\" >".format(self.r, self.g, self.b))
        print(" " * indent, "    <translate time=\"{}\">".format(time))
        for i in range(100):
            a = ((2 * pi)/100) * i
            print(" " * indent, "        <point X=\"{}\" Z=\"{}\"/>".format(self.orbit_x * sin(a), self.orbit_y * cos(a)))
        print(" " * indent, "    </translate>")
        print(" " * indent, "    <scale X=\"{0}\" Y=\"{0}\" Z=\"{0}\" />".format(self.radius))
        print(" " * indent, "    <models>")
        print(" " * indent, "        <model file=\"scenes/sphere.3d\"/>")
        print(" " * indent, "    </models>")
        for moon in self.moons:
            moon.print_moon()
        for ring in self.rings:
            ring.print_ring()
        print(" " * indent, "</group>")

    def print_moon(self, indent=12):
        a = random.uniform(-2 * pi, 2 * pi)
        b = random.uniform(-pi / 5, pi / 5)
        x = self.distance * cos(b) * sin(a)
        y = self.distance * sin(b)
        z = self.distance * cos(b) * cos(a)
        print(" " * indent, "<!-- {} -->".format(self.name))
        print(" " * indent, "<group R=\"{}\" G=\"{}\" B=\"{}\">".format(self.r, self.g, self.b))
        print(" " * indent, "    <translate X=\"{}\" Y=\"{}\" Z=\"{}\" /> <!-- alpha: {} | radius: {} -->".format(x, y, z, a, self.distance))
        print(" " * indent, "    <scale X=\"{0}\" Y=\"{0}\" Z=\"{0}\" />".format(self.radius))
        print(" " * indent, "    <models>")
        print(" " * indent, "        <model file=\"scenes/sphere.3d\"/>")
        print(" " * indent, "    </models>")
        print(" " * indent, "</group>")

class Ring:
    def __init__(self, r, g, b, rx, ry, rz, angle, sx, sy, sz):
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

    def print_ring(self, indent=12):
        print(" " * indent, "<group R=\"{}\" G=\"{}\" B=\"{}\" A=\"0.5\">".format(self.r, self.g, self.b))
        print(" " * indent, "    <rotate X=\"{}\" Y=\"{}\" Z=\"{}\" angle=\"{}\" />".format(self.rx, self.ry, self.rz, self.angle))
        print(" " * indent, "    <scale X=\"{}\" Y=\"{}\" Z=\"{}\" />".format(self.sx, self.sy, self.sz))
        print(" " * indent, "    <models>")
        print(" " * indent, "        <model file=\"scenes/torus.3d\" />")
        print(" " * indent, "    </models>")
        print(" " * indent, "</group>")

earth_moon = [
        Planet("Moon", 0.25, 0.8, 0.8, 0.8, 2.0514)
        ]
jupiter_moons = [
        Planet("Europa",   0.0222,  0.8, 0.8, 0.8, 2.0896),
        Planet("Io",       0.03181, 0.8, 0.8, 0.8, 2.0468),
        Planet("Ganymede", 0.03704, 0.8, 0.8, 0.8, 2.1432),
        Planet("Callisto", 0.03448, 0.8, 0.8, 0.8, 2.2514)
        ]
saturn_rings = [Ring(0.803, 0.702, 0.503, 1, 0, 0.5, 90, 1.5, 1.5, 0)]
saturn_moons = [Planet("Titan", 0.04425, 0.8, 0.8, 0.8, 2.1604)]
uranus_moons = [
        Planet("Miranda" , 0.0037728, 0.8, 0.8, 0.8, 1.035120),
        Planet("Ariel"   , 0.0092624, 0.8, 0.8, 0.8, 1.528160),
        Planet("Umbriel" , 0.0093552, 0.8, 0.8, 0.8, 2.130400),
        Planet("Titania" , 0.0126144, 0.8, 0.8, 0.8, 3.487280),
        Planet("Oberon"  , 0.0121824, 0.8, 0.8, 0.8, 4.668160)
        ]
neptune_moons = [
        Planet("Triton", 0.0000205, 0.8, 0.8, 0.8, 16.27)
        ]

mercury = Planet("Mercury", orbit_x=1.8,  orbit_y=1.8,  radius=0.0216, r=1.0,    g=0,      b=0)
venus   = Planet("Venus",   orbit_x=2.2,  orbit_y=2.2,  radius=0.0531, r=0.0,    g=1,      b=0)
earth   = Planet("Earth",   orbit_x=2.5,  orbit_y=2.5,  radius=0.0555, r=0.0,    g=0,      b=1, moons=earth_moon)
mars    = Planet("Mars",    orbit_x=3.4,  orbit_y=3.4,  radius=0.0333, r=0.5,    g=0,      b=0)
jupiter = Planet("Jupiter", orbit_x=6.8,  orbit_y=6.8,  radius=0.6111, r=0.6030, g=0.490,  b=0.373, moons=jupiter_moons)
saturn  = Planet("Saturn",  orbit_x=13.6, orbit_y=13.6, radius=0.4888, r=0.9290, g=0.800,  b=0.6588, moons=saturn_moons, rings=saturn_rings)
uranus  = Planet("Uranus",  orbit_x=27.2, orbit_y=27.2, radius=0.2052, r=0.5882, g=0.7215, b=0.7647, moons=uranus_moons)
neptune = Planet("Neptune", orbit_x=40,   orbit_y=40,   radius=0.1887, r=0.2352, g=0.4627, b=0.6588, moons=neptune_moons)
pluto   = Planet("Pluto",   orbit_x=53.6, orbit_y=53.6, radius=0.01,   r=0.5607, g=0.5294, b=0.5058)

planets = [ mercury, venus, earth, mars, jupiter, saturn, uranus, neptune, pluto ]

print("<scene>")
print("    <!--Sun-->")
print("    <group R=\"1\" G=\"1\" B=\"0\" A=\"1\">")
print("        <scale X=\"20\" Y=\"20\" Z=\"20\" />")
print("        <models>")
print("            <model file=\"scenes/sphere.3d\" />")
print("        </models>")
for planet in planets:
    planet.print_planet()
print("    </group>")
print("</scene>")
