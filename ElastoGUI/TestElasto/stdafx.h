// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <assert.h>
#include <tchar.h>
#include <fstream>

#include "opencv/cv.h"
#include "opencv/highgui.h"


// TODO: reference additional headers your program requires here
// �㷨�������Դ
#define	E_SOURCE_DLL     0      //����dll����
#define	E_SOURCE_GRAPHY  1      //����Elastography����
#define	E_SOURCE_SELF    2      //�Լ�д�ģ�������Ҫ����Elastography

#define ELASTO_CODE      E_SOURCE_SELF