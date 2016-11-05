// 以下の ifdef ブロックは DLL からのエクスポートを容易にするマクロを作成するための 
// 一般的な方法です。この DLL 内のすべてのファイルは、コマンド ラインで定義された THREADPOOLSAMPLE_EXPORTS
// シンボルを使用してコンパイルされます。このシンボルは、この DLL を使用するプロジェクトでは定義できません。
// ソースファイルがこのファイルを含んでいる他のプロジェクトは、 
// THREADPOOLSAMPLE_API 関数を DLL からインポートされたと見なすのに対し、この DLL は、このマクロで定義された
// シンボルをエクスポートされたと見なします。
#ifdef THREADPOOLSAMPLE_EXPORTS
#define THREADPOOLSAMPLE_API __declspec(dllexport)
#else
#define THREADPOOLSAMPLE_API __declspec(dllimport)
#endif

// このクラスは ThreadPoolSample.dll からエクスポートされました。
class THREADPOOLSAMPLE_API CThreadPoolSample {
public:
	CThreadPoolSample(void);
	// TODO: メソッドをここに追加してください。
};

extern THREADPOOLSAMPLE_API int nThreadPoolSample;

THREADPOOLSAMPLE_API int fnThreadPoolSample(void);
