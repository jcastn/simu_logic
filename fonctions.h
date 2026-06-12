#pragma once 

bool	gate_and(bool in_1, bool in_2)
{
	if (in_1 && in_2)
		return 1;
	else 
		return 0;
}

bool	gate_or(bool in_1, bool in_2)
{
	if (in_1 || in_2)
		return 1;
	else 
		return 0;
}

bool	gate_not(bool in)
{
	return !in;
}

bool	link(bool out)
{
	return out;
}

bool	source(bool status)
{
	return status;
}

void	diode(bool in)
{
	printf("%d\n", in);
}