// Copyright Hont, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"

#define COROUTINE_CASE_BEGIN 1
#define COROUTINE_YIELD_BREAK -1
#define COROUTINE_LOOP_MAX_LIMIT 512

#define COROUTINE_YIELD_CURRENT(index) index
#define COROUTINE_YIELD_NEXT(index) index+1

/** Easy coroutine context base struct */
struct EasyCoroutineContextBase
{
	float InternalTimer;
	int32 CoroutineIdentifier;

	EasyCoroutineContextBase()
		: InternalTimer(0.0)
		, CoroutineIdentifier(0)
	{
	}
};

/** Easy coroutine execute logic instance object */
struct EasyCoroutineInstance
{
	int32 Identifier;
	int32 YieldIndex;
	TSharedPtr<void> ContextObject;
	int32 (*PfCoroutineBody)(int32, TSharedPtr<void>);

	EasyCoroutineInstance()
		: Identifier(0)
		, YieldIndex(0)
		, ContextObject(nullptr)
		, PfCoroutineBody(nullptr)
	{
	}
};

/** Easy Coroutine provides common coroutine support. */
class EasyCoroutine final
{
private:
	int32 IdentifierCounter = 0;
	TArray<EasyCoroutineInstance*> Coroutines;

	bool TickInstance(EasyCoroutineInstance* Instance)
	{
		if (Instance->PfCoroutineBody == nullptr) return true;

		for (int32 Eps = 0; Eps < COROUTINE_LOOP_MAX_LIMIT; Eps++)
		{
			if (Eps == COROUTINE_LOOP_MAX_LIMIT - 1)
			{
				UE_LOG(LogTemp, Log, TEXT("Coroutine out of max loop count, please check! %s %s %d"), TEXT(__FILE__), TEXT(__FUNCTION__), __LINE__);
			}

			int32 returnYieldIndex = Instance->PfCoroutineBody(Instance->YieldIndex, Instance->ContextObject);
			if (returnYieldIndex == COROUTINE_YIELD_BREAK)
			{
				Instance->PfCoroutineBody = nullptr;

				break;
			}
			else
			{
				if (returnYieldIndex != Instance->YieldIndex)
				{
					Instance->YieldIndex = returnYieldIndex;
				}
				else
				{
					break;
				}
			}
		}

		if (Instance->PfCoroutineBody == nullptr) {
			return true;
		}

		return false;
	}

public:

	EasyCoroutine() {}
	~EasyCoroutine() {}


	/**
	 * Start new coroutine by standard function sign format.
	 * @PARAM Pf is the coroutine function
	 * @PARAM ContextObject is coroutine internal context.
	 *
	 *
	 * @Return Coroutine identifier.
	 */
	int32 StartCoroutine(int32 (*Pf)(int32, TSharedPtr<void>), TSharedPtr<void> ContextObject)
	{
		EasyCoroutineInstance* Coroutine = new EasyCoroutineInstance();
		Coroutine->Identifier = ++IdentifierCounter;
		Coroutine->YieldIndex = COROUTINE_CASE_BEGIN;
		Coroutine->ContextObject = ContextObject;
		Coroutine->PfCoroutineBody = Pf;

		Coroutines.Add(Coroutine);

		return Coroutine->Identifier;
	}

	/**
	 * Stop the coroutine.
	 * @PARAM CoroutineIdentifier is the coroutine identifier
	 */
	void StopCoroutine(const int32 CoroutineIdentifier)
	{
		int32 Idx = Coroutines.Num();
		while (--Idx > -1)
		{
			EasyCoroutineInstance* Instance = Coroutines[Idx];
			if (Instance->Identifier == CoroutineIdentifier)
			{
				delete Instance;
				Coroutines.RemoveAt(Idx);
				break;
			}
		}
	}

	/**
	 * Stop all coroutines.
	 */
	void StopAllCoroutines()
	{
		int32 Idx = Coroutines.Num();
		while (--Idx > -1)
		{
			EasyCoroutineInstance* Instance = Coroutines[Idx];
			delete Instance;
		}

		Coroutines.Empty();
	}

	/**
	 * Get current coroutine count.
	 *
	 *
	 * @Return Current coroutine count.
	 */
	FORCEINLINE int32 CurrentCoroutineCount()
	{
		return Coroutines.Num();
	}

	void Tick()
	{
		int32 Idx = Coroutines.Num();
		while (--Idx > -1)
		{
			EasyCoroutineInstance* Instance = Coroutines[Idx];

			if (TickInstance(Instance))
			{
				delete Instance;
				Coroutines.RemoveAt(Idx);
			}
		}
	}

	static int32 WaitForSeconds(const int32 CurrentYieldIndex, float& WaitVariable, const float Duration)
	{
		if (WaitVariable <= Duration)
		{
			WaitVariable += FApp::GetDeltaTime();

			return COROUTINE_YIELD_CURRENT(CurrentYieldIndex);
		}
		else
		{
			WaitVariable = 0.0f;
			return COROUTINE_YIELD_NEXT(CurrentYieldIndex);
		}
	}
};
