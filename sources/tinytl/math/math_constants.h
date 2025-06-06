#ifndef MATHCONSTANT_H
#define MATHCONSTANT_H

template<typename T> // variable templates
constexpr T PI = T(3.1415926535897932384626433832795L);

template<typename T, T N>
constexpr T intergral_constant = N;

template<bool v>
using bool_constant = intergral_constant<bool,v>;

template<typename T>
constexpr bool is_integral = false;
 
template<> //specialized 
constexpr bool is_integral<short> = true;
 
template<>
constexpr bool is_integral<int> = true;
 
template<>
constexpr bool is_integral<long> = true;

#endif