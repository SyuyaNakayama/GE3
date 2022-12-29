#pragma once
class Timer
{
private:
	int timer_;
	int timeMem_;
public:
	Timer() { timer_ = timeMem_ = 0; }
	Timer(int timer) { timer_ = timeMem_ = timer; }
	bool CountDown();
	int GetPassTime() { return timeMem_ - timer_; }
};