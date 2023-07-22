#pragma once

extern "C" bool __stdcall collisionAsm(double, double, double, double, double, double, double&, double&, size_t);

extern "C" double __stdcall collision_reaction_backAsm(double, double, double, double, size_t);

extern "C" char __stdcall paint_valAsm(double, double, double, double);