// Copyright Hont, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EasyCoroutine.h"

class GlobalEasyCoroutine final
{
public:
    static GlobalEasyCoroutine* Get() {

        if (bIsReleased) return nullptr;

        if (Instance == nullptr) {
            Instance = CreateInstance();
        }
        return Instance;
    }

    static void ReleasedStateRestore() {
        bIsReleased = false;
    }

    static void Release() {
        if (Instance != nullptr) {
            delete Instance;
            Instance = nullptr;
            bIsReleased = true;
        }
    }

private:
	static GlobalEasyCoroutine* CreateInstance() {
        GlobalEasyCoroutine* result = new GlobalEasyCoroutine();

		return result;
	}

private:
	static GlobalEasyCoroutine* Instance;
	static bool bIsReleased;

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

bool GlobalEasyCoroutine::bIsReleased = false;
GlobalEasyCoroutine* GlobalEasyCoroutine::Instance = nullptr;
