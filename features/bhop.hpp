#pragma once
#include "../singleton.hpp"
#include "../helpers/utils.hpp"
#include "../valve_sdk/csgostructs.hpp"
#include "../options.hpp"

class bhop : public Singleton<bhop>
{
private:
	friend class Singleton<bhop>;

public:
    void on_createmove();
};

class autostrafer : public Singleton<autostrafer>
{
private:
	friend class Singleton<autostrafer>;

public:
	void on_createmove();
};