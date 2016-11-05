// ThreadPoolSample.cpp : DLL アプリケーション用にエクスポートされる関数を定義します。
//

#include "stdafx.h"
#include "ThreadPoolSample.h"


// これは、エクスポートされた変数の例です。
THREADPOOLSAMPLE_API int nThreadPoolSample=0;

// これは、エクスポートされた関数の例です。
THREADPOOLSAMPLE_API int fnThreadPoolSample(void)
{
    return 42;
}

// これは、エクスポートされたクラスのコンストラクターです。
// クラス定義に関しては ThreadPoolSample.h を参照してください。
CThreadPoolSample::CThreadPoolSample()
{
    return;
}
