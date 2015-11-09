#ifndef CSTRAIN_H_H_H
#define CSTRAIN_H_H_H
#pragma   once 

#include "opencv\cv.h"
#include "CElasto.h"
#include "CDataProcess.h"
#include <iostream>
class CStrain : public CDataProcess{

public:
	CStrain();
	void Do();
//private:
	float strainAlgorithm(const EInput &input, EOutput &output);

	void  CalcStrain(const EInput &input, EOutput &output);
	void  CalcStrain2(const EInput &input, EOutput &output);

	//////////////////////////////////////////////////////////////////////////
	// �����任
	// ���룺
	//   pmatDisplacement,   ����-Ӧ�䣻
	// �����
	//   ppmatRadon,   ָ��ĵ�ַ�� ��������һ�����󱣴������任�Ľ���������������ĵ�ַ
	//                 ������ppmatRadon�С��û���ʹ�ñ����ͷ�����
	//////////////////////////////////////////////////////////////////////////
	static void  RadonSum(const CvMat *pmatDisplacement, CvMat **ppmatRadan);

private:

	//////////////////////////////////////////////////////////////////////////
	// ����Ӧ��ֵ��Ӧ��ͼ�ĻҶ�ֵ
	// ���룺
	//    count�� ��ϵĵ���
	//    pmat��  Ӧ��ľ���
	//    pimg��  Ӧ��ͼ
	//////////////////////////////////////////////////////////////////////////
	void  ComputeStrainValueAndImage(int count, CvMat *pmat, IplImage *pimg);
};
#endif //define CSTRAIN_H_H_H