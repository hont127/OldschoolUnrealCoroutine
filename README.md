# OldschoolUnrealCoroutine
Oldschool style unreal coroutine use the Switch-Case mode.

# How to use?

## 1.Include 'EasyCoroutine.h':

Include 'EasyCoroutine.h' to your project.




## 2.Attach EasyCoroutine with Tickable Object:
``` c++
//Define:
EasyCoroutine EasyCoroutine = EasyCoroutine();

...

//Attach to 'FTicker' or 'AActor' etc, and bounded 'Tick':
EasyCoroutine.Tick();
```




## 3.Define context class/struct:
``` c++
//Context object saved input arguments and member temporary variables.
class TestCoroutineContext : public EasyCoroutineContextBase
{
public:
    int32 CoroutineIdentifier;
    float CacheTime;
};
```




## 4.Define coroutine function body:
``` c++
int32 CoroutineTest(int32 YieldIndex, TSharedPtr<void> ContextObject)
{
    TestCoroutineContext* Context = (TestCoroutineContext*)ContextObject.Get();

    switch (YieldIndex)
    {
        case COROUTINE_CASE_BEGIN:
        {
            UE_LOG(LogTemp, Log, TEXT("coroutine%d: begin"), Context->CoroutineIdentifier);
            return COROUTINE_YIELD_NEXT(YieldIndex);
        }
        break;

        case COROUTINE_CASE_BEGIN + 1:
        {
            return EasyCoroutine::WaitForSeconds(YieldIndex, Context->CacheTime, 1.0);
        }
        break;

        case COROUTINE_CASE_BEGIN + 2:
        {
            UE_LOG(LogTemp, Log, TEXT("Coroutine%d: end"), Context->CoroutineIdentifier);
            return COROUTINE_YIELD_BREAK;
        }
        break;
    }

    return COROUTINE_YIELD_BREAK;
}
```




## 5.Start coroutine:
``` c++
EasyCoroutine.StartCoroutine(CoroutineTest, MakeShareable(new TestCoroutineContext()));//invoke, like the unity.
```

## 6.Start global coroutine:
``` c++
GlobalEasyCoroutine::Get()->RegisterCoroutine();//bind register.
...
GlobalEasyCoroutine::Get()->GetEC()->StartCoroutine(CoroutineTest, MakeShareable(new EasyCoroutineContextBase()));
```
