#ifndef SURFACEMANAGER
#define SURFACEMANAGER

#include <unordered_map>
#include <string>
#include "UISurface.h"
#include "TimeSystem/Timer.h"
#include "TimeSystem/Time.h"

/*ʹ�õ�����ģʽ�������*/
class SurfaceManager
{
public:
	/*��ȡʵ��*/
	static SurfaceManager *GetInstance();

	/*ע�����*/
	void RegisterSurface(const std::string &name, UISurface *surface);

	/*��ȡ����*/
	UISurface *GetSurface(const std::string &name);

	inline UISurface *GetActiveSurface()const;

	/*��������*/
	void StartSurface(const std::string &name);

	/*��������*/
	void StartSurface(const std::string &name,void *data);

	/*���ٵ�ǰ�����*/
	void DestroyActiveSurface();

	/*���ٵ�ǰ����棬��������һ���µĻ����(��ʹ��ǰ�������ջ��)*/
	void RestartActiveSurface(const std::string& name);

	/*��ȡ��ǰ������FPS*/
	inline float GetActiveFPS()const;

	/*������Ƿ�Ϊ��*/
	inline bool Empty()const;

private:
	friend class UIMain;
	static SurfaceManager *s_instance;

	SurfaceManager() = default;
	SurfaceManager(const SurfaceManager&) = delete;
	SurfaceManager(SurfaceManager&&) = delete;

	std::unordered_map<std::string, UISurface *> m_surfaces;//hash map�Ͳ�ʵ���ˣ�̫�鷳��
	std::vector<UISurface *> m_activeSurfaces;//�����ջ��Щ���ܲ���ʵ��

	/*���´���ջ���Ľ��棬������false��û�л���棬�˳�����*/
	inline void OnUpdata();

	/*���¼�*/
	inline void OnEvent(char input);

	/*ע����ɣ����ý����OnCreate*/
	inline void Begin();

	/*������������ý����OnDestroy*/
	inline void End();
};

UISurface *SurfaceManager::GetActiveSurface() const
{
	if (!m_activeSurfaces.empty())
		return m_activeSurfaces.back();
	return nullptr;
}
float SurfaceManager::GetActiveFPS()const
{
	if (m_activeSurfaces.empty())
		return INFINITY;
	return m_activeSurfaces.back()->GetFps();
}
bool SurfaceManager::Empty() const
{
	return m_activeSurfaces.empty();
}
void SurfaceManager::OnUpdata()
{
	if (!m_activeSurfaces.empty())
		GetActiveSurface()->OnUpdate();
}
void SurfaceManager::OnEvent(char input)
{
	if (!m_activeSurfaces.empty())
		m_activeSurfaces.back()->OnEvent(input);
}
void SurfaceManager::Begin()
{
	for (auto &i : m_surfaces)
		i.second->OnCreate();
}
void SurfaceManager::End()
{
	for (auto &i : m_surfaces)
	{
		i.second->OnDestroy();
		delete i.second;
	}
}
#endif
