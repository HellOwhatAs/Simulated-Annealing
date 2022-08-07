from SA import SA
from math import cos,pi

def func(x:list):
    return 20+x[0]**2+x[1]**2-10*(cos(2*pi*x[1])+cos(2*pi*x[0]))

sa=SA(
    func=func,
    x0=[5,5],
    lb=[-5,-5],
    ub=[5,5]
)
best_x=sa.run()
best_y=func(best_x)

print("best_x = {};\nbest_y = {};".format(best_x,best_y))
