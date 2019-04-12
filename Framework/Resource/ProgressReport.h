#pragma once
#include "Framework.h"

struct Progress
{
	Progress() { Clear(); }

	void Clear() {
		status.clear();
		jobsDone = 0;
		jobCount = 0;
		bLoading = false;
	}

	std::string status;
	int jobsDone;
	int jobCount;
	bool bLoading;
};

class ProgressReport final
{
public:
	static constexpr int Scene = 0;

public:
	static ProgressReport &Get() 
	{
		static ProgressReport instance;
		return instance;
	}

	auto IsLoading(const int &progressID) -> const bool { return reports[progressID].bLoading; }
	auto GetStatus(const int &progressID) -> const std::string { return reports[progressID].status; }
	auto GetPercentage(const int &progressID) { 
		float jobsDone = static_cast<float>(reports[progressID].jobsDone);
		float jobCount = static_cast<float>(reports[progressID].jobCount);
	
		return jobsDone / jobCount;
	}


	void SetIsLoading(const int &progressID, const bool &bLoading) { reports[progressID].bLoading = bLoading; }
	void SetStatus(const int &progressID, const std::string &status) { reports[progressID].status = status; }
	
	void SetJobsDone(const int &progressID, const int &jobsDone) {
		auto &report = reports[progressID];
		report.jobsDone = jobsDone;

		if (report.jobsDone >= report.jobCount) report.bLoading = false;
	}
	
	void SetJobCount(const int &progressID, const int &jobCount) { //여러개
		auto &report = reports[progressID];
		report.jobCount = jobCount;

		report.bLoading = true;
	}

	void IncrementJobsDone(const int &progressID) { //1개
		auto &report = reports[progressID];
		report.jobsDone++;

		if (report.jobsDone >= report.jobCount) report.bLoading = false;
	}

	void Reset(const int &progressID) {	reports[progressID].Clear(); }

private:
	ProgressReport() { reports[Scene] = Progress(); }
	~ProgressReport() = default;

	ProgressReport(const ProgressReport&) = delete;
	ProgressReport &operator=(const ProgressReport&) = delete;

private:
	std::map<int, Progress> reports;

};