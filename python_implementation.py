import numpy as np
import matplotlib as mpl
import matplotlib.path as mplPath
mpl.use('tkagg')
import matplotlib.pyplot as plt
import math 

vertices = [[5,5],[6,6],[8,8],[9,6],[10,7],[12,5],[10,5],[9,4],[6,3],[5,5]]
xs = []
ys = []
xmin = math.inf 
ymin = math.inf 
xmax = 0
ymax=  0

for v in vertices:
    if v[0] < xmin:
        xmin = v[0]
    if v[0] > xmax:
        xmax = v[0]
    if v[1] < ymin:
        ymin = v[1]
    if v[1] > ymax:
        ymax = v[1]

    xs.append(v[0])
    ys.append(v[1])

rect = [[xmin,ymin],[xmin,ymax],[xmax,ymax],[xmax,ymin],[xmin,ymin]]

rectxs = []
rectys = []
for r in rect:
    rectxs.append(r[0])
    rectys.append(r[1])

divider_n = 50

divide_xs = list(np.arange(xmin,xmax+0.5,(xmax-xmin)/divider_n))

divide_ys = list(np.arange(ymin,ymax+0.5,(ymax-ymin)/divider_n))

all_points = []

for y in divide_ys:
    all_points.append(list(zip(divide_xs,[y]*divider_n)))

print("all points: ", all_points)

print("divided xs: ", divide_xs)
print("divided ys: ", divide_ys)

poly_path = mplPath.Path(np.array(vertices))

#print(p, " is in polygon: ", poly_path.contains_point(p))

plt.plot(xs,ys)
plt.plot(rectxs,rectys,color='red')

for lst in all_points:
    for p in lst:
        if poly_path.contains_point(p):
            plt.scatter(p[0],p[1],color='green')
        else:
            plt.scatter(p[0],p[1],color='red')

#plt.scatter(xmin,ymin,color='red')
#plt.scatter(xmax,ymin,color='red')
#plt.scatter(xmin,ymax,color='red')
#plt.scatter(xmax,ymax,color='red')
plt.savefig("test.png")

plt.show()