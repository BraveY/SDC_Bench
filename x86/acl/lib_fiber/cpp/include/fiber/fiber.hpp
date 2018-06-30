#pragma once
#include <stddef.h>
#include "fiber_cpp_define.hpp"

struct ACL_FIBER;

namespace acl {

typedef enum 
{
	FIBER_EVENT_T_KERNEL,  // Linux: epoll, FreeBSD: kquque, Windows: iocp
	FIBER_EVENT_T_POLL,
	FIBER_EVENT_T_SELECT,
	FIBER_EVENT_T_WMSG,
} fiber_event_t;

/**
 * Э���ඨ�壬�����࣬��Ҫ����̳в�ʵ�ִ��鷽��
 */
class FIBER_CPP_API fiber
{
public:
	/**
	 * ���캯��
	 * @param running {bool} ��Ϊ true ʱ�����ʾ��ǰЭ������������������
	 *  ��һ��Э�̶�����ѣ��Ա����� ACL_FIBER ����󶨣���ʱ��ֹ���ñ���
	 *  ��� start ����������Э��; ��Ϊ false ʱ������Ҫ���� start ������
	 *  ������Э��
	 */
	fiber(bool running = false);
	virtual ~fiber(void);

	/**
	 * �ڴ���һ��Э��������ҹ������ running Ϊ false ʱ����Ҫ����������
	 * Э�̣�Ȼ����������ص� run ���������ص������ running Ϊ true ʱ��
	 * ���ֹ���� start ����
	 * @param stack_size {size_t} ������Э�̶����ջ��С
	 */
	void start(size_t stack_size = 320000);

	/**
	 * �ڱ�Э������ʱ���ô˺���֪ͨ��Э���˳�
	 * @return {bool} ���� false ��ʾ��Э��δ�������Ѿ��˳�
	 */
	bool kill(void);

	/**
	 * �жϵ�ǰЭ���Ƿ�֪ͨ�˳�
	 * @return {bool} ��Э���Ƿ�֪ͨ�˳�
	 */
	bool killed(void) const;

	/**
	 * �жϵ�ǰ�������е�Э���Ƿ�֪ͨ�˳����÷����� killed ������Ϊ��
	 * killed ���ȱ����� acl::fiber �������У��Ҹ�Э�̶����п����������У�
	 * Ҳ�п��ܱ����𣬶� self_killed ����Ҫ acl::fiber ����������һ����ʾ
	 * ��ǰ�������е�Э��
	 * @return {bool}
	 */
	static bool self_killed(void);

	/**
	 * ��ñ�Э�̶���� ID ��
	 * @return {unsigned int}
	 */
	unsigned int get_id(void) const;

	/**
	 * ��õ�ǰ���е�Э�̶���� ID ��
	 * @return {unsigned int}
	 */
	static unsigned int self(void);

	/**
	 * ��õ�ǰЭ����ִ��ĳ��ϵͳ API ����ʱ�Ĵ����
	 * return {int}
	 */
	int get_errno(void) const;

	/**
	 * ���õ�ǰЭ�̵Ĵ����
	 * @param errnum {int}
	 */
	void set_errno(int errnum);

	/**
	 * ��ʽ����Э�̵����¼��������ͣ�ͬʱ����Э�̵�����Ϊ������ģʽ����������Э�̺󲻱�
	 * ��ʽ���� schedule �� schedule_with ������Э�̵�����
	 * @param type {fiber_event_t} �¼��������ͣ��μ���FIBER_EVENT_T_XXX
	 * @param schedule_auto {bool} ���Ϊ true���򴴽�Э�̶�������и�Э��
	 *  ����󲻱���ʽ���� schedule/schedule_with ���������е�Э�̹��̣��ڲ���
	 *  �Զ�����Э�̵������������ڴ���������Э�̺󣬱�����ʽ�ص��� schedule ��
	 *  schedule_with ��ʽ������Э�̵���������������Э�̹��̣��ڲ�ȱʡ״̬Ϊ false
	 */
	static void init(fiber_event_t type, bool schedule_auto = false);

	/**
	 * ����Э�����еĵ��ȹ���
	 */
	static void schedule(void);

	/**
	 * ����Э�̵���ʱָ���¼��������ͣ����ñ���������ͬʱ������ schedule_init
	 * �� schedule ��������
	 * @param type {fiber_event_t} �¼��������ͣ��μ���FIBER_EVENT_T_XXX
	 */
	static void schedule_with(fiber_event_t type);

	/**
	 * �жϵ�ǰ�߳��Ƿ���Э�̵���״̬
	 * @return {bool}
	 */
	static bool scheduled(void);

	/**
	 *  ֹͣЭ�̵��ȹ���
	 */
	static void schedule_stop(void);

public:
	/**
	 * ����ǰ�������е�Э��(����Э��) ����
	 */
	static void yield(void);

	/**
	 * ����ǰЭ�̣�ִ�еȴ������е���һ��Э��
	 */
	static void switch_to_next(void);

	/**
	 * ��ָ��Э�̶�����������ж�����
	 * @param f {fiber&}
	 */
	static void ready(fiber& f);

