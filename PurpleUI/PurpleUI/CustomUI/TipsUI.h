#ifndef TIPSUI_H
#define TIPSUI_H

#include <queue>
#include <vector>
#include <string>
#include "SurfaceSystem/SurfaceManager.h"
#include "TimeSystem/Timer.h"

/*�����½���ʾ����ʾ��UI*/
class TipsUI :public UIComponent
{	
	static TipsUI *activeTips;//ͬһʱ��ֻ����һ��tips
public:

	static const float LONG_LENGTH;
	static const float SHORT_LENGTH;

	/*������ʾ*/
	void CreateTips(const std::string &title, const std::vector<std::string> &content);

	/*������ʾ����������ʹ�����еľ�ֵ̬*/
	inline void Show(float showTime);

protected:

	/*������Ҫ�ȵ��ø÷���*/
	virtual void OnCreate(const ViewRect &srcView)override;

	inline virtual void OnDisable()override;

	inline virtual void OnUpdate()override;

private:
	ViewRect m_srcView, m_cutView;
	float m_passTime;
	const float m_fadeTime = 1.0f;
	float m_stayTime;
	std::vector<std::string> m_datas;
	Timer *m_updateTimer;
	Timer m_beginTimer;
	Timer m_stayTimer;
	Timer m_fadeTimer;
	std::function<void()> *Update;
	std::function<void()> Begin;
	std::function<void()> Stay;
	std::function<void()> Fade;

	void Reset();

	void Draw(float percent);
};
void TipsUI::Show(float showTime)
{
	if (activeTips != nullptr)
		activeTips->Disable();
	activeTips = this;
	m_stayTime = showTime;
	m_stayTimer.SetUpadateTime(showTime);
	Reset();
	Enable();
}
void TipsUI::OnDisable()
{
	GetContext()->Draw();
}
void TipsUI::OnUpdate()
{
	(*Update)();
	m_updateTimer->Update();
	m_passTime += Time::GetDeltaTime();
}
#endif