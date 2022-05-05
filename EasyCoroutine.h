#pragma once

#include "CoreMinimal.h"

#define COROUTINE_CASE_BEGIN 1
#define COROUTINE_YIELD_BREAK -1
#define COROUTINE_LOOP_MAX_LIMIT 512

#define COROUTINE_YIELD_CURRENT(index) index
#define COROUTINE_YIELD_NEXT(index) index+1

class EasyCoroutineContextBase
{
public:
	float internalTimer;
	int coroutineIdentifier;

	EasyCoroutineContextBase()
	{
		internalTimer = 0.0;
		coroutineIdentifier = 0;
	}
};

struct EasyCoroutineInstance
{
	int identifier;
	int yieldIndex;
	TSharedPtr<void> contextObject;
	int (*pfCoroutineBody)(int, TSharedPtr<void>) = nullptr;
};

class EasyCoroutine final
{
private:
	int mIdentifierCounter;
	TArray<EasyCoroutineInstance*> mCoroutines;

public:
	int StartCoroutine(int (*pf)(int, TSharedPtr<void>), TSharedPtr<void> contextObject)
	{
		EasyCoroutineInstance* coroutine = new EasyCoroutineInstance();
		coroutine->identifier = ++mIdentifierCounter;
		coroutine->yieldIndex = COROUTINE_CASE_BEGIN;
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

		for (int eps = 0; eps < COROUTINE_LOOP_MAX_LIMIT; eps++)
		{
			if (eps == COROUTINE_LOOP_MAX_LIMIT - 1)
			{
				UE_LOG(LogTemp, Log, TEXT("Coroutine out of max loop count, please check!"));
			}

			int returnYieldIndex = instance->pfCoroutineBody(instance->yieldIndex, instance->contextObject);
			if (returnYieldIndex == COROUTINE_YIELD_BREAK)
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

	static int WaitForSeconds(int currentYieldIndex, float& waitVariable, float duration)
	{
		if (waitVariable <= duration)
		{
			waitVariable += FApp::GetDeltaTime();

			return COROUTINE_YIELD_CURRENT(currentYieldIndex);
		}
		else
		{
			waitVariable = 0.0f;
			return COROUTINE_YIELD_NEXT(currentYieldIndex);
		}
	}
};
