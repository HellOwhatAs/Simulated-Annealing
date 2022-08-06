from typing import List, Callable


def TSP_new_x(x: List[float], T: float) -> List[float]:
    ...


class SA:

    def __init__(self,
                 func,
                 x0,
                 T_max=100,
                 T_min=1e-7,
                 cool_down=0.9,
                 L=300,
                 max_stay_counter=150,
                 lb=List[float],
                 ub=List[float],
                 udf_new_x: function = None):
        """func: the function to minimize.\n
        x0: initial independent variable.\n
        T_max = 100 : initial temperature.\n
        T_min = 1e-7 : end temperature.\n
        cool_down = 0.9 : the constant multiplied to T.\n
        L =300 : num of iteration under every temperature.\n
        max_stay_counter = 150 : stop if best_y stay unchanged over max_stay_counter times.\n
        lb = [] : lower-bound (default is none).\n
        ub = [] : upper-bound (default is none).\n
        udf_new_x = None : user defined `func(x:List[float],T:float)->float` to generate new x."""

    def run() -> List[float]:
        """run optimize."""