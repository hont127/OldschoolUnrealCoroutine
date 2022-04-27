#pragma once

#include "CoreMinimal.h"

struct EasyCoroutineInstance
{
	int identifier;
	int yieldIndex;
	void* contextObject;
	int (*pfCoroutineBody)(int, void*) = nullptr;
};

class EasyCoroutine
{
private:
	int mIdentifierCounter;
	TArray<EasyCoroutineInstance*> mCoroutines;

public:

	int StartCoroutine(int (*pf)(int, void*), void* contextObject)
	{
		EasyCoroutineInstance* coroutine = new EasyCoroutineInstance();
		coroutine->identifier = ++mIdentifierCounter;
		coroutine->yieldIndex = 1;
		coroutine->contextObject = contextObject;
		coroutine->pfCoroutineBody = pf;

		mCoroutines.Add(coroutine);

		return coroutine->identifier;
	}

	void StopCoroutine(int coroutineIdentifier)
	{
		int i = mCoroutines.Num();
		while (--i > -1)
		{
			EasyCoroutineInstance* instance = mCoroutines[i];
			if (instance->identifier == coroutineIdentifier)
			{
				delete instance;
				mCoroutines.RemoveAt(i);
				break;
			}
		}
	}

	void StopAllCoroutines()
	{
		int i = mCoroutines.Num();
		while (--i > -1)
		{
			EasyCoroutineInstance* instance = mCoroutines[i];
			delete instance;
		}

		mCoroutines.Empty();
	}

	int CurrentCoroutineCount()
	{
		return mCoroutines.Num();
	}

	bool TickInstance(EasyCoroutineInstance* instance)
	{
		if (instance->pfCoroutineBody == nullptr) return true;

		for (int eps = 0; eps < 32; eps++)
		{
			if (eps == 31)
			{
				UE_LOG(LogTemp, Log, TEXT("----- Easy Coroutine Loop Max Error! -------"));
			}

			int returnYieldIndex = instance->pfCoroutineBody(instance->yieldIndex, instance->contextObject);
			if (returnYieldIndex == -1)
			{
				instance->pfCoroutineBody = nullptr;

				break;
			}
			else
			{
				if (returnYieldIndex != instance->yieldIndex)
				{
					instance->yieldIndex = returnYieldIndex;
				}
				else
				{
					break;
				}
			}
		}

		if (instance->pfCoroutineBody == nullptr) {
			delete instance->contextObject;
			return true;
		}

		return false;
	}

	void Tick()
	{
		int i = mCoroutines.Num();
		while (--i > -1)
		{
			EasyCoroutineInstance* instance = mCoroutines[i];

			if (TickInstance(instance))
			{
				delete instance;
				mCoroutines.RemoveAt(i);
			}
		}
	}

	static int WaitForSeconds(int waitYieldIndex, int nextYieldIndex, float& waitVariable, float duration)
	{
		if (waitVariable <= duration)
		{
			waitVariable += FApp::GetDeltaTime();

			return waitYieldIndex;
		}
		else
		{
			waitVariable = 0;
			return nextYieldIndex;
		}
	}
};