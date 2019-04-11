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

//Task작업단위 - process - thread 보통 로딩할 때 진행도를 표시해주는 progress bar에서 사용
class Thread final : public ISubsystem
{
public:
	Thread(class Context *context);
	~Thread();

	Thread(const Thread&) = delete;
	Thread &operator=(const Thread&) = delete;

	const bool Initialize() override;

	void Invoke(); //실행 execute와 같음

	template <typename Process>
	void AddTask(Process &&process);

private:
	std::vector<std::thread> threads;
	std::queue<std::shared_ptr<Task>> tasks;  //선입선출, 삽입삭제가 빈번해 vector가 아닌 queue사용
	std::mutex taskMutex;
	std::condition_variable conditionVar; //쓰레드에 통신하여 깨워줌
	uint threadCount;
	bool bStopping;
};

//unique_ptr  - 다른 놈이 참조 못함. 포인터를 나눠주면 이전에 있던 내용은 null이 되고 넘겨준 놈이 데이터를 가짐
//shared_ptr  - 하나라도 참조를 가지고 있으면 삭제하지 않음. 참조한 모든 놈이 삭제되면 삭제됨.서로가 참조를 가지고 있으면 순환참조가 일어나 삭제되지 않음.
//weak_ptr    - 그 문제를 해결하기 위해 나온 스마트포인터. 원본이 삭제되도 큰 문제가 없을 때 사용.

template<typename Process>
inline void Thread::AddTask(Process && process)
{
	if (threads.empty()) {
		Log::Warning("Thread::AddTask - Invailable threads");
		process();
		return;
	}

	std::unique_lock<std::mutex> lock(taskMutex); //lock_guard도 있지만(비슷한 기능) condition_variable을 사용하려면 unique_lock의 사용이 강제됨. 내가 원하는 때에 락과 언락이 가능
	{
		tasks.push(std::make_shared<Task>(std::bind(std::forward<Process>(process))));
	}
	lock.unlock();

	conditionVar.notify_one(); //쓰레드 하나를 깨움
}
