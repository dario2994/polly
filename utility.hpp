#ifndef UTILITY_HPP
#define UTILITY_HPP

#include <vector>
#include <map>
#include <algorithm>
#include <iostream>
#include <sstream>
#include <cassert>
using namespace std;

#ifdef POLLY_DEBUG
	#define MP_WRITE(str) do { std::clog << str << std::endl; } while( false )
#else
	#define MP_WRITE(str) do { } while ( false )
#endif

bool SignReadStream(istream &in, long long int &sign) {
	char c;
	if( !in.get(c) ) {
		return false;
	}
	else if ( c == '-' ) {
		sign = -1;
		return true;
	}
	else if ( c == '+' ) {
		sign = 1;
		return true;
	}
	
	in.unget();
	return false;
}

unsigned long long int IntegerGcdComputation(unsigned long long int x, 
																						 unsigned long long int y) {
	if ( y == 0 ) return x;
	return IntegerGcdComputation(y, x % y);
}

long long int IntegerGcd(long long int x, long long int y) {
	if ( x < 0 ) x = -x;
	if ( y < 0 ) y = -y;
	if ( x < y ) swap(x, y);
	return IntegerGcdComputation(x, y);
}

#endif // UTILITY_HPP
