

#include "Integer.h"


template<> std::string nnet::Char::type()   { return "Char"; }
template<> std::string nnet::Uchar::type()  { return "Uchar"; }
template<> std::string nnet::Short::type()  { return "Short"; }
template<> std::string nnet::Ushort::type() { return "Ushort"; }
template<> std::string nnet::Int::type()    { return "Int"; }
template<> std::string nnet::Uint::type()   { return "Uint"; }
template<> std::string nnet::Long::type()   { return "Long"; }
template<> std::string nnet::Ulong::type()  { return "Ulong"; }

