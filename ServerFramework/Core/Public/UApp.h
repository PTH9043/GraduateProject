#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_UAPP_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_UAPP_H

BEGIN(Core)
class UCoreInstance;
class UService;

/*
@ Date: 2024-01-09
@ Writer: 박태현
@ Explain
- 프로그램을 시작하기 위한 App 클래스이다. 
*/
class CORE_DLL UApp  {
public:
	UApp();
	virtual ~UApp();
public:
	virtual _bool Start() PURE;
protected:
	template<class T, class... Args>
	void CreateServiceObject(Args&&... _args) {
		m_spService = CreateInitConstructor<T>(m_spCoreInstance, std::forward<Args>(_args)...);
	}
protected: /* get set*/
	CSHPTRREF<UService> GetService() { return m_spService; }
private:
	SHPTR<UCoreInstance> m_spCoreInstance;
	SHPTR<UService>			 m_spService;
};

END

#endif // _SERVERFRAMEWORK_CORE_PUBLIC_UAPP_H 
