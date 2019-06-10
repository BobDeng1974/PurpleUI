#ifndef MENUUI_H
#define MENUUI_H

#include <vector>
#include <functional>
#include "SurfaceSystem/SurfaceManager.h"
#include "TimeSystem/Timer.h"

class MenuUI :public UIComponent
{
protected:
	struct MenuItem
	{
		std::string name;
		std::function<void(void *)> Event;
		void *data;
	};

	enum GravityMode
	{
		Left,
		Center
	};
	
	void OnCreate(const ViewRect &view)override;

	/*����һ���˵���ViewRect�����menuItemName����󳤶Ⱦ���
	@menuItems:�˵�����
	@x,y:�˵����Ͻǵĳ�ʼλ��
	@lineOffset:�˵�ÿ��֮��ľ���
	@pageCount:һҳ�����ʾ����Ԫ��[>0]
	@mode:�˵�Ԫ�صĶ����ʽ
	@drawBottomTips:�Ƿ������·����Ʋ˵���ʾ��,��׷��һ��*/
	void CreateMenu(const std::vector<MenuItem> &menuItems,
		int x, int y, int lineOffset, int pageCount, GravityMode mode, bool hasBottomTips);

	/*����һ���˵���ViewRect�����menuItemName����󳤶Ⱦ���
	@menuItems:�˵�����
	@x,y:�˵����Ͻǵĳ�ʼλ��
	@lineOffset:�˵�ÿ��֮��ľ���
	@mode:�˵�Ԫ�صĶ����ʽ*/
	void CreateMenu(const std::vector<MenuItem> &menuItems,
		int x, int y, int lineOffset, GravityMode mode);

	void UpChoose();

	void DownChoose();

	void UpPage();

	void DownPage();

	inline virtual void OnDraw()override;
	
	inline virtual void OnUpdate()override;

	inline virtual void OnLoseFocus()override;

	inline virtual void OnGetFocus()override;

	inline virtual void OnReset()override;

	inline virtual void OnPause()override;

	inline virtual void OnResume()override;

	inline virtual void OnEnable()override;

	inline virtual void OnDisable()override;

	virtual bool OnEvent(char input)override;
	
private:

	ViewRect m_srcView, m_menuView;
	std::vector<MenuItem> m_menuItems;
	int m_lineOffset;//ÿ��ռ���
	int m_pageLine;//һҳ������
	int m_pageCount;//һҳ���ٸ�
	int m_pageNum;//����ҳ
	int m_choose = 0;
	Timer m_animationTimer;
	char m_animationStep = 0;
	GravityMode m_mode;
	bool m_hasBottomTips;
	bool m_isLoseFocus = false;

	inline void DrawChooseItem(int pos, int step);

	inline void DrawNormalItem(int pos);

	/*ˢ��pos����ҳ*/
	inline void DrawPage(int pos);

	inline void DrawTips();
	
	//��Ԫ������ҳ��Ԫ�ص������ڸ�ҳ����һ��
	inline int GetLineInPage(int pos);

	//��Ԫ������ҳ��һ��Ԫ�ص�����
	inline int GetFirstInPage(int pos);

	//��Ԫ������ҳ���һ��Ԫ�ص�����
	inline int GetLastInPage(int pos);
};
void MenuUI::OnDraw()
{
	if (!m_menuItems.empty())
		DrawPage(m_choose);
}
void MenuUI::OnUpdate()
{
	if (IsFocus() && !m_menuItems.empty())
	{
		m_animationTimer.Update();
		DrawChooseItem(m_choose, m_animationStep);
		if (m_hasBottomTips)
			DrawTips();
	}
}
void MenuUI::OnLoseFocus()
{
	m_animationTimer.Pause();
}
void MenuUI::OnGetFocus()
{
	m_animationTimer.Resume();
}
void MenuUI::OnReset()
{
	m_animationStep = 0;
	m_animationTimer.Run();
}
void MenuUI::OnPause()
{
	m_animationTimer.Pause();
}
void MenuUI::OnResume()
{
	m_animationTimer.Resume();
}
void MenuUI::OnEnable()
{
	m_animationTimer.Resume();
}
void MenuUI::OnDisable()
{
	m_animationTimer.Pause();
}
void MenuUI::DrawChooseItem(int pos, int step)
{
	int line = GetLineInPage(pos);
	m_menuView.FillLine(0, line, ' ');
	if (m_menuItems.empty())
		return;
	switch (m_mode)
	{
	case MenuUI::Left:
		m_menuView.SetText(0, line, ("->" + m_menuItems[pos].name + std::string(step, '.')));
		break;
	case MenuUI::Center:
		std::string space(3 - step, ' ');
		std::string chooseText = "->" + space + m_menuItems[pos].name + space + "<-";
		m_menuView.SetText(GetTextCenterLayoutBeginPos(chooseText, m_menuView.GetCutRect().w), line, chooseText);
		break;
	}
}
void MenuUI::DrawNormalItem(int pos)
{
	int line = GetLineInPage(pos);
	m_menuView.FillLine(0, line, ' ');
	if (m_menuItems.empty())
		return;
	switch (m_mode)
	{
	case MenuUI::Left:
		m_menuView.SetText(0, line, m_menuItems[pos].name);
		break;
	case MenuUI::Center:
		m_menuView.SetText(GetTextCenterLayoutBeginPos(m_menuItems[pos].name, m_menuView.GetCutRect().w), line, m_menuItems[pos].name);
		break;
	}
}
void MenuUI::DrawPage(int pos)
{
	m_menuView.ResetCutView();
	int pagePos = GetFirstInPage(pos);
	int pageLast = GetLastInPage(pos);
	for (int line; pagePos <= pageLast; ++pagePos)
	{
		line = GetLineInPage(pagePos);
		if (pagePos == pos)
			DrawChooseItem(pagePos, m_animationStep);
		else
			DrawNormalItem(pagePos);
	}
	if (m_hasBottomTips)
		DrawTips();
}
void MenuUI::DrawTips()
{
	char tips[32];
	sprintf(tips, "��%d/%d�� ��%d/%dҳ", m_choose + 1, m_menuItems.size(), m_choose / m_pageCount + 1, m_pageNum);
	m_menuView.SetText(GetTextRightLayoutBeginPos(tips, m_menuView.GetCutRect().w), m_pageLine, tips);
}
int MenuUI::GetLineInPage(int pos)
{
	return (pos % m_pageCount) * (m_lineOffset + 1);
}
int MenuUI::GetFirstInPage(int pos)
{
	return (pos / m_pageCount)*m_pageCount;
}
int MenuUI::GetLastInPage(int pos)
{
	if (pos / m_pageCount + 1 == m_pageNum)
		return m_menuItems.size() - 1;
	return (pos / m_pageCount + 1)*m_pageCount - 1;
}
#endif