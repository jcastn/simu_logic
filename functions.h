//functions.h
#pragma once
#include "structures.h"

//Prototypes
Component* create_component(int id, TypeComponent type, int in_nbr);
Link* connect_components(Component* src, Component* dest, int port_number);
bool generic_eval(Component* comp);
bool op_and(bool a, bool b);
bool op_or(bool a, bool b);
bool op_xor(bool a, bool b);