#pragma once

//////////////////////////////////////////////////////////////////////////
// ����֡������
//
//////////////////////////////////////////////////////////////////////////

const  BYTE  FrameSynHead[] = {0xff, 0x5b, 0x50, 0x00, 0xff, 0x5d, 0x50, 0x00}; // ����֡-֡ͷͬ����
const  BYTE  FrameSynTail[] = {0xff, 0xff, 0x84, 0x13, 0xff, 0xff, 0x14, 0x21}; // ����֡-֡βͬ����

const  int   kValidFrameSize = 4096 * 300 * 2; // ����֡��Ч�ֽڵ�size
const  int   kTotalFrameSize = kValidFrameSize + sizeof(FrameSynHead) / sizeof(BYTE) + sizeof(FrameSynTail) / sizeof(BYTE);

class Parser
{
public:
	Parser(void);
	virtual ~Parser(void);

	//////////////////////////////////////////////////////////////////////////
	//  ���з�����
	//  ���룺
	//      pBytes,  ������ֽ���
	//      nBytes,  �����ֽڵ�����
	//  �����
	//      pFrame,       ȥ����֡ͷ��֡βͬ�����Ժ����Ч���ݵ�ַ
	//      nSizeFrame,   ����֡�������Ĵ�С���ֽڵ�λ�� �����Ժ�������õ��ǽ���������֡�Ĵ�С
	//  ���أ�
	//     0�� ��ʾ�����ɹ���
	//     ��0�� ����ʧ�ܣ�
	//        -1�� �������ݵĳ��Ȳ�������һ������֡
	//        -2�� �Ҳ���֡ͷ
	//        -3�� ����֡��������֡ͷ�ҵ��ˣ�
	//        -4�� �Ҳ���֡β��֡ͷ�ҵ��ˣ�
	//////////////////////////////////////////////////////////////////////////
	static int   DoIt(BYTE **ppFrame, int &nSizeFrame, const BYTE *pBytes, int nBytes);

	//////////////////////////////////////////////////////////////////////////
    //  �ٵķ�����
	//  ������λ��δ����ͬ���ֽڵ������ֻҪ���ݴ�����Ч���Ⱦ���Ϊ�Ϸ�
	//////////////////////////////////////////////////////////////////////////
	static int   DoDummy(BYTE **ppFrame, int &nSizeFrame, const BYTE *pBytes, int nBytes);

protected:


private:
};

