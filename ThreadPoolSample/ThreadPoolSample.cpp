// ThreadPoolSample.cpp : DLL �A�v���P�[�V�����p�ɃG�N�X�|�[�g�����֐����`���܂��B
//

#include "stdafx.h"
#include "ThreadPoolSample.h"


// ����́A�G�N�X�|�[�g���ꂽ�ϐ��̗�ł��B
THREADPOOLSAMPLE_API int nThreadPoolSample=0;

// ����́A�G�N�X�|�[�g���ꂽ�֐��̗�ł��B
THREADPOOLSAMPLE_API int fnThreadPoolSample(void)
{
    return 42;
}

// ����́A�G�N�X�|�[�g���ꂽ�N���X�̃R���X�g���N�^�[�ł��B
// �N���X��`�Ɋւ��Ă� ThreadPoolSample.h ���Q�Ƃ��Ă��������B
CThreadPoolSample::CThreadPoolSample()
{
    return;
}
