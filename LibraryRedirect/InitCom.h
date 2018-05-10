#pragma once

class InitCom
{
public:
	InitCom();
	InitCom(InitCom&) = delete;

	~InitCom() noexcept;
};
