//////////////////////////////////////////////////////////////////////////
// ���紦��Ľӿ�
// ʹ��˵����
//     ����ǰ���ó�ʼ������Initialize
//     Ȼ�����DoEnvelop�����紦��
//     �����˳�ǰ����Release�ͷ���Դ
//////////////////////////////////////////////////////////////////////////

#ifndef INTERFACE_H_H_H
#define INTERFACE_H_H_H

#include <iostream>

struct CvMat;

#ifdef __cplusplus
extern "C"{
#endif

namespace mmode
{


extern void Initialize();


//////////////////////////////////////////////////////////////////////////
// ���紦��
// ���룺
//      pmatRF�� RF���ݣ� 32λfloat
//      file_hilber�� hilber�任��Ӱ��bmp
//      file_gray��   �Ҷ�ͼ��bmp
//////////////////////////////////////////////////////////////////////////
extern void  DoEnvelop(const CvMat *pmatRF, const char *file_hilber, const char *file_gray);

//extern void  DoEnvelop2(const CvMat *pmatRF, const char *file_hilber, CvMat *pmatGray);

extern void  Release();
}

#ifdef __cplusplus
}
#endif 

#endif
