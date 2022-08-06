#include <pybind11/pybind11.h>
#include <pybind11/stl.h>
#include <random>
namespace py = pybind11;
using namespace std;
default_random_engine randg(time(0));
vector<double> TSP_new_x(const vector<double>& _x, double T) {
	vector<double> x = _x;
	int new_x_strategy = uniform_int_distribution<int>(0, 2)(randg);
	//cout << new_x_strategy << '\n';
	switch (new_x_strategy) {
	case 0: {
		uniform_int_distribution<int> rtmp(0, x.size() - 2);
		int n1 = rtmp(randg), n2 = rtmp(randg);
		if (n1 >= n2) {
			swap(n1, n2); ++n2;
		}
		swap(x[n1], x[n2]);
	}break;
	case 1: {
		uniform_int_distribution<int> rtmp(0, x.size() - 2);
		int n1 = rtmp(randg), n2 = rtmp(randg);
		if (n1 >= n2) {
			swap(n1, n2); ++n2;
		}
		reverse(&x[n1], &x[n2]);
	}break;
	case 2: {
		uniform_int_distribution<int> rtmp(0, x.size() - 3);
		vector<int> tmp = { rtmp(randg) ,rtmp(randg) ,rtmp(randg) };
		vector<double> ret;
		sort(tmp.begin(), tmp.end());
		tmp[1]++; tmp[2] += 3;
		for (auto i = &x[0]; i < &x[tmp[0]]; ++i)ret.push_back(*i);
		for (auto i = &x[tmp[0]]; i < &x[tmp[1]]; ++i)ret.push_back(*i);
		for (auto i = &x[tmp[1]]; i < &x[tmp[2]]; ++i)ret.push_back(*i);
		for (auto i = &x[tmp[2]]; i < &x[x.size()]; ++i)ret.push_back(*i);
		//x = ret;
		ret.swap(x);
	}break;
	}
	return x;
}
class _SA {
private:
	py::function _func,_udf_new_x;
	bool _has_bounds = 0,_udfx=0;
	vector<double> _x0, __x0, _lb, _ub;
	double _T_max, _T_min, _T, _cool_down;
	int _L, _max_stay_counter, _n_dim, _hop = 10;
public:
	_SA(py::function func, const vector<double>& x0, double T_max = 100, double T_min = 1e-7,double cool_down=0.9, int L = 300, int max_stay_counter = 150, const vector<double>& lb = {}, const vector<double>& ub = {}, py::object udf_new_x=py::none()) {
		_func = func;
		if (!udf_new_x.is(py::none())) {
			_udfx = 1;
			_udf_new_x = udf_new_x;
		}
		__x0 = _x0 = x0;
		if (lb.size()!=0 && ub.size()!=0) {
			if (lb.size() == x0.size())_lb = lb;
			else _lb.resize(x0.size(), lb[0]);
			if (ub.size() == x0.size())_ub = ub;
			else _ub.resize(x0.size(), ub[0]);
			_has_bounds = 1;
		}
		_n_dim = x0.size();
		_T_max = T_max;
		_T = _T_max;
		_T_min = T_min;
		_cool_down = cool_down;
		_L = L;
		_max_stay_counter = max_stay_counter;
	}
	vector<double> get_new_x(const vector<double>&x) {
		uniform_real_distribution<double> randu(-1, 1);
		vector<double> u;
		u.resize(_n_dim, 0.);
		for (int i = 0; i < x.size();++i) {
			double r = randu(randg);
			u[i] = x[i] + ((r > 0 ? 1 : (r == 0 ? 0 : -1)) * _T * (pow((1 + 1.0 / _T), (r < 0 ? -r : r)) - 1.0)) * _hop;
			if (_has_bounds) {
				u[i] = max(u[i], _lb[i]);
				u[i] = min(u[i], _ub[i]);
			}
		}
		return u;
	}
	vector<double> run() {
		uniform_real_distribution<double> randu(0, 1);
		double now_y = _func(_x0).cast<double>();
		auto t_best_x = _x0;
		auto t_best_y = now_y;
		int tmp_count = 0;
		while (_T > _T_min) {
			bool changed = 0;
			for (int i = 0; i < _L; ++i) {
				auto new_x = (_udfx)?_udf_new_x(_x0,_T).cast<vector<double>>():get_new_x(_x0);
				double new_y = _func(new_x).cast<double>();
				if (new_y < t_best_y) {
					t_best_x = new_x;
					t_best_y = new_y;
					changed = 1;
				}
				if (new_y < now_y) {
					_x0 = new_x;
					now_y = new_y;
				}
				else {
					if (randu(randg) < exp((now_y - new_y) / _T)) {
						_x0 = new_x;
						now_y = new_y;
					}
				}
			}
			_T *= _cool_down;
			_x0 = t_best_x;
			now_y = t_best_y;
			if (!changed) {
				++tmp_count;
				if (tmp_count > _max_stay_counter)break;
			}
			else tmp_count = 0;
		}
		return _x0;
	}
};
PYBIND11_MODULE(SA, m) {
	py::class_<_SA>(m, "SA")
		.def(py::init<
			py::function,
			const vector<double>&,
			double,
			double,
			double,
			int,
			int,
			const vector<double>&,
			const vector<double>&,
			py::object
		>(),
			py::arg("func"),
			py::arg("x0"),
			py::arg("T_max") = 100,
			py::arg("T_min") = 1e-7,
			py::arg("cool_down") = 0.9,
			py::arg("L") = 300,
			py::arg("max_stay_counter") = 150,
			py::arg("lb") = vector<double>(),
			py::arg("ub") = vector<double>(),
			py::arg("udf_new_x").none(1) = nullptr
		)
		.def("run", &_SA::run);
	m.def("TSP_new_x", &TSP_new_x,py::arg("x"),py::arg("T"));
}