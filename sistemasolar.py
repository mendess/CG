import random
from math import sin, cos, pi

class Planet:
    def __init__(self, name, distance, radius, r, g, b, moons=[], rings=[]):
        self.name = name
        self.distance = distance
        self.radius = radius
        self.r = r
        self.g = g
        self.b = b
        self.moons = moons
        self.rings = rings

    def print_planet(self, indent=8):
        a = random.uniform(-pi / 4, pi / 4)
        x = self.distance * sin(a)
        z = self.distance * cos(a)
        print(" " * indent, "<!-- {} -->".format(self.name))
        print(" " * indent, "<group R=\"{}\" G=\"{}\" B=\"{}\" >".format(self.r, self.g, self.b))
        print(" " * indent, "    <translate X=\"{}\" Z=\"{}\" /> <!-- alpha: {} | radius: {} -->".format(x, z, a, self.distance))
        print(" " * indent, "    <scale X=\"{0}\" Y=\"{0}\" Z=\"{0}\" />".format(self.radius))
        print(" " * indent, "    <models>")
        print(" " * indent, "        <model file=\"sphere.3d\"/>")
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
        print(" " * indent, "        <model file=\"sphere.3d\"/>")
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
        print(" " * indent, "        <model file=\"torus_1.3d\" />")
        print(" " * indent, "    </models>")
        print(" " * indent, "</group>")

mercury = Planet("Mercury", 1.8, 0.0216, 1, 0, 0)
venus = Planet("Venus", 2.2, 0.05310, 0, 1, 0)
earth = Planet("Earth", 2.5, 0.0555, 0, 0, 1, [Planet("Moon", 2.0514, 0.25, 0.8, 0.8, 0.8)])
mars = Planet("Mars", 3.4, 0.0333, 0.5, 0, 0)
jupiter_moons = [
        Planet("Europa", 2.0896, 0.0222, 1, 0, 0),
        Planet("Io", 2.0468, 0.03181, 0, 0.8, 0),
        Planet("Ganymede", 2.1432, 0.03704, 0, 0, 0.8),
        Planet("Callisto", 2.2514, 0.03448, 0.5, 0.5, 0)
        ]
jupiter = Planet("Jupiter", 6.8, 0.6111, 0.603, 0.490, 0.373, jupiter_moons)
saturn_rings = [Ring(0.803, 0.702, 0.503, 1, 0, 0.5, 90, 1.5, 1.5, 0)]
saturn_moons = [Planet("Titan", 2.1604, 0.04425, 0.8, 0.8, 0.8)]
saturn = Planet("Saturn", 13.6, 0.4888, 0.9294, 0.8, 0.6588, saturn_moons, saturn_rings)
uranus_moons = [
        Planet("Miranda" , 1.035120, 0.0037728, 0.8, 0.8, 0.8),
        Planet("Ariel" , 1.528160, 0.0092624, 0.8, 0.8, 0.8),
        Planet("Umbriel" , 2.130400, 0.0093552, 0.8, 0.8, 0.8),
        Planet("Titania" , 3.487280, 0.0126144, 0.8, 0.8, 0.8),
        Planet("Oberon" , 4.668160, 0.0121824, 0.8, 0.8, 0.8)
        ]
uranus = Planet("Uranus", 27.2, 0.2052, 0.5882, 0.7215, 0.7647, uranus_moons)
neptune_moons = [
        Planet("Triton", 16.27, 0.0000205, 0.8, 0.8, 0.8)
        ]
neptune = Planet("Neptune", 40, 0.1887, 0.2352, 0.4627, 0.6588, neptune_moons)
pluto = Planet("Pluto", 53.6, 0.01, 0.5607, 0.5294, 0.5058)

planets = [ mercury, venus, earth, mars, jupiter, saturn, uranus, neptune, pluto ]

print("<scene>")
print("    <!--Sun-->")
print("    <group R=\"1\" G=\"1\" B=\"0\" A=\"1\">")
print("        <scale X=\"20\" Y=\"20\" Z=\"20\" />")
print("        <models>")
print("            <model file=\"sphere.3d\" />")
print("        </models>")
for planet in planets:
    planet.print_planet()
print("    </group>")
print("</scene>")
