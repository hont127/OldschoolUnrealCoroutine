# OldschoolUnrealCoroutine
Oldschool style unreal coroutine use the Switch-Case mode.

# How to use?

## 1.Include 'EasyCoroutine.h':

Include 'EasyCoroutine.h' to your project.




## 2.Attach EasyCoroutine with Tickable Object:
``` c++
//Define:
EasyCoroutine mEasyCoroutine = EasyCoroutine();

...

//Attach to 'FTicker' or 'AActor' etc, and bounded 'Tick':
mEasyCoroutine.Tick();
```




## 3.Define context class/struct:
``` c++
//Context object saved input arguments and member temporary variables.
class TestCoroutineContext
{
public:
    int coroutineIdentifier;
    float cacheTime;
};
```




## 4.Define coroutine function body:
``` c++
int CoroutineTest(int yieldIndex, void* contextObject)
{
	TestCoroutineContext* context = (TestCoroutineContext*)contextObject;

	switch (yieldIndex)
	{
		case COROUTINE_CASE_BEGIN:
		{
			UE_LOG(LogTemp, Log, TEXT("coroutine%d: begin"), context->coroutineIdentifier);
			return COROUTINE_YIELD_NEXT(yieldIndex);
		}
		break;

		case COROUTINE_CASE_BEGIN + 1:
		{
			return EasyCoroutine::WaitForSeconds(yieldIndex, context->cacheTime, 1.0);
		}
		break;

		case COROUTINE_CASE_BEGIN + 2:
		{
			UE_LOG(LogTemp, Log, TEXT("coroutine%d: end"), context->coroutineIdentifier);
			return COROUTINE_YIELD_BREAK;
		}
		break;
	}

	return COROUTINE_YIELD_BREAK;
}
```




## 5.Start coroutine:
``` c++
TestCoroutineContext* context = new TestCoroutineContext();
context1->coroutineIdentifier = 1;//input something.
mEasyCoroutine.StartCoroutine(CoroutineTest, context);//invoke, like the unity.
```
