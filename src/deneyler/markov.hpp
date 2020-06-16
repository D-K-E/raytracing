#ifndef MARKOV_HPP
#define MARKOV_HPP
#include <algorithm>
#include <custom/haftasonu/commons.hpp>
#include <stdexcept>
#include <vector>

struct State;
using row = std::vector<float>;
using srow = std::vector<State>;
using mat = std::vector<row>;
using smat = std::vector<srow>;

inline float random_double(float min, float max) {
  // random double number in range [min, max]
  static std::uniform_real_distribution<float> distr(min, max);
  static thread_local std::mt19937 gen;
  static std::function<float()> rand_gen = std::bind(distr, gen);
  return rand_gen();
}

inline float random_double() { return random_double(0, 1); }
inline int random_int() { return static_cast<int>(random_double()); }
inline int random_int(int min, int max) {
  return static_cast<int>(random_double(min, max));
}
vec3 random_vec() {
  // random vector
  return vec3(random_double(), random_double(), random_double());
}
vec3 random_vec(float mi, float ma) {
  // random vector in given seed
  return vec3(random_double(mi, ma), random_double(mi, ma),
              random_double(mi, ma));
}
vec3 random_in_unit_sphere() {
  // random in unit sphere
  while (true) {
    //
    vec3 v = random_vec(-1, 1);
    if (dot(v, v) >= 1) {
      continue;
    }
    return v;
  }
}
vec3 random_unit_vector() {
  // unit vector
  float a = random_double(0, 2 * PI);
  float z = random_double(-1, 1);
  float r = sqrt(1 - z * z);
  return vec3(r * cos(a), r * sin(a), z);
}
vec3 random_in_hemisphere(vec3 normal) {
  // normal ekseninde dagilan yon
  vec3 unit_sphere_dir = random_in_unit_sphere();
  if (dot(unit_sphere_dir, normal) > 0.0) {
    return unit_sphere_dir;
  } else {
    return -1.0f * unit_sphere_dir;
  }
}
vec3 random_in_unit_disk() {
  // lens yakinsamasi için gerekli
  while (true) {
    vec3 point = vec3(random_double(-1, 1), random_double(-1, 1), 0);
    if (dot(point, point) >= 1) {
      continue;
    }
    return point;
  }
}

row random_row(unsigned int nb, float mi = 0.0, float ma = 1.0,
               float sumVal = 1.0) {
  // given an array size generate a vector of given size whose sum is equal
  // to sumVal
  std::vector<float> res(nb);
  float s = 0;
  for (int i = 0; i < nb; i++) {
    res[i] = random_double(mi, ma);
    s += res[i];
  }
  for (int i = 0; i < nb; i++) {
    res[i] = sumVal / s;
  }
  return res;
}
mat random_mat(unsigned int nb, float mi = 0.0, float ma = 1.0,
               float sumVal = 1.0) {
  // generate nxn matrix whose row sum up to sumVal
  mat resmat(nb, row(nb));
  for (int i = 0; i < nb; i++) {
    resmat[i] = random_row(nb, mi, ma, sumVal);
  }
  return resmat;
}

template <typename T> T random_pick(std::vector<T> r) {
  // randomly pick an element
  int index = random_int(0, r.size());
  return r[index];
}
template <typename T> T random_pick(std::vector<std::vector<T>> m) {
  // randomly pick an element from matrix of size nxn
  int index1 = random_int(0, m.size());
  int index2 = random_int(0, m.size());
  return m[index1][index2];
}
template <typename T> T random_pick(std::vector<std::vector<T>> m, int index1) {
  // randomly pick an element from matrix of size nxn
  int index2 = random_int(0, m.size());
  return m[index1][index2];
}
int get_weighted_index(row probs) {
  // adapted from https://stackoverflow.com/a/4437287/7330813
  float thresh = random_double(0, 1);
  int index = 0;
  while (thresh >= 0 and index < probs.size()) {
    thresh -= probs[index];
    index++;
  }
  return index;
}
template <typename T> T random_pick(std::vector<T> r, row probs) {
  // pick and element from array using the given distribution
  //
  if (r.size() != probs.size()) {
    throw std::invalid_argument("probability row should have same size of" +
                                "pick vector")
  }
  int index = get_weighted_index(probs);
  return r[index];
}
template <class T> bool is_unique(vector<T> X) {
  // from https://stackoverflow.com/q/2769174/7330813
  set<T> Y(X.begin(), X.end());
  return X.size() == Y.size();
}

class MarkovChain {
public:
  mat probs;
  row<State> states;
  MarkovChain(){};
  MarkovChain(std::vector<State> vs, float mi = 0.0, float ma = 1.0)
      : states(vs) {
    if (is_unique(vs) == false) {
      throw std::invalid_argument("Vector must contain unique states");
    }
    probs = random_mat(vs.size(), mi, ma);
  }
  MarkovChain() MarkovChain(mat ps, std::vector<State> vs) : {
    if (vs.size() != ps.size()) {
      throw std::invalid_argument("Number of states and number of rows in " +
                                  "transition matrix must match");
    }
    if (ps.size() != ps[0].size()) {
      throw std::invalid_argument("Number of rows and number of columns in " +
                                  "transition matrix must match");
    }
    if (is_unique(vs) == false) {
      throw std::invalid_argument("Vector must contain unique states");
    }

    probs = ps;
    states = vs;
  }
  int state_index(State s) {
    // find state index
    std::vector<State>::iterator it =
        std::find(states.begin(), states.end(), current);
    if (it != states.end()) {
      throw std::invalid_argument("State not in states");
    }
    return std::distance(states.begin(), it);
  }
  std::vector<int> state_indices(std::vector<State> ss) {
    std::vector<int> indices;
    for (auto const &s : ss) {
      indices.push_back(state_index(s));
    }
    return indices;
  }
  State next_state(State current) {
    // get next state from the current one using probability matrix
    int index = state_index(current);
    row state_prob = probs[index];
    return random_pick(states, state_prob);
  }
  std::vector<State> generate_states(State s, int nbState) {
    // generate nbState amount of states
    std::vector<State> generated;
    for (int i; i < nbState; i++) {
      State nstate = next_state(s);
      generated.push_back(nstate);
      s = nstate;
    }
    return generated;
  }
  std::vector<State> generate_states(State start, State end) {
    std::vector<State> path;
    bool isArrived = false;
    while (isArrived == false) {
      State nstate = next_state(s);
      path.push_back(nstate);
      if (nstate == end) {
        isArrived = true;
      }
    }
    return path;
  }
  std::vector<int> path_indices(State start, State end) {
    return state_indices(generate_states(start, end));
  }
  float path_prob(State start, State end) {
    // probability of path between start state and the end state
    // basically p_{ij} = \sum_{k=0}^{r} p_{ik} p_{kj} where r is the number
    // of states
    int start_index = state_index(start);
    int end_index = state_index(end);
    float s;
    for (int k = 0; k < states.size(); k++) {
      float p_ik = probs[start_index][k];
      float p_kj = probs[k][end_index];
      s += p_ik * p_kj;
    }
    return s;
  }
};

#endif
