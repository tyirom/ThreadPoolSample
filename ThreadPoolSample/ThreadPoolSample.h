// �ȉ��� ifdef �u���b�N�� DLL ����̃G�N�X�|�[�g��e�Ղɂ���}�N�����쐬���邽�߂� 
// ��ʓI�ȕ��@�ł��B���� DLL ���̂��ׂẴt�@�C���́A�R�}���h ���C���Œ�`���ꂽ THREADPOOLSAMPLE_EXPORTS
// �V���{�����g�p���ăR���p�C������܂��B���̃V���{���́A���� DLL ���g�p����v���W�F�N�g�ł͒�`�ł��܂���B
// �\�[�X�t�@�C�������̃t�@�C�����܂�ł��鑼�̃v���W�F�N�g�́A 
// THREADPOOLSAMPLE_API �֐��� DLL ����C���|�[�g���ꂽ�ƌ��Ȃ��̂ɑ΂��A���� DLL �́A���̃}�N���Œ�`���ꂽ
// �V���{�����G�N�X�|�[�g���ꂽ�ƌ��Ȃ��܂��B
#ifdef THREADPOOLSAMPLE_EXPORTS
#define THREADPOOLSAMPLE_API __declspec(dllexport)
#else
#define THREADPOOLSAMPLE_API __declspec(dllimport)
#endif

// ���̃N���X�� ThreadPoolSample.dll ����G�N�X�|�[�g����܂����B
class THREADPOOLSAMPLE_API CThreadPoolSample {
public:
	CThreadPoolSample(void);
	// TODO: ���\�b�h�������ɒǉ����Ă��������B
};

extern THREADPOOLSAMPLE_API int nThreadPoolSample;

THREADPOOLSAMPLE_API int fnThreadPoolSample(void);
