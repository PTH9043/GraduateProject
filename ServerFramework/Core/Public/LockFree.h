#ifndef _SERVERFRAMEWORK_CORE_PUBLIC_LOCKFREE_H
#define _SERVERFRAMEWORK_CORE_PUBLIC_LOCKFREE_H

#include <random>
#include <queue>

namespace Core
{
	/*
	@ Date: 2023-12-29
	@ Writer: 박태현
	@ Explain: 멀티쓰레드 환경에서 하나의 포인터에 CAS가 몰리는 것을 방지하기 위해 쓰레드를 잠시 대기시키는 클래스
	*/
	class CACHE_ALGIN Backoff {
	public:
		Backoff(int _minDelay, int _maxDelay) : m_UniformBackOff{ _minDelay, _maxDelay }, m_Limit{ 0 }, m_MaxDelay{ _maxDelay } {}

		void Relax() {
			int delay = 0;
			m_Limit = m_UniformBackOff(m_Dre);
			m_Limit *= 2;
			if (m_Limit > m_MaxDelay)
				m_Limit = m_MaxDelay;
			std::this_thread::sleep_for(std::chrono::microseconds(delay));
		}
	private:
		std::default_random_engine					m_Dre;
		std::uniform_int_distribution<int>		m_UniformBackOff;
		int																m_MaxDelay;
		int																m_Limit;
	};

	namespace PTHStack {

		/*
		@ Date: 2023-12-29
		@ Writer: 박태현
		@ Explain: Stack의 노드
		*/
		template<class T>
		struct CACHE_ALGIN NODE {

			NODE<T>() : next{ nullptr }, retireEpoch{ 0 }, isRemove{ false }
			{
				if constexpr ((true == std::is_pointer<T>()) || (true == SmartPointer<T>))
					data = nullptr;
				else
					data = T();
			}
			NODE<T>(T _data, bool _isRemove = false) : next{ nullptr }, retireEpoch{ 0 }, data{ _data }, isRemove{ _isRemove }
			{}

			~NODE<T>()
			{
				if (true == isRemove)
				{
					if constexpr ((std::is_pointer<T>()))
					{
						delete data;
					}
				}
			}
		public:
			NODE<T>* volatile	next;
			T									data;
			std::atomic_uint		retireEpoch;
			bool								isRemove;
		};
		/*
		@ Date: 2023-12-29
		@ Writer: 박태현
		@ Explain: Stack의 EBR Controller
		*/
		template<class T>
		class EBRController {
		public:
			EBRController() : m_Epoch{ 0 }, m_RemainCount{ 0 }, m_Retired{},
				m_Reservation{}, m_CurrentNumThreads{ } {}
			~EBRController()
			{
				clear_ebr();
			}

			void Initialize(int _currThread, int _remainCount)
			{
				m_CurrentNumThreads = _currThread;
				m_RemainCount = _remainCount;

				for (int i = 0; i < m_CurrentNumThreads; ++i)
				{
					m_Reservation[i] = 0;
				}
			}

			unsigned int get_min_reservation() {
				unsigned int min_re = 0xffffffff;
				for (int i = 0; i < m_CurrentNumThreads; ++i) {
					min_re = std::min(min_re, m_Reservation[i].load(std::memory_order_relaxed));
				}
				return min_re;
			}

			void empty() {
				unsigned int max_safe_epoch = get_min_reservation();

				while (false == m_Retired[TLS::g_ThreadID].empty()) {
					auto f = m_Retired[TLS::g_ThreadID].front();
					if (f->retireEpoch >= max_safe_epoch)
						break;
					m_Retired[TLS::g_ThreadID].pop();
					delete f;
				}
			}

			void retire(NODE<T>* ptr) {
				m_Retired[TLS::g_ThreadID].push(ptr);
				ptr->retireEpoch = m_Epoch.load(std::memory_order_relaxed);
				if (m_Retired[TLS::g_ThreadID].size() >= m_RemainCount) {
					empty();
				}
			}

			void start_op() {
				m_Reservation[TLS::g_ThreadID].store(m_Epoch.fetch_add(1, std::memory_order_relaxed), std::memory_order_relaxed);
			}

			void end_op() {
				m_Reservation[TLS::g_ThreadID].store(ENDOP_VALUE, std::memory_order_relaxed);
			}

		private:
			void clear_ebr()
			{
				for (int i = 0; i < m_CurrentNumThreads; ++i)
				{
					while (!m_Retired[i].empty())
					{
						NODE<T>* node = m_Retired[i].front();
						delete node;
						m_Retired[i].pop();
					}
				}
				m_Epoch = 0;
			}

		private:
			static constexpr	unsigned int														ENDOP_VALUE{ 0xffffffff };
			std::atomic_uint																				m_Epoch;
			unsigned int																						m_RemainCount;
			std::array<std::queue<NODE<T>*>, TLS::MAX_THREAD>		m_Retired;
			std::array<std::atomic_uint, TLS::MAX_THREAD>					m_Reservation;
			int																										m_CurrentNumThreads;
		};
		/*
		@ Date: 2023-12-29
		@ Writer: 박태현
		@ Explain: LockFreeStack + Backoff
		*/
		template<class T>
		class CACHE_ALGIN LockFreeStack {
		public:
			LockFreeStack() : m_BackOff{ 1, 50 }, m_Top{ nullptr } { }
			~LockFreeStack() {
				clear();
			}

			void Initialize(int _currentThread, int _remainCount)
			{
				m_EBRController.Initialize(_currentThread, _remainCount);
			}

			T Top()
			{
				NODE<T>* pNode = m_Top;
				if (nullptr == pNode)
				{
					return T();
				}
				return pNode->data;
			}

			void Push(T _data, bool _isRemove = false)
			{
				m_EBRController.start_op();
				NODE<T>* newNode = new NODE{ _data, _isRemove };
				while (true) {
					NODE<T>* p = m_Top;
					newNode->next = p;
					if (true == CAS(p, newNode)) {
						m_EBRController.end_op();
						return;
					}
					m_BackOff.Relax();
				}
			}
			T Pop() {
				m_EBRController.start_op();
				while (true)
				{
					NODE<T>* ptr = m_Top;
					if (ptr == nullptr)
					{
						m_EBRController.end_op();
						return -2;
					}
					NODE<T>* next = ptr->next;
					T data = ptr->data;
					if (false == CAS(ptr, next))
					{
						m_BackOff.Relax();
						continue;
					}
					m_EBRController.retire(ptr);
					m_EBRController.end_op();
					return data;
				}
			}

			bool IsEmpty()
			{
				NODE<T>* pNode = m_Top;
				return pNode == nullptr;
			}

			void clear()
			{
				NODE<T>* p = m_Top;
				while (p != nullptr) {
					NODE<T>* t = p;
					p = p->next;
					delete t;
				}
				m_Top = nullptr;
			}
		private:
			bool CAS(NODE<T>* _old, NODE<T>* _new)
			{
				return std::atomic_compare_exchange_strong(reinterpret_cast<volatile std::atomic_llong*>(&m_Top),
					reinterpret_cast<long long*>(&_old),
					reinterpret_cast<long long>(_new));
			}
		private:
			EBRController<T>	m_EBRController;
			Core::Backoff			m_BackOff;
			NODE<T>* volatile	m_Top;
		};
	}

	template<class T>
	using CONSTACK = Core::PTHStack::LockFreeStack<T>;
}


#endif // _SERVERFRAMEWORK_CORE_PUBLIC_LOCKFREE_H