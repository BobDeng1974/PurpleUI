#ifndef UISURFACE_H
#define UISURFACE_H

#include <cstdio>
#include <cstdlib>
#include <typeinfo>
#include <vector>
#include <algorithm>
#include "UIComponent.h"

/*һ�����棬Ӧ������̳в����*/
class UISurface
{	
public:

	virtual ~UISurface();

	/*�����ﴴ�������UI���@AddUIComponent*/
	virtual void OnCreateUI() {}

	/*�ڽ����һ�δ�����ʱ�����,ֻ�����һ��,�������FocusUI�����¼�*/
	virtual void OnCreate() {}

	/*������������*/
	/*�ڽ�������ʱ�����*/
	inline virtual void OnEnter();

	/*������������*/
	/*����һ�������İ汾*/
	inline virtual void OnEnter(void *data);

	/*�ڽ���֮���л�����һ�������ʱ�����*/
	inline virtual void OnExit();

	/*�ڴ���һ�����淵�ؽ���ʱ����*/
	inline virtual void OnBack();

	/*�ڽ����˳�ʱ����*/
	virtual void OnQuit() {}

	/*������������*/
	/*�ڽ�����Ƶ�ʱ�����*/
	inline virtual void OnUpdate();

	/*������������*/
	/*�ڽ������ٵ�ʱ�����*/
	inline virtual void OnDestroy();

	/*�����¼�����*/
	inline virtual void OnEvent(char input);

	/*��ȡ��ͼ*/
	inline ViewRect *GetView();

	/*��ȡUI���*/
	template<class T>
	inline T *GetUIComponent();

	/*��ȡUI���*/
	template<class T>
	inline std::vector<T*> GetUIComponents();

	/*���ý���UI����UI����������¼���Ϣ��������Ȩ�����Ƿ��¼�����*/
	inline void SetFocusUI(UIComponent *focusUi);

	/*��ȡ����UI*/
	inline UIComponent * GetFocusUI()const;

	/*ǿ�ƽ����ػ�*/
	inline void Draw();

	/*ǿ�ƽ����ػ棬���ػ�withoutUI*/
	inline void Draw(UIComponent *withoutUI);

	/*��ȡ֡��*/
	inline float GetFps()const;

	/*����֡��*/
	inline void SetFps(float fps);

protected:

	UISurface(unsigned char fps,int w,int h)
		: m_fps(fps)
	{
		m_view.SetViewRect(w, h);
	}

	/*���UI������ͷ��йܵ���Դ*/
	void ClearUIComponents();

	/*���UI������Ƿ�����ڴ��й�[�����˳�ʱ�Զ��ͷ���Դ]*/
	void AddUIComponent(UIComponent *newUI, bool isMemoryDeposit = true);

private:
	friend class SurfaceManager;

	struct UIComponentMsg
	{
		UIComponentMsg() = default;
		UIComponentMsg(UIComponent *ui, bool isMemoryDeposited = true)
		{
			this->ui = ui;
			this->isMemoryDeposit = isMemoryDeposited;
		}

		UIComponent *ui;
		bool isMemoryDeposit;
	};

	float m_fps;//֡��
	ViewRect m_view;
	std::vector<UIComponentMsg> m_uiComs;
	UIComponent *m_focusUi = nullptr;

	/*��������UIʱ����*/
	void OnCreatedUI();
};
void UISurface::OnEnter()
{
	m_view.ResetSrcView();
	for (auto &msg : m_uiComs)
	{
		msg.ui->OnReset();
		if (msg.ui->IsEnable())
			msg.ui->OnDraw();
	}
}
void UISurface::OnEnter(void * data)
{
	OnEnter();
}
void UISurface::OnExit()
{
	for (auto &msg : m_uiComs)
		msg.ui->OnPause();
}
void UISurface::OnBack()
{
	for (auto &msg : m_uiComs)
		msg.ui->OnResume();
}
void UISurface::OnUpdate()
{
	system("cls");
	for (auto &msg : m_uiComs)
		if (msg.ui->IsEnable())
			msg.ui->OnUpdate();
	puts(m_view.GetViewData());
}
void UISurface::OnDestroy()
{
	for (auto &msg : m_uiComs)
		msg.ui->OnDestroy();
	ClearUIComponents();
}
void UISurface::OnEvent(char input)
{
	if (m_focusUi != nullptr)
		if (!m_focusUi->OnEvent(input))
			return;
	for (auto &msg : m_uiComs)
		if (msg.ui != m_focusUi && msg.ui->IsEnable())
			msg.ui->OnEvent(input);
}
ViewRect *UISurface::GetView()
{
	return &m_view;
}
template<class T>
T *UISurface::GetUIComponent()
{
	return (T*)std::find_if(m_uiComs.begin(), m_uiComs.end(),[&](const UIComponentMsg &uiMsg)
	{
		return typeid(*(uiMsg.ui)) == typeid(T);
	})->ui;
}
template<class T>
std::vector<T*> UISurface::GetUIComponents()
{
	std::vector<T*> uis;
	for (auto &uiMsg : m_uiComs)
	{
		if (typeid(*(uiMsg.ui)) == typeid(T))
			uis.push_back(uiMsg.ui);
	}
	return uis;
}

void UISurface::SetFocusUI(UIComponent *focusUi)
{
	if (m_focusUi != nullptr)
	{
		m_focusUi->m_isFocus = false;
		m_focusUi->OnLoseFocus();
	}
	m_focusUi = focusUi;
	m_focusUi->m_isFocus = true;
	focusUi->OnGetFocus();
}
UIComponent *UISurface::GetFocusUI() const
{
	return m_focusUi;
}
void UISurface::Draw()
{
	m_view.ResetSrcView();
	for (auto &msg : m_uiComs)
		if (msg.ui->IsEnable())
			msg.ui->OnDraw();
}
void UISurface::Draw(UIComponent *withoutUI)
{
	for (auto &msg : m_uiComs)
		if (msg.ui != withoutUI && msg.ui->IsEnable())
			msg.ui->OnDraw();
}
float UISurface::GetFps() const
{
	return m_fps;
}
void UISurface::SetFps(float fps)
{
	m_fps = fps;
}
#endif

