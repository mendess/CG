import sys

class Patch:
    def __init__(self, points):
        self.points = points


i = 0
f = open(sys.argv[1], "r")
f.readline()
f.readline()
f.readline()
f.readline()
points = {}
patches = []

temp_points = []
for p in f:
    if p not in points:
        points[p] = i
        i += 1
    temp_points.append(p)
    if len(temp_points) == 16:
        patches.append(Patch(temp_points))
        temp_points = []

patches_idx = []
for patch in patches:
    idxs = []
    for p in patch.points:
        idxs.append(points[p])
    a = idxs[0:4]
    b = idxs[4:8]
    c = idxs[8:12]
    d = idxs[12:16]
    l = [a[::-1], b[::-1], c[::-1], d[::-1]]
    patches_idx.append([item for sublist in l for item in sublist])


sorted_points = [(v, k) for k, v in points.items()]

out = open("out_" + sys.argv[1], "w")
out.write(str(len(patches_idx)) + "\n")
for i in patches_idx:
    out.write(",".join(list(map(lambda x: str(x), i))) + "\n")

out.write(str(len(sorted_points)) + "\n")

for p in map(lambda x: x[1], sorted(sorted_points)):
    out.write(" " + p)

