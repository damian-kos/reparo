
#pragma once
#include <iostream>
#include <list>
#include <string>

class IObserver {
public:
  virtual ~IObserver() {};
  virtual void Update(const int& passed_int = 0) = 0;
};

class ISubject  {
public:
  virtual ~ISubject() {};
  virtual void Attach(IObserver* observer) = 0;
  virtual void Detach(IObserver* observer) = 0;
  virtual void Notify() = 0;
};

