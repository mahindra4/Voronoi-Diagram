import json
import matplotlib.pyplot as plt

with open("voronoi_cells.json","r") as file:
    data = json.load(file)

x_min = data["boundary_box"]["x_min"]
x_max = data["boundary_box"]["x_max"]
y_min = data["boundary_box"]["y_min"]
y_max = data["boundary_box"]["y_max"]
plt.xlim(x_min,x_max)
plt.ylim(y_min,y_max)
for cell in data["cells"]:
    x = []
    y = []
    for point in cell["region"]:
        x.append(point["x"])
        y.append(point["y"])
    x.append(x[0])
    y.append(y[0])
    plt.fill(x,y,alpha = 0.7)
    plt.plot(x,y,color = 'black')
    plt.scatter(cell["point"]["x"],cell["point"]["y"],color = "black")
plt.show()
