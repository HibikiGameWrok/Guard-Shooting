#pragma once

namespace Utility
{
	// 数値制限
	template<typename T>bool Clamp(T& t, T min, T max)
	{
		// tに代入(tがminより小さければminを入れる、
		//         tがmaxより大きければmaxを入れる、
		//         それ以外ならtを入れる)
		// t = (t < min) ? min : (t > max) ? max : t;

		if (t < min)
		{
			t = min;
			return true;
		}

		if (t > max)
		{
			t = max;
			return true;
		}

		return false;
	}
}