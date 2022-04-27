# OldschoolUnrealCoroutine
Oldschool style unreal coroutine

# How to use?

## 1.Attach EasyCoroutine with AActor(Tickable Object):
```
//define:
EasyCoroutine mEasyCoroutine = EasyCoroutine();

//attach to FTicker or AActor tick or etc:
mEasyCoroutine.Tick();
```

## 1.define context class:
```
//Your input to coroutine arguments or member temp variables.
class TestCoroutineContext
{
public:
    int coroutineIdentifier;
    float cacheTime;
};
```

## 2.define coroutine function body:
```
int CoroutineTest(int yieldIndex, void* contextObject)
{
    TestCoroutineContext* context = (TestCoroutineContext*)contextObject;

    switch (yieldIndex)
    {
        case 1:
        {
            UE_LOG(LogTemp, Log, TEXT("coroutine%d: begin"), context->coroutineIdentifier);
            return 2;
        }
        break;

        case 2:
        {
            return EasyCoroutine::WaitForSeconds(2, 3, context->cacheTime, 1.0);
        }
        break;

        case 3:
        {
            UE_LOG(LogTemp, Log, TEXT("coroutine%d: end"), context->coroutineIdentifier);
            return -1;
        }
        break;
    }

    return -1;// -1 is exit coroutine.
}
```

## 3.invoke:
```
TestCoroutineContext* context1 = new TestCoroutineContext();
context1->coroutineIdentifier = 1;//input something.
mEasyCoroutine.StartCoroutine(CoroutineTest, context1);//invoke, like the unity.
```
