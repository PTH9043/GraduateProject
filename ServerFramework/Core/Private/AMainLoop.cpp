#include "CoreDefines.h"
#include "AMainLoop.h"
#include "AService.h"
#include "AGameTimer.h"
#include "ACoreInstance.h"
#include "AMonster.h"
#include "AAnimator.h"
#include "AAnimController.h"

namespace Core {

	AMainLoop::AMainLoop(OBJCON_CONSTRUCTOR, Asio::io_context& _context, SHPTR<AService> _spServerService) :
		ACoreObject(OBJCON_CONDATA), m_spGameTimer{ Create<AGameTimer>() }, 
		m_SteadyTimer{ _context }, m_wpServerService{_spServerService}
	{

	}

	void AMainLoop::RegisterTimer(_int _iTime)
	{
		m_SteadyTimer.expires_from_now(std::chrono::microseconds(_iTime));
		// 타이머의 비동기 대기 설정
		m_SteadyTimer.async_wait(std::bind(&AMainLoop::TimerThread, this, std::placeholders::_1));
	}

	void AMainLoop::TimerThread(const boost::system::error_code& _error)
	{
		m_spGameTimer->Tick();
		SHPTR<ACoreInstance> spCoreInstance = GetCoreInstance();
		SHPTR<AService> spServerService = m_wpServerService.lock();
		{
			_double dTimeDelta = m_spGameTimer->GetDeltaTime();
			LIST<AGameObject*> AliveObjectList;
			{
				// Mob을 찾음 
				for (AGameObject* iter : m_PawnCollisionList)
				{
					AGameObject* spObject = iter;

					if (true == spObject->IsPermanentDisable())
						continue;

					AliveObjectList.push_back(spObject);
				}

				// Mob을 찾음 
				for (AGameObject* iter : m_StaticObjCollisionList)
				{
					AGameObject* spObject = iter;

					if (true == spObject->IsPermanentDisable())
						continue;

					AliveObjectList.push_back(spObject);
				}

				if (0 >= AliveObjectList.size())
				{
					RegisterTimer(10);
					return;
				}
			}
			// Animation 활성 
			{
				std::atomic_thread_fence(std::memory_order_seq_cst);
				for (auto& iter : AliveObjectList)
				{
					iter->Tick(dTimeDelta);
				}
				// Pawn Collision 
				for (AGameObject* iter : m_PawnCollisionList)
				{
					if (true == iter->IsPermanentDisable())
						continue;
					// Pawn Collision
					for (AGameObject* value : m_PawnCollisionList)
					{
						if (iter == value)
							continue;

						iter->Collision(value, dTimeDelta);
					}
					// Static Obj Coll
					for (AGameObject* value : m_StaticObjCollisionList)
					{
						iter->Collision(value, dTimeDelta);
					}
				}
				// Static Obj CollisionList 
				for (AGameObject* iter : m_StaticObjCollisionList)
				{
					if (true == iter->IsPermanentDisable())
						continue;

					for (AGameObject* value : m_PawnCollisionList)
					{
						iter->Collision(value, dTimeDelta);
					}
				}
			}
		}
		RegisterTimer(1);
	}

	void AMainLoop::InsertPawnCollisionList(AGameObject* _pGameObject)
	{
		m_PawnCollisionList.insert(_pGameObject);
	}

	void AMainLoop::InsertStaticObjCollisionList(AGameObject* _pGameObject)
	{
		m_StaticObjCollisionList.insert(_pGameObject);
	}

	void AMainLoop::Free()
	{
	}

}