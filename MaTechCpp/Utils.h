#pragma once
#include "stdafx.h"

using namespace std;
using namespace sf;

#define SafeDelete(x) { if(x != nullptr) { delete x; x = nullptr; } }
#define SafeDeleteArr(x) { if(x != nullptr) { delete[] x; x = nullptr; } }

#define SafeDeletePtr(x) { (x).reset(); }

namespace MTUtil
{
	
}