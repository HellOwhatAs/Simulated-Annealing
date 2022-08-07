from SA import SA, TSP_new_x
import numpy as np
from scipy import spatial
import matplotlib.pyplot as plt

num_points = 50

points_coordinate = np.random.rand(num_points, 2)
distance_matrix = spatial.distance.cdist(points_coordinate, points_coordinate, metric='euclidean')

def cal_total_distance(routine):
    num_points=len(routine)
    return sum([distance_matrix[int(routine[i % num_points]), int(routine[(i + 1) % num_points])] for i in range(num_points)])

sa_tsp=SA(
    func=cal_total_distance,
    x0=list(range(num_points)),
    udf_new_x=TSP_new_x
)

best_points=[int(i) for i in sa_tsp.run()]

best_points_ = np.concatenate([best_points, [best_points[0]]])
best_points_coordinate = points_coordinate[best_points_, :]
plt.plot(best_points_coordinate[:, 0], best_points_coordinate[:, 1], 'o-r')
plt.show()
