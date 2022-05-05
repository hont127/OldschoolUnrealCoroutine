// Copyright Hont, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EasyCoroutine.h"

class GlobalEasyCoroutine final
{
public:
    static GlobalEasyCoroutine* Instance() {

        if (sIsReleased) return nullptr;

        if (sInstance == nullptr) {
            sInstance = CreateInstance();
        }
        return sInstance;
    }

    static void ReleasedStateRestore() {
        sIsReleased = false;
    }

    static void Release() {
        if (sInstance != nullptr) {
            delete sInstance;
            sInstance = nullptr;
            sIsReleased = true;
        }
    }

private:
	static GlobalEasyCoroutine* CreateInstance() {
        GlobalEasyCoroutine* result = new GlobalEasyCoroutine();

		return result;
	}

private:
	static GlobalEasyCoroutine* sInstance;
	static bool sIsReleased;

private:
    FDelegateHandle UpdateTickerHandle;
    EasyCoroutine* InternalEasyCoroutine;

public:
    EasyCoroutine* GetEC()
    {
        return InternalEasyCoroutine;
    }

    void RegisterCoroutine()
    {
        InternalEasyCoroutine = new EasyCoroutine();

        UpdateTickerHandle = FTicker::GetCoreTicker().AddTicker(TEXT("Global Easy Coroutine"), 0.0f, [this](float DeltaTime)
        {
            InternalEasyCoroutine->Tick();
            return true;
        });
    }

    void UnregisterCoroutine()
    {
        FTicker::GetCoreTicker().RemoveTicker(UpdateTickerHandle);
        delete InternalEasyCoroutine;
    }
};

bool GlobalEasyCoroutine::sIsReleased = false;
GlobalEasyCoroutine* GlobalEasyCoroutine::sInstance = nullptr;
