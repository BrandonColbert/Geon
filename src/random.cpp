#include "utils/random.h"

#define specialize(type, group) template<> type Sequence::next<type>(type min, type max) { return next##group(min, max); }

using namespace std;
using Sequence = Random::Sequence;

Sequence::Sequence(unsigned int seed) : value(seed) {}

void Sequence::moveNext() {
	seed_seq ss{value};
	vector<unsigned int> s(1);
	ss.generate(s.begin(), s.end());
	value = s[0] & 0x7fffffff;
}

namespace Random {
	specialize(int, Integer)
	specialize(long, Integer)
	specialize(float, Rational)
	specialize(double, Rational)
}