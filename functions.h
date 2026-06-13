//functions.h
#pragma once
#include "structures.h"

Component* create_component(int id, TypeComponent type);
Link* connect_components(Component* src, Component* dest, int port_number);
