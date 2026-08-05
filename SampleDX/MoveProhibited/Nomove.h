#pragma once


class Nomove
{
public:

	/// <summary>
	/// コピームーブ禁止
	/// </summary>
	/// <param name=""></param>
	Nomove(const Nomove&) = delete;
	Nomove& operator=(const Nomove&) = delete;
	Nomove(Nomove&&) = delete;
	Nomove& operator=(Nomove&&) = delete;

protected:
	/// <summary>
    /// コンストラクタ
    /// </summary>
	Nomove() = default;
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~Nomove() = default;

};