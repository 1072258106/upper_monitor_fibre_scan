#include "StdAfx.h"
#include "Parser.h"


Parser::Parser(void)
{
}


Parser::~Parser(void)
{
}


int Parser::DoIt(BYTE **ppFrame, int &nSizeFrame, const BYTE *pBytes, int nBytes)
{
	int ret = -1;
	int nSynHead = sizeof(FrameSynHead) / sizeof(BYTE);
	int nSynTail = sizeof(FrameSynTail) / sizeof(BYTE);

	// ������ֽ�Ӧ�ô���һ������ͬ���ֵ���Ч����֡�ĳ���
	if (nBytes < (kValidFrameSize + nSynHead + nSynTail)) 
	{
		ret = -1;
		goto exit;
	}

	bool  found_head = false;
	bool  found_tail = false;

	BYTE *p = (BYTE*)pBytes;
	BYTE *pFrame = 0;

	// ����ͬ������֡ͷ
	for (;;)
	{
		if (memcmp(p, FrameSynHead, nSynHead) == 0)
		{ // �ҵ����˳�ѭ��
			found_head = true;
			pFrame = p + nSynHead;//��Ч���ݵĵ�ַ
			break;
		}
		else
		{ // ��һ��λ��
			p++;
		}
	}

	// ������ͬ������֡β
	if (found_head)
	{
		p += kValidFrameSize + nSynHead;
		if (p > (pBytes + nBytes - nSynTail))
		{ // ���ܳ������뻺�����ı߽�
			ret = -3;
		}
		else
		{
			found_tail = memcmp(p, FrameSynTail, nSynTail) == 0;
			if (found_tail)
			{ //�ҵ�֡β�� ���سɹ�
				*ppFrame = pFrame;
				ret = 0;
				nSizeFrame = kValidFrameSize;
			}
			else
			{
				ret = -4;
			}
		}
	}
	else
	{
		ret = -2;
	}

exit:
	return ret;
}


//////////////////////////////////////////////////////////////////////////
//  �ٵķ�����
//  ������λ��δ����ͬ���ֽڵ������ֻҪ���ݴ�����Ч���Ⱦ���Ϊ�Ϸ�
//////////////////////////////////////////////////////////////////////////
int Parser::DoDummy(BYTE **ppFrame, int &nSizeFrame, const BYTE *pBytes, int nBytes)
{
	int ret = (nBytes >= kValidFrameSize) ? 0 : -1;

	if (ret == 0)
	{
		*ppFrame = (BYTE*)pBytes;
		nSizeFrame = kValidFrameSize;
	}

	return ret;
}