	/**
	 * ʹ��ǰ���е�Э������ָ��������
	 * @param milliseconds {unsigned int} ָ��Ҫ���ߵĺ�����
	 * @return {unsigned int} ��Э�����ߺ��ٴα����Ѻ�ʣ��ĺ�����
	 */
	static unsigned int delay(unsigned int milliseconds);

	/**
	 * �߳���������ô˺������õ�ǰ�߳��Ƿ���Ҫ hook ϵͳ API���ڲ�ȱʡ
	 * �� hook ϵͳ API
	 * @param on {bool}
	 */
	static void hook_api(bool on);

	/**
	 * ��ʽ���ñ�����ʹ acl ������� IO ����Э�̻����� UNIX ƽ̨�²�����ʽ����
	 * ����������Ϊ�ڲ����Զ� HOOK IO API
	 */
	static void acl_io_hook(void);

	/**
	 * ���ñ�����ȡ�� acl�������е� IO Э�̻�
	 */
	static void acl_io_unlock(void);

	/**
	 * ��õ�ǰϵͳ�������
	 * @return {int}
	 */
	static int  get_sys_errno(void);

	/**
	 * ���õ�ǰϵͳ�������
	 * @param errnum {int}
	 */
	static void set_sys_errno(int errnum);

public:
	/**
	 * ���ر�Э�̶����Ӧ�� C ���Ե�Э�̶���
	 * @return {ACL_FIBER* }
	 */
	ACL_FIBER* get_fiber(void) const;

protected:
	/**
	 * �麯����������ʵ�ֱ���������ͨ������ start ��������Э�̺󣬱�
	 * �麯�����ᱻ���ã��Ӷ�֪ͨ����Э��������; ����ڹ��캯���еĲ���
	 * running Ϊ true ���� start ������ֹ���ã��ʱ��鷽��Ҳ���ᱻ����
	 */
	virtual void run(void);

private:
	ACL_FIBER* f_;

	static void fiber_callback(ACL_FIBER* f, void* ctx);
};

/**
 * ��������ʱ����Э����
 */
class FIBER_CPP_API fiber_timer
{
public:
	fiber_timer(void);
	virtual ~fiber_timer(void) {}

	/**
	 * ����һ��Э�̶�ʱ��
	 * @param milliseconds {unsigned int} ���뼶ʱ��
	 * @param stack_size {size_t} Э�̵�ջ�ռ��С
	 */
	void start(unsigned int milliseconds, size_t stack_size = 320000);

protected:
	/**
	 * �������ʵ�ָô��鷽��������ʱ������ʱ��ص��÷���
	 */
	virtual void run(void) = 0;

private:
	ACL_FIBER* f_;

	static void timer_callback(ACL_FIBER* f, void* ctx);
};

/**
 * ��ʱ������Э��
 */
template <typename T>
class fiber_trigger : public fiber
{
public:
	fiber_trigger(timer_trigger<T>& timer)
	: delay_(100)
	, stop_(false)
	, timer_(timer)
	{
	}

	virtual ~fiber_trigger(void) {}

	void add(T* o)
	{
		mbox_.push(o);
	}

	void del(T* o)
	{
		timer_.del(o);
	}

	timer_trigger<T>& get_trigger(void)
	{
		return timer_;
	}

	// @override
	void run(void)
	{
		while (!stop_) {
			T* o = mbox_.pop(delay_);
			if (o)
				timer_.add(o);

			long long next = timer_.trigger();
			long long curr = get_curr_stamp();
			if (next == -1)
				delay_ = 100;
			else {
				delay_ = next - curr;
				if (delay_ <= 0)
					delay_ = 1;
			}
		}
	}

private:
	long long delay_;
	bool stop_;

	timer_trigger<T>& timer_;
	mbox<T> mbox_;
};

} // namespace acl

#if defined(__GNUC__) && (__GNUC__ > 6 ||(__GNUC__ == 6 && __GNUC_MINOR__ >= 0))
# ifndef   ACL_USE_CPP11
#  define  ACL_USE_CPP11
# endif
#endif

#ifdef	ACL_USE_CPP11

#include <functional>

namespace acl
{

class FIBER_CPP_API go_fiber
{
public:
	go_fiber(void) {}
	go_fiber(size_t stack_size) : stack_size_(stack_size) {}

	void operator=(std::function<void()> fn);

private:
	size_t stack_size_ = 320000;
};

} // namespace acl

#define	go		acl::go_fiber()=
#define	go_stack(size)	acl::go_fiber(size)=

/**
 * static void fiber1(void)
 * {
 * 	printf("fiber: %d\r\n", acl::fiber::self());
 * }
 *
 * static void fiber2(acl::string& buf)
 * {
 * 	printf("in fiber: %d, buf: %s\r\n", acl::fiber::self(), buf.c_str());
 * 	buf = "world";
 * }
 *
 * static void fiber3(const acl::string& buf)
 * {
 * 	printf("in fiber: %d, buf: %s\r\n", acl::fiber::self(), buf.c_str());
 * }
 *
 * static test(void)
 * {
 * 	go fiber1;
 * 	
 * 	acl::string buf("hello");
 *
 * 	go[&] {
 * 		fiber2(buf);
 * 	};
 * 	
 * 	go[=] {
 * 		fiber3(buf);
 * 	};
 * 
 * 	go[&] {
 * 		fiber3(buf);
 * 	};
 * }
 */
#endif