# OldschoolUnrealCoroutine
Oldschool style unreal coroutine use the Switch-Case mode.

# How to use?

## 1.Include 'EasyCoroutine.h':

Include 'EasyCoroutine.h' to your project.




## 2.Attach EasyCoroutine with Tickable Object:
```
//Define:
EasyCoroutine mEasyCoroutine = EasyCoroutine();

...

//Attach to 'FTicker' or 'AActor' and bounded 'Tick':
mEasyCoroutine.Tick();
```




## 3.define context class/struct:
```
//Your input to coroutine arguments or member temporary variables.
class TestCoroutineContext
{
public:
    int coroutineIdentifier;
    float cacheTime;
};
```




## 4.define coroutine function body:
```
int CoroutineTest(int yieldIndex, void* contextObject)//Pay attention function signature
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
    }//Use the switch case to control breakout and order

    return -1;// -1 is exit coroutine.
}
```




## 5.Start coroutine:
```
TestCoroutineContext* context = new TestCoroutineContext();
context1->coroutineIdentifier = 1;//input something.
mEasyCoroutine.StartCoroutine(CoroutineTest, context);//invoke, like the unity.
```
