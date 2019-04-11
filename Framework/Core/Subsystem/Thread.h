#pragma once
#include "ISubsystem.h"

class Task final
{
public:
	typedef std::function<void()> Process;

public:
	Task(Process &&process) {
		this->process = std::forward<Process>(process);
	}
	~Task() = default;

	Task(const Task&) = delete;
	Task &operator=(const Task&) = delete;

	void Execute() {
		process();
	}

private:
	Process process;
};

//Task�۾����� - process - thread ���� �ε��� �� ���൵�� ǥ�����ִ� progress bar���� ���
class Thread final : public ISubsystem
{
public:
	Thread(class Context *context);
	~Thread();

	Thread(const Thread&) = delete;
	Thread &operator=(const Thread&) = delete;

	const bool Initialize() override;

	void Invoke(); //���� execute�� ����

	template <typename Process>
	void AddTask(Process &&process);

private:
	std::vector<std::thread> threads;
	std::queue<std::shared_ptr<Task>> tasks;  //���Լ���, ���Ի����� ����� vector�� �ƴ� queue���
	std::mutex taskMutex;
	std::condition_variable conditionVar; //�����忡 ����Ͽ� ������
	uint threadCount;
	bool bStopping;
};

//unique_ptr  - �ٸ� ���� ���� ����. �����͸� �����ָ� ������ �ִ� ������ null�� �ǰ� �Ѱ��� ���� �����͸� ����
//shared_ptr  - �ϳ��� ������ ������ ������ �������� ����. ������ ��� ���� �����Ǹ� ������.���ΰ� ������ ������ ������ ��ȯ������ �Ͼ �������� ����.
//weak_ptr    - �� ������ �ذ��ϱ� ���� ���� ����Ʈ������. ������ �����ǵ� ū ������ ���� �� ���.

template<typename Process>
inline void Thread::AddTask(Process && process)
{
	if (threads.empty()) {
		Log::Warning("Thread::AddTask - Invailable threads");
		process();
		return;
	}

	std::unique_lock<std::mutex> lock(taskMutex); //lock_guard�� ������(����� ���) condition_variable�� ����Ϸ��� unique_lock�� ����� ������. ���� ���ϴ� ���� ���� ����� ����
	{
		tasks.push(std::make_shared<Task>(std::bind(std::forward<Process>(process))));
	}
	lock.unlock();

	conditionVar.notify_one(); //������ �ϳ��� ����
}
