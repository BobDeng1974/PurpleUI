#pragma once
#include "MenuUI.h"
class TestMenu :public MenuUI
{
public:
	void OnCreate(const ViewRect &view)override
	{
		MenuUI::OnCreate(view);
		CreateMenu({ 
			{ "��һ���˵�",[](void *) {},nullptr },
			{ "�ڶ����˵�",[](void *) {},nullptr }
			}, 40, 5, 1, Center);
	}
};